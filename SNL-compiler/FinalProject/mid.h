#pragma once
#include <iostream>
#include <vector>
#include "func.h"
#include "global.h"
#include "globalVar.h"
#include "lexical.h"
#include "LL1.h"
#include "newLL1.h"
#include "semantic.h"
#include <map>
#include <unordered_map>

using namespace std;

struct ARG
{
    ARG();
    ARG(string _name);
    string form; // 取值是ValueForm, LabelForm, AddrForm
    int value; // form == ValueForm 记录 常数 的数值
    int label; // form == LabelForm 记录 标号 的数值
    string Addr; // 变量的ARG结构的内容
    string name; // 记录变量的名字 若为临时变量，则为空
    int dataLevel; // 记录变量标识符所在的层数 若为临时变量，取-1
    int dataOff;// 记录变量的标识符的偏移量 若是临时变量 取值临时变量的编码
    int access; // 记录变量的访问方式，根据是直接还是非直接，取值 1是dir 2是indir
};

struct Argnode {
    string codekind;
    ARG* Arg1;
    ARG* Arg2;
    ARG* res;
};

extern vector<Argnode> midtable;

struct midprintnode {
    string op;
    string m1;
    string m2;
    string m3;
};

extern vector<Node*> ArrayTable;
extern vector<Node*> FieldTable;

extern int level;


extern unordered_map<int, vector<pair<int, Node*>>> mapArray;
extern unordered_map<int, vector<pair<int, Node*>>> mapRecord;
extern vector<midprintnode> MidPrintNodeTable;

extern int tmpidx;


extern void GenAssignS(Node* t);
extern void GenCallS(Node* t);
extern void GenReadS(Node* t);
extern void GenWriteS(Node* t);
extern void GenIfS(Node* t);
extern void GenWhileS(Node* t);
extern void GenBody(Node* t);
extern string getBehind(Node* t);
extern void GenStatement(Node* t);
extern ARG* GenArray(Node* t);
extern ARG* GenField(Node* t);

extern void ProcessChild(Node* t);

extern void processMid(Node* t);

extern void printMidCode(vector<Argnode>& vec);

extern void GenProcDec(Node*);
extern void GenCode(vector<Argnode>& vec, string codekind, ARG* arg1, ARG* arg2, ARG* arg3);
extern void GenDeclare(Node* t);

extern void GenBody(Node* t,string);
