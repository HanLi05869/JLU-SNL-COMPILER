#pragma once
#include "func.h"
#include "global.h"
#include "globalVar.h"
#include "lexical.h"
#include <string>
#include <cstring>

#include <tuple>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

enum class NodeKind
{
    /* 标志结点 */
    ProK, // 根标志结点
    PheadK, // 程序头标志结点
    TypeK, // 类型声明标志结点
    VarK, // 变量声明标志结点
    ProcDecK, // 函数声明标志结点
    StmLK, // 语句序列标志结点
    /* 具体结点 */
    DecK, // 声明结点
    StmtK, // 语句结点
    ExpK //表达式结点
};

enum class DecKind
{
    ArrayK, // 组类型
    CharK, // 字符类型
    IntegerK, // 整数类型
    RecordK, // 记录类型
    IdK // 类型标识符作为类型
};

enum class StmtKind
{
    IfK, // 判断语句类型
    WhileK, // 循环语句类型
    AssignK, // 赋值语句类型
    ReadK, // 读语句类型
    WriteK, // 写语句类型
    CallK, // 函数调用语句类型
    ReturnK // 返回语句类型
};

enum class ExpKind
{
    OpK, // 操作符类型
    ConstK, // 常整数类型
    VariK // 标识符类型
};

union Kind
{
    DecKind dec;
    StmtKind stmt;
    ExpKind exp;
};

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
            int op; //LT, EQ, PLUS, MINUS, TIMES, OVER; // 记录语法树结点的运算符单词，为单词类型。
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

class SyntaxTree
{
public:
    SyntaxTree();
    ~SyntaxTree();
    void setRoot(Node* node);
    Node* getRoot();
private:
    Node* root;
};


class ParserLL1
{
public:
    SyntaxTree* run();
private:
    stack<tuple<int, int, string>> symbal_stack; // LL1分析栈
    stack<Node**> syntaxtree_stack; // 语法树结构
    stack<Node*> op_stack;
    stack<Node*> num_stack;

    Node* current_treenode; // 指向当前的节点
    const char** temp;
    Node* saveP;
    bool getExpResult = true;
    int expflag = 0;
    int getExpResult2 = false;
public:
    // static int nowLine;
    // static int head;
    static Node* newSpecNode(string kind);
    static Node* newStmtNode(string kind);
    static Node* newExpNode(string kind);
private:
    int getPriosity(int op);
    void process(int id);
};


// extern void show(Node* root, int depth, vector<int>& v, bool islast);

extern void printTreeNewLL1(Node* t, int layer, bool islastson);


extern string outstrLL1new[512];
extern int strlineLL1new;

extern string NodePrintInfo(Node* node);
