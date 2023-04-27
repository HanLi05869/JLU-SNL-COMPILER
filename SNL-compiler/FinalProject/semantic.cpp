#include "func.h"
#include "global.h"
#include "globalVar.h"
#include "newLL1.h"
#include <iostream>
#include"semantic.h"
using namespace std;

SymbolTable* smbltable = new SymbolTable();
vector<SymbolBaseNode*> SBase;  //array
vector<SymbolRec*> SRec;       //record
string bstr[160];
int bindex = 0;     //:=
TreeNode* root;
int is_correct_semantic = 1;
int procedure[160];
int indexp = 0;
int beginp[16];
int endp[16];
string pname[16];

void findtype(string& temptyp,SymbolTable*STptr) {  //typedef

    while (temptyp != "char" && temptyp != "array" && temptyp != "integer" && temptyp != "record") {
        for (int i = 0; i < STptr->index; i++) {
            if (temptyp == STptr->table[i]->name)
                temptyp = STptr->table[i]->type;
        }
    }
}

string findBtype(SymbolRecord* t, bool flag) {
    if (flag) return t->type;
    else {
        string temptyp = t->type;
        string tempname = t->name;
        if (temptyp != "array") tempname = temptyp;
        if (temptyp != "char" && temptyp != "integer") {
            for (auto& s : SBase) {
                if (s->name == tempname)
                    return s->btype;
            }
        }
        else return temptyp;
    }
}

bool checkfor(string str) {
    return str != "end" && str != "endwh" && str != "fi" && str != "do" && str != ";" && str != "else";
}

void printErrorMsg(string str) {
    cout << str;
    is_correct_semantic = 0;
    //exit(0);
}

bool check_Repeat(SymbolTable*st,string name ) {
    for (int i = 0; i < st->index; i++)
    {
        if (name == st->table[i]->name)
        {
            return false;
        }
    }
    return true;
}

bool checkInvalid(TreeNode* t, int idx)
{
    if (idx < t->child.size())
        return true;
    else
        return false;
}

int checkRecord(string rstr,string estr) {
    for (auto& s : SRec)
        if (rstr == s->RName)
            for (auto& r : s->SRec)
                if (r->name == estr)
                    return 1;
    return 0;
}

void findproc() {
    int i0 = treeroot->child[2]->child[0]->tk->index;
    for (int i = 0; i < i0; i++)
    {
        if (tokenList[i].wd.str == "procedure")
        {
            procedure[indexp] = tokenList[i].index;
            pname[indexp] = tokenList[i].wd.str;
            for (int j = i; j < i0; j++)
            {
                if (tokenList[j].wd.str == "begin")
                {
                    beginp[indexp] = tokenList[j].index;
                    for (int k = j; k < i0; k++)
                    {
                        if (tokenList[k].wd.str == "end")
                        {
                            endp[indexp] = tokenList[k].index;
                            indexp++;
                            break;
                        }
                    }
                    // cout << indexp - 1 << "  " << beginp[indexp - 1] << "  " << endp[indexp - 1] << "  " << endl;
                    break;
                }
            }
        }
    }
}

void typeSave(TreeNode* t, SymbolTable* st) {
    string name = t->child[0]->child[0]->name;
    Token* tk = t->child[0]->child[0]->tk;

    string tempstr = t->child[2]->child[0]->name;
    string type;
    // cout << tempstr << endl;
    if (tempstr == "BaseType") {
        type = t->child[2]->child[0]->child[0]->tk->wd.str;
    }
    else if (tempstr != "BaseType" && tempstr != "StructureType") {
        type = t->child[2]->child[0]->tk->wd.str;
    }
    else {
        type = t->child[2]->child[0]->child[0]->child[0]->tk->wd.str;
        if (type == "array") {
            string sbase = t->child[2]->child[0]->child[0]->child.back()->child[0]->name;
            // cout <<name<<"  "<< sbase << endl;
            SBase.push_back(new SymbolBaseNode(name, sbase));
        }
    }
    string kind = "typeKind";
    //cout << "here" << endl;
    if(check_Repeat(st,name)) st->addRecord(name, kind, type, tk);
    else {
        string temp = "Line ";
        temp += to_string(tk->line);
        temp += " ";
        temp += "\"";
        temp += name;
        temp += "\" ";
        temp += "RepeatDeclaration Error!\n";
        printErrorMsg(temp);
    }
    if (!t->child.back()->child.empty())
        typeSave(t->child.back()->child[0], st);
}

void typeSaveTable(TreeNode* t, SymbolTable* st)
{
    if (t->child.empty()) {
        return;
    }
    else
    {
        typeSave(t->child[0]->child[1], st);
    }
}

void varSave(TreeNode* t, SymbolTable* st)
{
    string type;
    // cout << t->child[0]->child[0]->name << endl;
    int flag = 0;
    if (t->child[0]->child[0]->name == "BaseType")
    {
        type = t->child[0]->child[0]->child[0]->tk->wd.str;
    }
    else if (t->child[0]->child[0]->name == "StructureType")
    {
        type = t->child[0]->child[0]->child[0]->child[0]->tk->wd.str;
        if (t->child[0]->child[0]->child[0]->name == "ArrayType")
        {
            flag = 1;//array
        }
        else
        {
            flag = 2;//record
        }
    }
    else
    {
        type = t->child[0]->child[0]->tk->wd.str;
        int flagg = 0;
        SymbolTable* STptr=st;
        int max = STptr->index;
        for (int i = 0; i <= max; i++) {
            if (i == max) {
                if (STptr->pre) {
                    STptr = st->pre;
                    max = STptr->index;
                    i = -1;
                }
            }
            else if (type == STptr->table[i]->name && STptr->table[i]->kind == "typeKind") flagg = 1;
        }
        if (!flagg) {
            string temp = "Line ";
            temp += to_string(t->child[0]->child[0]->tk->line);
            temp += " ";
            temp += "\"";
            temp += type;
            temp += "\" ";
            temp += "NotType Error!\n";
            printErrorMsg(temp);
        }
    }

    TreeNode* p = t->child[1];
    while (p->name == "VarIDList")
    {
        //cout <<p->child[0]->name<<endl;
        string name = p->child[0]->name;
        Token* tk = p->child[0]->tk;
        string kind = "varKind ";

        if (check_Repeat(st, name)) st->addRecord(name, kind, type, tk);
        else {
            string temp = "Line ";
            temp += to_string(tk->line);
            temp += " ";
            temp += "\"";
            temp += name;
            temp += "\" ";
            temp += "RepeatDeclaration Error!\n";
            printErrorMsg(temp);
        }

        if (flag)
        {
            if (flag == 1)SBase.push_back(new SymbolBaseNode(name,
                t->child[0]->child[0]->child[0]->child.back()->child[0]->tk->wd.str));
            else
            {
                SymbolRec* SR = new SymbolRec(name);
                TreeNode* sr = t->child[0]->child[0]->child[0]->child[1];
                string basetyp;
                while (!sr->child.empty()) {
                    type = sr->child[0]->child[0]->tk->wd.str;
                    if (sr->child[0]->name == "BaseType")
                    {
                        flag = 0;
                    }
                    else if (sr->child[0]->name == "ArrayType")
                    {
                        flag = 1;
                        basetyp = sr->child[0]->child.back()->child[0]->name;
                    }
                    auto ssr = sr->child[1];
                    while (!ssr->child.empty())
                    {
                        if (ssr->name == "IDMore") ssr = ssr->child[1];
                        if (!flag)
                        {
                            SR->SRec.push_back(new SymbolRecNode(ssr->child[0]->name, type));
                        }
                        else SR->SRec.push_back(new SymbolRecNode(ssr->child[0]->name, type, basetyp));
                        ssr = ssr->child[1];
                    }
                    if (!sr->child.back()->child.empty()) sr = sr->child.back()->child[0];
                    else break;
                }

                SRec.push_back(SR);
            }
        }

        if (p->child[1]->child.empty())
            break;
        else
            p = p->child[1]->child[1];

    }
    if (!t->child.back()->child.empty())
            varSave(t->child.back()->child[0], st);

}

void varSaveTable(TreeNode* t, SymbolTable* st)
{
    if (t->child.empty()) {
        //cout << "varSaveTable_exit" << endl;
        return;
    }
    else
    {

        varSave(t->child[0]->child[1], st);
    }
    //cout << "varSaveTable_exit" << endl;
}

void procSaveparam(TreeNode* t, SymbolTable* st, SymbolTable* nextst) {
    TreeNode* temp = t;
    string name;
    Token* tk;
    string kind;
    int dir = 1;
    string type;
    int idx = 0;
    if (temp->child[0]->child[0]->name == "var") {
        idx = 1;
        dir = 0;
    }
    if (temp->child[0]->child[idx]->child[0]->name == "BaseType")
        type = temp->child[0]->child[idx]->child[0]->child[0]->tk->wd.str;
    else if (temp->child[0]->child[idx]->child[0]->name == "StructureType") {
        type = temp->child[0]->child[idx]->child[0]->child[0]->child[0]->tk->wd.str;
        SBase.push_back(new SymbolBaseNode(t->child[0]->child[idx+1]->child[0]->name,
            t->child[0]->child[0]->child[0]->child.back()->child[0]->tk->wd.str));
    }
    else type = temp->child[0]->child[idx]->child[0]->tk->wd.str;
    kind = "varKind ";
    TreeNode* g = temp->child[0]->child[idx+1];
    name = g->child[0]->name;
    tk = g->child[0]->tk;

    if (check_Repeat(nextst, name)) {
        nextst->addRecord(name, kind, type, tk, NULL, dir);
        nextst->paramcount++;
    }
    else {
        string temp = "Line ";
        temp += to_string(tk->line);
        temp += " ";
        temp += "\"";
        temp += name;
        temp += "\" ";
        temp += "RepeatDeclaration Error!\n";
        printErrorMsg(temp);
    }
    while (!g->child[1]->child.empty()) {
        g = g->child[1]->child[1];
        name = g->child[0]->name;
        tk = g->child[0]->tk;
        if (check_Repeat(nextst, name)) {
            nextst->addRecord(name, kind, type, tk);
            nextst->paramcount++;
        }
        else {
            string temp = "Line ";
            temp += to_string(tk->line);
            temp += " ";
            temp += "\"";
            temp += name;
            temp += "\" ";
            temp += "RepeatDeclaration Error!\n";
            printErrorMsg(temp);
        }
    }
}

void procSaveTable(TreeNode* t, SymbolTable* st, SymbolTable* nextst)
{
    if (t->child.empty()) {
        // cout << "procSaveTable_exit" << endl;
        return;
    }
    else
    {
        TreeNode* h = t->child[0];
        nextst = new SymbolTable();//新的表
        string name = h->child[1]->child[0]->name;
        Token* tk = h->child[1]->child[0]->tk;
        string kind = "procKind";
        string type = h->child[0]->tk->wd.str;

        if (check_Repeat(st, name)) st->addRecord(name, kind, type, tk, nextst);
        else {
            string temp = "Line ";
            temp += to_string(tk->line);
            temp += " ";
            temp += "\"";
            temp += name;
            temp += "\" ";
            temp += "RepeatDeclaration Error!\n";
            printErrorMsg(temp);
        }

        TreeNode* temp = h->child[3];

        if (!temp->child.empty()) {
            temp = temp->child[0];
            procSaveparam(temp, st, nextst);
            temp = temp->child[1];//parammore

            while (temp->child.size())
            {
                temp = temp->child[1];
                procSaveparam(temp, st, nextst);
                temp = temp->child.back();
            }
        }
        h = h->child[6]->child[0];
        for (int i = 0; i < h->child.size(); i++)
        {
            TreeNode* temp = h->child[i];
            if (temp->name == "TypeDec")
                typeSaveTable(temp, nextst);
            else if (temp->name == "VarDec")
                varSaveTable(temp, nextst);
            else if (temp->name == "ProcDec")
            {
                SymbolTable* p = NULL;
                procSaveTable(temp, nextst, p);
            }
        }
        if (!t->child[0]->child.back()->child.empty())
            procSaveTable(t->child[0]->child.back(), st, nextst);
    }
}

void checkParam(int i,SymbolTable*ST,string bstr[],int bcon[], int& bindex)//检查参数类型和个数
{
    if (i >= Index)
        return;
    int j = i;
    SymbolTable* STptr = ST;
    int max = ST->index;
    while (tokenList[j].wd.tok != RPAREN)
    {
        if (tokenList[j].wd.tok == ID)
        {
            if (tokenList[j + 1].wd.tok != DOT) {
                for (int k = 0; k <= max; k++)
                {
                    if (k == max)
                    {
                        if(STptr->pre)
                        {
                            STptr = STptr->pre;
                            max = STptr->index;
                            k = -1;
                        }
                    }
                    else if (tokenList[j].wd.str == STptr->table[k]->name)
                    {
                        bstr[bindex] = STptr->table[k]->type;
                        findtype(bstr[bindex],STptr);
                        bcon[bindex] = 0;
                        bindex++;
                        break;
                    }
                }

            }
            else
            {
                int flag = 0;
                for (auto& s : SRec)
                {
                    if (s->RName == tokenList[j + 1].wd.str)
                    {
                        for (auto& ss : s->SRec)
                        {
                            if (ss->name == tokenList[j + 2].wd.str)
                            {
                                bstr[bindex] = ss->type;
                                bcon[bindex] = 0;
                                bindex++;
                                flag = 1;
                                break;
                            }
                        }
                    }
                    if (flag)
                    {
                        j += 2; break;
                    }
                }
            }

        }
        else {
            if (tokenList[j].wd.tok == INTC) {
                bstr[bindex] = "integer";
                bcon[bindex++] = 1;
            }
            else if (tokenList[j].wd.tok == CHARC) {
                bstr[bindex] = "char";
                bcon[bindex++] = 1;
            }
        }
        j++;
    }
}

void checkProgramBodyAndIO(TreeNode* t, SymbolTable* st)
{
    if (!t)
        return;

    SymbolTable* STptr = st;
    int max = st->index;

    for (int i = 0; i < Index; i++)
    {
        if (tokenList[i].wd.tok == LPAREN)
        {
            string temp = tokenList[i - 1].wd.str;
            int flag = 0;
            max = st->index;
            STptr = st;
            for (int j = 0; j < max; j++)
            {
                if (temp == STptr->table[j]->name && STptr->table[j]->kind == "procKind")
                {
                    flag = 1;
                    break;
                }
                else if (j == max-1 && STptr->table[j]->next) {
                    STptr = STptr->table[j]->next;
                    max = STptr->index;
                    j = -1;
                }
            }
            if (flag == 0 && temp != "read" && temp != "write" && temp != ":=" && temp != "<")
            {
                string temp = "Line ";
                temp += to_string(tokenList[i - 1].line);
                temp += " ";
                temp += "\"";
                temp += tokenList[i - 1].wd.str;
                temp += "\" ";
                temp += "ProcNotFound Error\n";
                printErrorMsg(temp);
            }
        }
    }

    TreeNode* p0 = t->child[2]->child[0];
    TreeNode* p1 = t->child[3];
    int i0 = p0->tk->index;
    int i1 = p1->tk->index;
    int pnum = 0;
    for (int i = i0; i <= i1; i++)
    {
        // cout << i << endl;
        if (tokenList[i].wd.tok == LPAREN) {
            if (tokenList[i - 1].wd.tok == ID) {
                for (int j = 0; j <= max; j++)
                {
                    if (j == max) {
                        if (STptr->pre) {
                            STptr = STptr->pre;
                            max = STptr->index;
                            j = -1;
                        }
                    }
                    else if (tokenList[i - 1].wd.str == STptr->table[j]->name && STptr->table[j]->kind == "procKind")
                    {
                        //cout << "\t" << tokenList[i - 1].wd.str << "\t" << to_string(tokenList[i].line) << endl;
                        string astr[16]; //astr 存的是正确情况下的参数类型列表 bstr是实际的参数类型列表
                        int adir[16];
                        int aindex = 0;

                        // astr的初始化很简单 因为已经有符号表了所以记录一下就行
                        for (int k = 0; k < STptr->table[j]->next->paramcount; k++)
                        {
                            astr[aindex] = (STptr->table[j]->next)->table[k]->type;
                            adir[aindex]= (STptr->table[j]->next)->table[k]->dir;
                            findtype(astr[aindex],STptr);
                            // cout << astr[aindex] << endl;
                            aindex++;
                        }
                        string bstr[16]; //astr 存的是正确情况下的参数类型列表 bstr是实际的参数类型列表
                        int bcon[16];
                        int bindex = 0;
                        checkParam(i + 1, STptr, bstr, bcon, bindex);
                        if (aindex != bindex) // 如果参数个数不相等 那就直接报错
                        {
                            string temp = "Line ";
                            temp += to_string(tokenList[i].line);
                            temp += " ";
                            temp += "\"";
                            temp += tokenList[i - 1].wd.str;
                            temp += "\" ";
                            temp += "ParamNum Error!\n";
                            printErrorMsg(temp);
                        }
                        else for (int k = 0; k < aindex; k++)
                        {
                            if (astr[k] != bstr[k]) {
                                string temp = "Line ";
                                temp += to_string(tokenList[i].line);
                                temp += " ";
                                temp += "\"";
                                temp += tokenList[i - 1].wd.str;
                                temp += "\" ";
                                temp += "ParamType Error!\n";
                                printErrorMsg(temp);
                            }
                            else if (adir[k] == 0 && bcon[k] == 1) {
                                string temp = "Line ";
                                temp += to_string(tokenList[i].line);
                                temp += " ";
                                temp += "\"";
                                temp += tokenList[i - 1].wd.str;
                                temp += "\" ";
                                temp += "ParamDir NotSame Error!\n";
                                printErrorMsg(temp);
                            }
                        }
                    }
                }
            }
            else if (tokenList[i - 1].wd.tok==READ|| tokenList[i - 1].wd.tok == WRITE) {
                string tempstr = tokenList[i + 1].wd.str;
                int flag = 0;
                for (int k = 0; k <= max; k++)
                {
                    if (k == max) {
                        if (STptr->pre) {
                            STptr = STptr->pre;
                            max = STptr->index;
                            k = -1;
                        }
                    }
                    else if (tokenList[i + 2].wd.tok == DOT)
                        flag=checkRecord(tokenList[i + 1].wd.str, tempstr);
                    else if (tempstr == STptr->table[k]->name && STptr->table[k]->kind == "varKind ")
                    {
                        flag = 1;
                        break;
                    }
                }
                if (!flag) {
                    string temp = "Line ";
                    temp += to_string(tokenList[i - 1].line);
                    temp += " ";
                    temp += "\"";
                    temp += tempstr;
                    temp += "\" ";
                    temp += "IO NotVar Error!\n";
                    printErrorMsg(temp);
                }
            }
        }
        if (i == i1&&pnum<indexp) {
            STptr = st;
            max = st->index;
            string procname = tokenList[procedure[pnum] + 1].wd.str;
            for (int j = 0; j < max; j++)
            {
                if (STptr->table[j]->name == procname)
                {
                    STptr = STptr->table[j]->next;
                    max = STptr->index;
                    break;
                }
                else if (j == max - 1 && STptr->table[j]->next) {
                    STptr = STptr->table[j]->next;
                    max = STptr->index;
                    j = -1;
                }
            }
            i = beginp[pnum] - 1;
            i1 = endp[pnum];
            pnum++;
        }
    }
}

void checkAssignAndDeclaration(TreeNode* t)// 这个函数的功能比较复杂 首先是检查赋值号左边的变量类型 还顺便看是否有未声明标识符
{
    TreeNode* p0 = t->child[2]->child[0];
    TreeNode* p1 = t->child[3];
    SymbolTable* p = smbltable;
    SymbolTable* STptr = smbltable;
    int max = STptr->index;
    int i0 = p0->tk->index;
    int i1 = p1->tk->index;
    int pnum = 0;
    for (int i = i0; i <= i1; i++)
    {
        if (tokenList[i].wd.tok == ASSIGN)
        {
            int u = i - 1;
            if (tokenList[u].wd.str == "]")
                while (tokenList[u--].wd.str != "[");
                string tempstr = tokenList[u].wd.str;
            int flag = 0;
            STptr = p;
            max = STptr->index;
            for (int j = 0; j <= max; j++)
            {
                if (j == max) {
                    if (STptr->pre) {
                        STptr = STptr->pre;
                        max = STptr->index;
                        j = -1;
                    }
                }
                else if (tempstr == STptr->table[j]->name && STptr->table[j]->kind == "varKind ")
                {
                    flag = 1;
                }
                else if (tokenList[u - 1].wd.tok == DOT) {
                    flag = checkRecord(tokenList[u - 2].wd.str, tempstr);
                }
            }
            if (flag == 0)
            {
                string temp = "Line ";
                temp += to_string(tokenList[i].line);
                temp += " ";
                temp += "\"";
                temp += tempstr;
                temp += "\" ";
                temp += "NotVarKind Error!\n";
                printErrorMsg(temp);
            }
        }
        if (tokenList[i].wd.tok == ID)
        {
            string tempstr = tokenList[i].wd.str;
            int flag = 0;
            STptr = p;
            max = STptr->index;
            for (int j = 0; j <= max; j++)
            {
                if (j == max) {
                    if (STptr->pre) {
                        STptr = STptr->pre;
                        max = STptr->index;
                        j = -1;
                    }
                }
                else if (tokenList[i + 1].wd.tok == DOT) {
                    flag = checkRecord(tempstr, tokenList[i + 2].wd.str);
                }
                else if (tempstr == STptr->table[j]->name && STptr->table[j]->kind != "typeKind ")
                {
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                string temp = "Line ";
                temp += to_string(tokenList[i].line);
                temp += " ";
                temp += "\"";
                temp += tempstr;
                temp += "\" ";
                temp += "NoDeclaration Error!\n";
                printErrorMsg(temp);
            }
        }
        if (i == i1 && pnum < indexp) {
            STptr = smbltable;
            max = STptr->index;
            string procname = tokenList[procedure[pnum] + 1].wd.str;
            for (int j = 0; j <= max; j++)
            {
                if (j == max) {
                    if (STptr->pre) {
                        STptr = STptr->pre;
                        max = STptr->index;
                        j = -1;
                    }
                }
                else if (STptr->table[j]->name == procname)
                {
                    p = STptr->table[j]->next;
                    max = p->index;
                    break;
                }
            }
            i = beginp[pnum] - 1;
            i1 = endp[pnum];
            pnum++;
        }

    }
}

void checkAssignLeftRight(TreeNode* t)// 检查赋值符号左右的类型的
{
    TreeNode* p0 = t->child[2]->child[0];
    TreeNode* p1 = t->child[3];
    int i0 = p0->tk->index;
    int i1 = p1->tk->index;
    SymbolTable* p = smbltable;
    SymbolTable* STptr=smbltable;
    int pnum = 0;
    int max=STptr->index;
    int flag = 0;

    for (int i = i0; i <= i1; i++)
    {
        string temptyp;
        if (tokenList[i].wd.tok == ASSIGN)
        {
            string tempstr = tokenList[i - 1].wd.str;
            int u = i - 1;
            bool SBflag = true;
            if (tempstr == "]") {
                SBflag = false;
                while (tokenList[u--].wd.str != "[");
                tempstr = tokenList[u].wd.str;
            }
            if (tokenList[u - 1].wd.str == ".") {
                for (auto& s : SRec) {
                    if (tokenList[u - 2].wd.str == s->RName) {
                        for (auto& r : s->SRec) {
                            if (r->name == tempstr) {
                                if (r->type == "integer") flag = 1;
                                else if (r->type == "array" && r->btype == "integer") flag = 1;
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                STptr = p;
                max = STptr->index;
                for (int j = 0; j <= max; j++)
                {
                    if (j == max) {
                        if (STptr->pre) {
                            STptr = STptr->pre;
                            max = STptr->index;
                            j = -1;
                        }
                    }
                    else {
                        if (tempstr == STptr->table[j]->name && STptr->table[j]->kind == "varKind ") {
                            temptyp = findBtype(STptr->table[j], SBflag);
                            findtype(temptyp, STptr);
                            if (temptyp == "integer") flag = 1;
                            break;
                        }
                    }
                }
            }
            if (flag == 0)
            {
                string temp = "Line ";
                temp += to_string(tokenList[i].line);
                temp += " ";
                temp += "\"";
                temp += ":=";
                temp += "\" ";
                temp += "Left NotSameType Error!\n";
                printErrorMsg(temp);
            }
            if (flag)
            {
                flag = 0;
                for (int k = i + 1; checkfor(tokenList[k].wd.str); k++)
                {
                    STptr = p;
                    max = STptr->index;
                    if (tokenList[k].wd.tok == ID)
                    {
                        bool SBflag = true;
                        if (tokenList[k + 1].wd.str == "[") SBflag = false;
                        int flag = 0;
                        for (int l = 0; l <= max; l++)
                        {
                            if (l == max) {
                                if (STptr->pre) {
                                    STptr = STptr->pre;
                                    max = STptr->index;
                                    l = -1;
                                }
                            }
                            else if (tokenList[k].wd.str == STptr->table[l]->name&& STptr->table[l]->kind=="varKind ") {
                                temptyp = findBtype(STptr->table[l], SBflag);
                                findtype(temptyp,STptr);
                                if (temptyp == "integer") {
                                    flag = 1;
                                }
                                else if (temptyp == "record") {
                                    for (auto& s : SRec) {
                                        if (tokenList[k].wd.str == s->RName) {
                                            for (auto& r : s->SRec) {
                                                if (r->name == tokenList[k + 2].wd.str) {
                                                    if (r->type == "integer") flag = 1;
                                                    else if (r->type == "array" && r->btype == "integer") flag = 1;
                                                    // cout << tempstr << endl;
                                                }
                                                if (flag) break;
                                            }
                                        }
                                        if (flag) break;
                                    }
                                    k = k + 2;
                                }
                            }
                        }
                        if (flag == 0)
                        {
                            // cout << 2 << endl;
                            string temp = "Line ";
                            temp += to_string(tokenList[i].line);
                            temp += " ";
                            temp += "\"";
                            temp += ":=";
                            temp += "\" ";
                            temp += "Right NotSameType Error!\n";
                            printErrorMsg(temp);
                        }
                    }

                }
            }
        }
        if (i == i1 && pnum < indexp) {
            STptr = smbltable;
            max = STptr->index;
            string procname = tokenList[procedure[pnum] + 1].wd.str;
            for (int j = 0; j < max; j++)
            {
                if (STptr->table[j]->name == procname)
                {
                    p = STptr->table[j]->next;
                    max = p->index;
                    break;
                }
                else if (j == max - 1 && STptr->table[j]->next) {
                    STptr = STptr->table[j]->next;
                    max = STptr->index;
                    j = -1;
                }
            }
            i = beginp[pnum] - 1;
            i1 = endp[pnum];
            pnum++;
        }
    }
}

void checkRange()//检查array越界的
{
    string type[16];
    string pname[16];
    int min[16];
    int max[16];
    int iarr = 0;

    for (int i = 0; i < Index; i++)
    {
        if (tokenList[i].wd.tok == PROGRAM || tokenList[i].wd.tok == PROCEDURE)
            pname[iarr] = tokenList[i + 1].wd.str;
        if (tokenList[i].wd.tok == UNDERANGE)
        {
            min[iarr] = atoi(tokenList[i - 1].wd.str.c_str());
            max[iarr] = atoi(tokenList[i + 1].wd.str.c_str());
            if (min[iarr] > max[iarr])
            {
                string temp = "Line ";
                temp += to_string(tokenList[i].line);
                temp += " ";
                temp += "\"";
                temp += tokenList[i].wd.str;
                temp += "\" ";
                temp += "UnderRange Error!\n";
                printErrorMsg(temp);
            }
            type[iarr] = tokenList[i + 5].wd.str;
            if (pname[iarr] == "")pname[iarr] = pname[iarr - 1];
            // cout <<pname[iarr]<<"||"<<type[iarr]<<"||" << min[iarr] << "||" << max[iarr] << endl;
            iarr++;
        }
    }
    int n=iarr;
    for (int i = 0; i < Index; i++)
    {
        if (tokenList[i].wd.tok == BEGIN) {
            while (n > 0 && pname[n] == pname[n - 1]) n--;
            if(n>0) n--;
        }
        if (tokenList[i].wd.str == "[" && tokenList[i + 1].wd.tok == INTC && tokenList[i + 2].wd.str == "]")
        {
            string temptype;
            int flag = 0;
            int ran = atoi(tokenList[i + 1].wd.str.c_str());
            for (int j = 0; j < smbltable->index; j++)
            {
                for (int s = 0; s < iarr;s++) {
                    if (tokenList[i - 1].wd.str == type[s] && pname[s]==pname[n]
                        &&ran<=max[s]&&ran>=min[s]) {
                        flag = 1;
                        break;
                    }
                }
            }
            if (flag == 0)
            {
                string temp = "Line ";
                temp += to_string(tokenList[i-1].line);
                temp += " ";
                temp += "\"";
                temp += tokenList[i-1].wd.str;
                temp += "\" ";
                temp += "OutRange Error!\n";
                printErrorMsg(temp);
            }
        }
    }
}


void Check_STEP3(TreeNode* treeroot, SymbolTable* smbltable)
{
    findproc();

    checkProgramBodyAndIO(treeroot, smbltable);

    checkAssignAndDeclaration(treeroot);

    checkAssignLeftRight(treeroot);

    checkRange();
}
