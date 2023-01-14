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
    func->addChild(op->clone());
    return op->down();
}

static Tree *processRShift(Tree *func, Tree *op)
{
    func->addChild(op->clone());
    return op->down();
}

static void processStatement(Tree *func, Tree *st)
{
    switch (st->getType())
    {
    case TreeType::SCOPE:
        processScope(func, st);
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
