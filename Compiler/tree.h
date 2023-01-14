#ifndef NODE_H
#define NODE_H

#include <variant>
#include <memory>
#include <list>
#include <cassert>

enum class TreeType
{
    NONE,
    INT,
    VAR,
    SYMBOL,
    FUNC,
    LABEL,
    LIST,
    SCOPE,
    ASSIGN,
    RETURN,
    IF,
    WHILE,
    GOTO,
    CMP,
    PHI,
    LSHIFT,
    RSHIFT,
    ALWAYS,
    CARRY,
    NCARRY,
    ZERO,
    NZERO,
    SIGN,
    NSIGN,
    EQUAL,
    NEQUAL,
    // same as binary instructions
    // ADD, SUB, OR, XOR, AND

    ////////////////
    // backend nodes
    ////////////////
    REG,
    // operations
    MOV,
    JMP,
    // binary ops
    ADC,
    ADD,
    SBC,
    SUB,
    AND,
    OR,
    XOR,
    // unary ops
    SHR,
};

class Tree;
typedef std::list<Tree*> Trees;

class Tree
{
public:
    Tree()
        : parent(nullptr), prev(nullptr), next(nullptr), child(nullptr),
          type(TreeType::NONE)
    {}
    explicit Tree(int v, TreeType t = TreeType::INT)
        : Tree()
    {
        value = v;
        type = t;
    }
    explicit Tree(char v)
        : Tree((int)v, TreeType::SYMBOL)
    {}
    explicit Tree(char *v)
        : Tree()
    {
        value = v;
        type = TreeType::FUNC;
    }
    // copy properties, not links
    explicit Tree(const Tree *n)
        : Tree()
    {
        value = n->value;
        type = n->type;
    }

    Tree(TreeType t, Tree *n1 = nullptr, Tree *n2 = nullptr, Tree *n3 = nullptr)
        : Tree()
    {
        type = t;
        if (n1)
        {
            addChild(n1);
        }
        if (n2)
        {
            addChild(n2);
        }
        if (n3)
        {
            addChild(n3);
        }
    }

    Tree *clone();

    void setType(TreeType t) { type = t; }
    TreeType getType() const { return type; }

    void addFirstChild(Tree *n);
    void addChild(Tree *n);
    void merge(Tree *n);
    void insertParent(Tree *n);
    void insertRight(Tree *n);
    void replaceWith(Tree *n);

    Tree *up() { return parent; }
    Tree *down() { return child; }
    Tree *right() { return next; }
    Tree *left() { return prev; }

    std::string getTreeName() const;

    void print(int level = 0) const;

    template <typename T> T get() const { return std::get<T>(value); }

private:
    void printTree(int level) const;

private:
    std::variant<
        int, // integer literal or variable
        std::string // function name
        > value;

    Tree *parent, *prev, *next, *child;
    TreeType type;
};

#endif
