#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <setjmp.h>

using namespace std;

enum NonTerminal
{
    Program, ProgramHead, ProgramName, DeclarePart,
    TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
    TypeId, TypeName, BaseType, StructureType,
    ArrayType, Low, Top, RecType,
    FieldDecList, FieldDecMore, IdList, IdMore,
    VarDec, VarDeclaration, VarDecList, VarDecMore,
    VarIdList, VarIdMore, ProcDec, ProcDeclaration,
    ProcDecMore, ProcName, ParamList, ParamDecList,
    ParamMore, Param, FormList, FidMore,
    ProcDecPart, ProcBody, ProgramBody, StmList,
    StmMore, Stm, AssCall, AssignmentRest,
    ConditionalStm, StmL, LoopStm, InputStm,
    InVar, OutputStm, ReturnStm, CallStmRest,
    ActParamList, ActParamMore, RelExp, OtherRelE,
    Exp, OtherTerm, Term, OtherFactor,
    Factor, Variable, VariMore, FieldVar,
    FieldVarMore, CmpOp, AddOp, MultOp
};

enum Terminal
{
    ENDFILE, ERROR,
    PROGRAM, PROCEDURE, TYPE, VAR, IF,
    THEN, ELSE, FI, WHILE, DO, ENDWH,
    BEGIN, END1, READ, WRITE, ARRAY, OF,
    RECORD, RETURN1,
    INTEGER, CHAR, //INTEGER 是integer
    ID, INTC, CHARC,
    ASSIGN, EQ, LT, PLUS, MINUS,
    TIMES, OVER, LPAREN, RPAREN, DOT,
    COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
    UNDERANGE
};

typedef Terminal LexType;

struct Word
{
    string str;
    LexType tok;
};

struct Token {
    int line;
    Word wd;
    int index = -1;
};

struct TreeNode
{
    string name;
    Token* tk;
    vector<TreeNode*> child;
    TreeNode(string nm)
    {
        name = nm;
        tk = 0;
        child.resize(0);
        for (auto& s : child)
            s = 0;
    }

    void addChild(TreeNode* t) {
        child.push_back(t);
    }
};

struct StackNode
{
    int ntflag = -1;

    NonTerminal n = (NonTerminal)-1;
    Terminal t = (Terminal)-1;

    TreeNode* p = nullptr;

    StackNode* next = nullptr;

};


// LL1
/*
struct Node {

    string nodekind; // 记录语法树的结点类型
    Node* child[3] = { nullptr }; // 指向子语法树结点指针
    Node* sibling = nullptr; // 指向兄弟语法树结点指针
    int lineno; // 记录源程序行号
    union {
        const char* dec;
        const char* stmt;
        const char* exp;
    }kind; // 记录语法树结点的具体类型，为共用体结构
    int idnum = 0; // 记录一个结点中标识符的个数
    string name[10]; // 结点中标识符的名字
    Token* table[10]; // 指针数组，数组成员是结点中各个标识符在符号表中的入口
    string type_name; // 记录类型名，当结点是声明类型，且类型是由类型标识符表示时有效
    struct {
        struct {
            int low; // 整型类型变量，记录数组的下界
            int up; // 整型类型变量，记录数组的上界
            string childType; // 记录数组的成员类型
        }ArrayAttr; // 记录数组类型的属性
        struct {
            string paramt; // valparamtype, varparamtype; // 记录过程的参数类型，表示过程的参数是值参或变参
        }ProcAttr; // 记录过程的属性
        struct {
            string op; //LT, EQ, PLUS, MINUS, TIMES, OVER; // 记录语法树结点的运算符单词，为单词类型。
            // 当语法树结点为"关系运算表达式"对应结点时，取值LT、EQ
            // 当语法树结点为"加法运算简单表达式"对应结点时，取值PLUS、MINUS
            // 当语法树结点为"乘法运算项"对应结点时，取值TIMES、OVER
            // 其余情况下无效
            int val;
            string varkind; //IdV, ArrayMembV, FieldMembV; // 记录语法树结点的数值，当语法树结点为"数字因子"对应的语法树结点时有效，为整数类型
            string type; // Void, Integer, Boolean; // 记录语法树结点的检查类型，取值Void, Integer, Boolean, 为类型检查ExpType类型
        }ExpAttr; // 记录表达式的属性
    } attr;
    Node(string s) {
        this->nodekind = s;
    }
};
*/

struct AnalysisStack
{
    StackNode* head = nullptr; // you can use -> to access the top element directly, using top() function is also accessible.
    int isEmpty = 1;

    void push(int ntflg, int num)
    {
        StackNode* p = new StackNode();

        if (ntflg == 0)
            p->n = (NonTerminal)num;
        else if (ntflg == 1)
            p->t = (Terminal)num;

        p->ntflag = ntflg;
        p->next = head;
        head = p;
        isEmpty = 0;
    }

    void push(int ntflg, int num, string _name)
    {
        StackNode* p = new StackNode();

        p->p = new TreeNode(_name);

        if (ntflg == 0)
            p->n = (NonTerminal)num;
        else if (ntflg == 1)
            p->t = (Terminal)num;

        p->ntflag = ntflg;
        p->next = head;
        head = p;
        isEmpty = 0;

    }

    void push(int ntflg, int num, string _name, int useless)
    {
        StackNode* p = new StackNode();

        p->p = new TreeNode(_name);

        if (ntflg == 0)
            p->n = (NonTerminal)num;
        else if (ntflg == 1)
            p->t = (Terminal)num;

        p->ntflag = ntflg;
        p->next = head;
        head = p;
        isEmpty = 0;

    }

    void pop()
    {
        StackNode* tmp = head;
        head = head->next;
        if (!head)
            isEmpty = 1;
        // delete(tmp);
    }

    StackNode* top() const
    {
        return this->head;
    }

    int IsEmpty() const
    {
        return this->isEmpty;
    }
};


extern jmp_buf jump_buffer;
