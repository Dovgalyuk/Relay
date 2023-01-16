#include <map>
#include "middleend.h"
#include "objects.h"

static void processAssign(Tree *func, Tree *op);
static void processStatement(Tree *func, Tree *st);
static void processScope(Tree *func, Tree *sc);
static Tree *processLShift(Tree *func, Tree *op);
static Tree *processRShift(Tree *func, Tree *op);

// returns LIST of the variables/consts
static Tree *processExpr(Tree *func, Tree *expr)
{
    switch (expr->getType())
    {
    case TreeType::LIST:
        // don't reallocate
        return expr;
    case TreeType::ADD:
    case TreeType::SUB:
        {
            Tree *left = processExpr(func, expr->down());
            Tree *right = processExpr(func, expr->down()->right());
            Tree *res = createTempVar(left, right);
            Tree *op = new Tree(expr->getType());
            op->addChild(res);
            op->addChild(left);
            op->addChild(right);
            func->addChild(op);
            return res->clone();
        }
    case TreeType::LSHIFT:
        return processLShift(func, expr)->clone();
    case TreeType::RSHIFT:
        return processRShift(func, expr)->clone();
    default:
        assert(0);
    }
    return nullptr;
}

static void processAssign(Tree *func, Tree *op)
{
    Tree *res = new Tree(TreeType::ASSIGN);
    res->addChild(op->down()->clone());
    res->addChild(processExpr(func, op->down()->right()));
    func->addChild(res);
}

static void processSimpleCond(Tree *func, Tree *cond, Tree *label_then, Tree *label_else)
{
    processExpr(func, cond->down());
    // add if
    Tree *if_op = new Tree(TreeType::IF);
    switch (cond->getType())
    {
    case TreeType::CARRY:
        if_op->addChild(new Tree(TreeType::NCARRY));
        break;
    case TreeType::NCARRY:
        if_op->addChild(new Tree(TreeType::CARRY));
        break;
    case TreeType::ZERO:
        if_op->addChild(new Tree(TreeType::NZERO));
        break;
    case TreeType::NZERO:
        if_op->addChild(new Tree(TreeType::ZERO));
        break;
    case TreeType::SIGN:
        if_op->addChild(new Tree(TreeType::NSIGN));
        break;
    case TreeType::NSIGN:
        if_op->addChild(new Tree(TreeType::SIGN));
        break;
    default:
        assert(0);
    }
    if_op->addChild(label_else->clone());
    func->addChild(if_op);
}

static void processComparison(Tree *func, Tree *cond, Tree *label_then, Tree *label_else)
{
    Tree *left = processExpr(func, cond->down());
    Tree *right = processExpr(func, cond->down()->right());
    Tree *op = new Tree(TreeType::CMP, left, right);
    func->addChild(op);
    // add if
    Tree *if_op = new Tree(TreeType::IF);
    switch (cond->getType())
    {
    case TreeType::EQUAL:
        if_op->addChild(new Tree(TreeType::NZERO));
        break;
    case TreeType::NEQUAL:
        if_op->addChild(new Tree(TreeType::ZERO));
        break;
    default:
        assert(0);
    }
    if_op->addChild(label_else->clone());
    func->addChild(if_op);
}

static void processCond(Tree *func, Tree *cond, Tree *label_then, Tree *label_else)
{
    switch (cond->getType())
    {
    case TreeType::AND:
    case TreeType::OR:
        // TODO
        break;
    case TreeType::EQUAL:
    case TreeType::NEQUAL:
        processComparison(func, cond, label_then, label_else);
        break;
    default:
        processSimpleCond(func, cond, label_then, label_else);
    }
}

static void processIf(Tree *func, Tree *op)
{
    Tree *label_then = createLabel();
    Tree *label_else = createLabel();
    Tree *then_op = op->down()->right();
    Tree *else_op = then_op->right();
    // add cond + if
    processCond(func, op->down(), label_then, label_else);
    // add then
    func->addChild(label_then);
    processStatement(func, then_op);
    // add else
    func->addChild(label_else);
    if (else_op)
    {
        processStatement(func, else_op);
    }
}

static void processWhile(Tree *func, Tree *op)
{
    Tree *label_cont = createLabel();
    Tree *label_body = createLabel();
    Tree *label_exit = createLabel();

    /*
    Label:
      CMP
      JMP Cond, Exit
      Body
      JMP Label
    Exit:
    */
    func->addChild(label_cont);
    processCond(func, op->down(), label_body, label_exit);
    func->addChild(label_body);
    processStatement(func, op->down()->right());
    func->addChild(new Tree(TreeType::GOTO, label_cont->clone()));
    func->addChild(label_exit);
}

static void processReturn(Tree *func, Tree *op)
{
    Tree *res = new Tree(TreeType::RETURN);
    res->addChild(processExpr(func, op->down()));
    func->addChild(res);
}

static Tree *processLShift(Tree *func, Tree *op)
{
    Tree *res = new Tree(TreeType::LSHIFT);
    Tree *temp = createTempVar(op->down(), nullptr);
    res->addChild(temp);
    res->addChild(op->down()->clone());
    func->addChild(res);
    Tree *assign = new Tree(TreeType::ASSIGN,
        op->down()->clone(), temp->clone());
    func->addChild(assign);
    return temp;
}

static Tree *processRShift(Tree *func, Tree *op)
{
    Tree *res = new Tree(TreeType::RSHIFT);
    Tree *temp = createTempVar(op->down(), nullptr);
    res->addChild(temp);
    res->addChild(op->down()->clone());
    func->addChild(res);
    Tree *assign = new Tree(TreeType::ASSIGN,
        op->down()->clone(), temp->clone());
    func->addChild(assign);
    return temp;
}

static void processArgs(Tree *func, Tree *st)
{
    int reg = 1;
    for (st = st->down() ; st ; st = st->right(), ++reg)
    {
        Tree *assign = new Tree(TreeType::ASSIGN);
        assign->addChild(new Tree(TreeType::LIST, st->clone()));
        assign->addChild(new Tree(TreeType::LIST,
            new Tree(reg, TreeType::REG)));
        func->addChild(assign);
    }
    assert(reg < 7);
}

static void processStatement(Tree *func, Tree *st)
{
    switch (st->getType())
    {
    case TreeType::SCOPE:
        processScope(func, st);
        break;
    case TreeType::ARGS:
        processArgs(func, st);
        break;
    case TreeType::ASSIGN:
        processAssign(func, st);
        break;
    case TreeType::IF:
        processIf(func, st);
        break;
    case TreeType::WHILE:
        processWhile(func, st);
        break;
    case TreeType::RETURN:
        processReturn(func, st);
        break;
    case TreeType::LSHIFT:
        processLShift(func, st);
        break;
    case TreeType::RSHIFT:
        processRShift(func, st);
        break;
    default:
        break;
    }
}

static void processScope(Tree *func, Tree *sc)
{
    Tree *it = sc->down();
    while (it)
    {
        processStatement(func, it);
        it = it->right();
    }
}

static Tree *processFunction(Tree *func)
{
    Tree *newFunc = new Tree(func);
    processScope(newFunc, func->down());
    return newFunc;
}

Tree *makeOperations(Tree *root)
{
    Tree *newRoot = new Tree(TreeType::LIST);
    // iterate over functions
    Tree *it = root->down();
    while (it)
    {
        newRoot->addChild(processFunction(it));
        it = it->right();
    }
    delete root;
    return newRoot;
}

static void createVariablesFunc(CodeGraph *graph, Tree *func)
{
    typedef std::map<int, Trees> AllVars;
    std::map<CodeGraph::Node*, AllVars> nodeVars;
    typedef std::map<int, Tree*> Vars;
    // collect variables
    CodeGraph::Node *n = graph->getNodes().front();
    graph->clearVisited();
    graph->dfs(n, Vars(), [&nodeVars](CodeGraph::Node *node, Vars &vars)
        {
            Tree *tree = node->info;
            TreeType tt = tree->getType();
            Tree *left = tree->down();
            if (tt == TreeType::LABEL)
            {
                // save the vars
                for (auto &vv : vars)
                {
                    Trees &dest = nodeVars[node][vv.first];
                    if (dest.empty())
                    {
                        dest.push_back(createTemp());
                        // add symbol for reference
                        dest.front()->addChild(new Tree(vv.second->down()));
                    }
                    dest.push_back(vv.second);
                    vv.second = dest.front();
                }
            }
            else if (tt != TreeType::CMP && tt != TreeType::RETURN
                && left && left->getType() == TreeType::LIST)
            {
                // modifies first argument
                for (Tree *sym = left->down() ; sym ; sym = sym->right())
                {
                    if (sym->getType() == TreeType::SYMBOL)
                    {
                        int var = sym->get<int>();
                        Tree *temp = createTemp();
                        vars[var] = temp;
                        sym->insertParent(temp);
                        sym = temp;
                    }
                }
            }
        }
    );
    // insert phi
    for (auto nv : nodeVars)
    {
        for (auto vt : nv.second)
        {
            Tree *phiNode = new Tree(TreeType::PHI);
            for (auto t : vt.second)
                phiNode->addChild(t->clone());
            nv.first->info->insertRight(phiNode);
        }
    }
    // replace usages
    Vars phi;
    for (Tree *st = func->down() ; st ; st = st->right())
    {
        Tree *op = st->down();
        if (op && st->getType() != TreeType::CMP
            && st->getType() != TreeType::RETURN)
        {
            op = op->right();
        }
        for ( ; op ; op = op->right())
        {
            Tree *v = op;
            if (v->getType() == TreeType::LIST)
            {
                v = v->down();
            }
            for ( ; v ; v = v->right())
            {
                if (v->getType() == TreeType::SYMBOL)
                {
                    Tree *n = phi[v->get<int>()];
                    if (!n)
                    {
                        n = createTemp();
                    }
                    else
                    {
                        n = n->clone();
                    }
                    v->replaceWith(n);
                    std::swap(v, n);
                    delete n;
                }
            }
        }
        if (st->getType() == TreeType::PHI)
        {
            Tree *var = st->down();
            Tree *sym = var->down();
            phi[sym->get<int>()] = var;
        }
    }
}

void createVariables(CodeGraphList *graphs, Tree *root)
{
    root = root->down();
    for (auto g : *graphs)
    {
        createVariablesFunc(g, root);
        root = root->right();
    }
}
