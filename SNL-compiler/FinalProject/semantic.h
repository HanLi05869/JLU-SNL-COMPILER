#pragma once
#include<string>
#include<string.h>
#include<cstring>
#include"global.h"
using namespace std;
// 语义分析
class SymbolTable;
extern string bstr[160];
extern int bindex;
extern int procedure[160];
struct SymbolBaseNode;
struct SymbolRecNode;
struct SymbolRec;
extern vector<SymbolBaseNode*> SBase;
extern SymbolTable* smbltable;
extern void findproc();
extern void typeSaveTable(TreeNode* t, SymbolTable* st);
extern void varSaveTable(TreeNode* t, SymbolTable* st);
extern void procSaveTable(TreeNode* t, SymbolTable* st, SymbolTable* nextst);
extern void checkParam(TreeNode* t, string bstr[], int& bindex);
extern void checkProgramBodyAndIO(TreeNode* t, SymbolTable* st);
extern void checkAssignAndDeclaration(TreeNode* t);
extern void checkAssignLeftRight(TreeNode* t);
extern void checkRange();
extern void Check_STEP3(TreeNode* treeroot, SymbolTable* smbltable);
// 符号表的记录节点
struct SymbolRecord
{
    string name; // 名字
    string kind; // 三种类型
    string type; // 具体类型
    int dir; //访问方式
    Token* tk; // 指向tokenlist
    SymbolTable* next;
};

class SymbolTable
{
public:
    SymbolTable()
    {
        index = 0;
        paramcount = 0;
    }
    SymbolRecord* table[5120];
    int index;
    int paramcount;
    SymbolTable* pre;
    void addRecord(string name, string kind, string type, Token* tk, SymbolTable* next = NULL, int dir = 1)
    {
        // cout <<"\t" << name << "\t" << kind << endl;
        table[index] = new SymbolRecord();
        // char temp[100];

        table[index]->name = name;
        table[index]->kind = kind;
        table[index]->type = type;
        table[index]->tk = tk;
        table[index]->dir = dir;
        table[index]->next = next;
        if (next) {
            next->pre = this;
            // cout << table[index]->name << endl;
        }
        index++;
        //cout << "ok|" <<index<< endl;
    }
    void printTable(int layer = 0)
    {
        if(layer==0) cout << "============================= SymbolTable =============================" << endl;
        string outputstr;
        for (int i = 0; i < index; i++)
        {
            for (int i = 0; i < layer; i++)
            {
                cout << "\t";
                outputstr += "\t";
            }

            cout << table[i]->name
                << "\t"
                << table[i]->kind
                << "\t"
                << table[i]->type << endl;

            outputstr += table[i]->name;
            outputstr += "\t";
            outputstr += table[i]->kind;
            outputstr += "\t";
            outputstr += table[i]->type;
            outputstr += "\n";

            if (table[i]->next)
            {
                //cout << endl;
                //outputstr += "\n";
                table[i]->next->printTable(layer + 1);
                //cout << endl;
                //outputstr += "\n";
            }
        }
        // if (layer == 0) cout << "No Error Found!!！" << endl;
    }
};

struct SymbolBaseNode{
    string name; // 名字
    string btype; // 具体类型
    SymbolBaseNode(string n, string bt) {
        name = n;
        btype = bt;
    }
};

struct SymbolRecNode {
    string name; // 名字
    string type; // 具体类型
    string btype;
    SymbolRecNode(string n, string t, string bt = "") {
        name = n;
        type = t;
        btype = bt;
    }
};

struct SymbolRec {
    string RName;
    vector<SymbolRecNode*> SRec;
    SymbolRec(string rn) {
        RName = rn;
    }
};

extern int is_correct_semantic;
