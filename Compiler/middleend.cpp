#include <map>
#include "middleend.h"
#include "objects.h"

static void processAssign(Tree *func, Tree *op);
static void processStatement(Tree *func, Tree *st);
static void processScope(Tree *func, Tree *sc);
static Tree *processShift(Tree *func, Tree *op, TreeType t);
static Tree *processModify(Tree *func, Tree *op, TreeType t);

static Tree *processList(Tree *func, Tree *list, bool addr)
{
    Tree *res = new Tree(list);
    for (Tree *it = list->down() ; it ; it = it->right())
    {
        switch (it->getType())
        {
        case TreeType::VAR:
        case TreeType::SYMBOL:
            res->addChild(it->clone());
            break;
        case TreeType::INT:
            {
                int val = it->get<int>();
                int max = addr ? 255 : 7;
                if (val < 0 || val > max)
                {
                    Tree *temp = createTemp();
                    res->addChild(temp);
                    Tree *assign = new Tree(TreeType::ASSIGN,
                        new Tree(TreeType::LIST, temp->clone()),
                        new Tree(TreeType::LIST, it->clone()));
                    func->addChild(assign);
                }
                else
                {
                    res->addChild(it->clone());
                }
            }
            break;
        default:
            assert(0);
        }
    }
    return res;
}

// returns LIST of the variables/consts
static Tree *processExpr(Tree *func, Tree *expr, bool addr = false)
{
    switch (expr->getType())
    {
    case TreeType::LIST:
        return processList(func, expr, addr);
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
        return processShift(func, expr, TreeType::LSHIFT)->clone();
    case TreeType::RSHIFT:
        return processShift(func, expr, TreeType::RSHIFT)->clone();
    case TreeType::ADD_ASSIGN:
        return processModify(func, expr, TreeType::ADD)->clone();
    case TreeType::SUB_ASSIGN:
        return processModify(func, expr, TreeType::SUB)->clone();
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

static void processOut(Tree *func, Tree *op)
{
    Tree *res = new Tree(TreeType::OUT);
    // TODO: address calculation
    res->addChild(processExpr(func, op->down(), true));
    res->addChild(processExpr(func, op->down()->right()));
    func->addChild(res);
}

static void processSimpleCond(Tree *func, Tree *cond, Tree *label_then, Tree *label_else)
{
    /* can have side effects */
    if (cond->down()) {
        processExpr(func, cond->down());
    }
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
        if_op->addChild(new Tree(TreeType::NONZERO));
        break;
    case TreeType::NONZERO:
        if_op->addChild(new Tree(TreeType::ZERO));
        break;
    case TreeType::SIGN:
        if_op->addChild(new Tree(TreeType::NSIGN));
        break;
    case TreeType::NSIGN:
        if_op->addChild(new Tree(TreeType::SIGN));
        break;
    case TreeType::ALWAYS:
        delete if_op;
        /* do nothing for comparison */
        return;
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
        if_op->addChild(new Tree(TreeType::NONZERO));
        break;
    case TreeType::NEQUAL:
        if_op->addChild(new Tree(TreeType::ZERO));
        break;
    case TreeType::GREATER:
        if_op->addChild(new Tree(TreeType::NSIGN));
        break;
    case TreeType::LESS:
        if_op->addChild(new Tree(TreeType::SIGN));
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
    case TreeType::GREATER:
    case TreeType::LESS:
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

static void processHalt(Tree *func, Tree *op)
{
    func->addChild(op->clone());
}

static Tree *processShift(Tree *func, Tree *op, TreeType t)
{
    Tree *res = new Tree(t);
    Tree *temp = createTempVar(op->down(), nullptr);
    res->addChild(temp);
    res->addChild(op->down()->clone());
    func->addChild(res);
    Tree *assign = new Tree(TreeType::ASSIGN,
        op->down()->clone(), temp->clone());
    func->addChild(assign);
    return temp;
}

static Tree *processModify(Tree *func, Tree *op, TreeType t)
{
    Tree *res = new Tree(t);
    Tree *temp = createTempVar(op->down(), nullptr);
    res->addChild(temp);
    res->addChild(op->down()->clone());
    res->addChild(processExpr(func, op->down()->right()));
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
    case TreeType::ADD_ASSIGN:
        processModify(func, st, TreeType::ADD);
        break;
    case TreeType::SUB_ASSIGN:
        processModify(func, st, TreeType::SUB);
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
        processShift(func, st, TreeType::LSHIFT);
        break;
    case TreeType::RSHIFT:
        processShift(func, st, TreeType::RSHIFT);
        break;
    case TreeType::OUT:
        processOut(func, st);
        break;
    case TreeType::HALT:
        processHalt(func, st);
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

/////////////////////////////////////////////////////////////////

// static void checkBinaryRanges(Tree *stat)
// {
//     Tree *last = stat->down()->right()->right();
//     if (last->getType() == TreeType::LIST)
//     {
//         last = last->down();
//     }
//     assert(!last->right());
//     if (last->getType() == TreeType::INT)
//     {
//         int val = last->get<int>();
//         if (val < 0 || val > 7)
//         {
//             Tree *temp = createTemp();
//             last->replaceWith(temp);
//             Tree *assign = new Tree(TreeType::ASSIGN,
//                 new Tree(TreeType::LIST, temp->clone()),
//                 new Tree(TreeType::LIST, last));
//             stat->insertLeft(assign);
//         }
//     }
// }

// static void checkRangesFunction(Tree *root)
// {
//     // Tree *it = root->down();
//     // while (it)
//     // {
//     //     switch (it->getType())
//     //     {
//     //     case TreeType::ADD:
//     //     case TreeType::SUB:
//     //         checkBinaryRanges(it);
//     //         break;
//     //     }
//     //     it = it->right();
//     // }
// }

// void checkRanges(Tree *root)
// {
//     Tree *it = root->down();
//     while (it)
//     {
//         checkRangesFunction(it);
//         it = it->right();
//     }
// }

/////////////////////////////////////////////////////////////////

static void createVariablesFunc( Tree *func)
{
    typedef std::map<int, Trees> AllVars;
    // Variables coming to labels
    // std::map<CodeGraph::Node*, AllVars> nodeVars;
    typedef std::map<int, Tree*> Vars;
    // collect variables
    // CodeGraph::Node *n = graph->getNodes().front();
    // graph->clearVisited();
    Vars vars;
#if 0
    for (Tree *st = func->down() ; st ; st = st->right())
    {
/*
    graph->dfs(n, 1, [&nodeVars](CodeGraph::Node *node, int unused)
        {
            Tree *tree = node->info;
            */
            TreeType tt = st->getType();
            Tree *left = st->down();
/*            if (tt == TreeType::LABEL)
            {
                // save the vars
                for (auto &vv : vars)
                {
                    Trees &dest = nodeVars[node][vv.first];
                    // new SSA var
                    if (dest.empty())
                    {
                        dest.push_back(createTemp());
                        // add symbol for reference
                        dest.front()->addChild(new Tree(vv.second->down()));
                    }
                    // label contains all incoming values
                    //tree->addChild(vv.second->clone());
                    dest.push_back(vv.second);
                    vv.second = dest.front();
                }
            }
            else */if (st->isModification()
                && left && left->getType() == TreeType::LIST)
            {
                // modifies first argument
                for (Tree *sym = left->down() ; sym ; sym = sym->right())
                {
                    if (sym->getType() == TreeType::SYMBOL)
                    {
                        //int var = sym->get<int>();
                        //Tree *temp = createTemp();
                        //vars[var] = temp;
                        //sym->insertParent(temp);
                        //sym = temp;
                        sym->setType(TreeType::VAR);
                    }
                }
            }
        }
#endif
    //);
    /*
    // insert phi
    for (auto nv : nodeVars)
    {
        for (auto vt : nv.second)
        {
            Tree *phiNode = new Tree(TreeType::PHI);
            for (auto t : vt.second)
            {
                // phi collects the values
                phiNode->addChild(t->clone());
            }
            nv.first->info->insertRight(phiNode);
        }
    }
    */
    // replace usages
    // TODO: loop to vars
    //Vars phi;
    for (Tree *st = func->down() ; st ; st = st->right())
    {
        Tree *op = st->down();
        // if (op && st->isModification())
        // {
        //     // Tree *v = op;
        //     // if (v->getType() == TreeType::LIST)
        //     // {
        //     //     v = v->down();
        //     // }
        //     // v->setFlag(FLAG_DEF);
        //     op = op->right();
        // }
        // only usages, not assignments
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
                    v->setType(TreeType::VAR);
                    // Tree *n;
                    // int var = v->get<int>();
                    // /*
                    // n = phi[var];
                    // if (!n)
                    // {
                    //     n = createTemp();
                    // }
                    // else
                    // {
                    //     n = n->clone();
                    // }
                    // */
                    // n = vars[var];
                    // if (n)
                    // {
                    //     n = n->clone();
                    // }
                    // else
                    // {
                    //     n = createTemp();
                    // }
                    // // n->setFlag(FLAG_USE);
                    // v->replaceWith(n);
                    // std::swap(v, n);
                    // delete n;
                }
                // else if (v->getType() == TreeType::VAR)
                // {
                //     v->setFlag(FLAG_USE);
                // }
            }
        }
        /*
        if (st->getType() == TreeType::PHI)
        {
            Tree *var = st->down();
            Tree *sym = var->down();
            phi[sym->get<int>()] = var;
        }
        */
    }
}

void createVariables(Tree *root)
{
    root = root->down();
    while (root)
    {
        createVariablesFunc(root);
        root = root->right();
    }
}

void insertReturns(Tree *root)
{
    root = root->down();
    while (root)
    {
        root->addChild(new Tree(TreeType::RETURN));
        root = root->right();
    }
}
