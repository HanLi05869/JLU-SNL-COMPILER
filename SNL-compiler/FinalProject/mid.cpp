#include "mid.h"
#include "func.h"
#include "newLL1.h"
#include "global.h"
#include "globalVar.h"
#include "semantic.h"

#include <iomanip>

using namespace std;

vector<Argnode> midtable;

vector<midprintnode> MidPrintNodeTable;

int tmpidx = 0;

ARG::ARG()
{

}

ARG::ARG(string _name) {
    this->name = _name;
}

ARG* NewTemp(int access) {
    auto t = new ARG();
    t->form = "AddrForm";
    t->dataLevel = -1;
    t->dataOff = tmpidx;
    t->access = access;
    string s = "temp";
    t->name += s;
    t->name += to_string(tmpidx);

    tmpidx += 1;
    return t;
}

ARG* ARGValue(int value) {
    auto t = new ARG();
    t->form = "AddrForm";
    // 写变量名暂时略
    // 层数也略
    // 偏移也略
    // 访问信息也略
    t->value = value;
    return t;
}

int NewLabel()
{
    // 编号值label += 1
    // 返回新的编号
    return 0;
}

ARG* ARGLabel(int label)
{
    auto t = new ARG();
    t->form = "LabelForm";
    t->label = label;
    return t;
}

ARG* ARGAddr(const char* id, int level, int off, int access)
{
    auto t = new ARG();
    t->form = "ValueForm";
    // 变量名信息没写(现在写了)
    t->name = *id;
    t->dataLevel = level;
    t->dataOff = off;
    t->access = access;
    return t;
}




string wacao = "";

vector<Node* > ArrayTable;
vector<Node*> FieldTable;
int level = 0;

unordered_map<int, vector<pair<int, Node*>>> mapArray;
unordered_map<int, vector<pair<int, Node*>>> mapRecord;

void processMid(Node * t)
{
    if (!t)
        return;
    auto t1 = t->child[0];// PheadK对应的节点，程序头
    auto t2 = t->child[1];// declare
    auto t3 = t->child[2];// 程序体
    /*if (t1) {
        cout << "t1: " << NodePrintInfo(t1) << endl;
    }
    if (t2) {
        cout << "t2: " << NodePrintInfo(t2) << "    " << NodePrintInfo(t2->sibling) << endl;
        if (t2->sibling->sibling) {
            cout << t2->sibling->sibling->nodekind << endl;
        }
    }
    if (t3) {
        cout << "t3: " << endl;
    }
    system("pause");
    cout << NodePrintInfo(t2) << endl;
    cout << NodePrintInfo(t3) << endl;
    system("pause");*/
    wacao = getBehind(t1);

    GenDeclare(t2);
    if (t3) {
        // GenCode(midtable, "ENTRY", new ARG(getBehind(t1)), nullptr, nullptr);
        GenBody(t3, getBehind(t1));
        // GenCode(midtable, "ENDPROC", new ARG(getBehind(t1)), nullptr, nullptr);
    }
}


void GenDeclare(Node* t)
{
    ++level;
    if (!t)
        return;

    auto tmp = t;
    int flg = 0;
    while (tmp)
    {
        if (tmp->nodekind == "ProcDecK") {

            flg = 1;
            break;
        }
        if (tmp->nodekind == "VarK")
        {
            auto tmp_child = tmp->child[0];
            while (tmp_child)
            {
                string type = string(tmp_child->kind.dec);

                if (type == "ArrayK") {
                    // 这里要做的事情是去记录这个数值
                    mapArray[level].push_back({ level, tmp_child });
                }
                if (type == "RecordK") {
                    mapRecord[level].push_back({level, tmp_child});
                }
                tmp_child = tmp_child->sibling;
            }

        }
        tmp = tmp->sibling;
    }
    if (flg) // 有函数声明
    {
        /*int idx = 0;
        for (int i = 0; i < 3; ++i) {
            auto miao = tmp->child[i];
            if (miao && miao->nodekind == "VarK") { // 声明
                idx = i;
                break;
            }
        }
        cout << NodePrintInfo(tmp) << endl;
        cout << NodePrintInfo(tmp->child[idx]) << endl;
        cout << NodePrintInfo(tmp->child[idx]->sibling) << endl;*/

        GenProcDec(tmp);
    }

    tmp = t;
    while (tmp)
    {
        if (tmp->nodekind == "StmLK") {

            // GenCode(midtable, "ENTRY", new ARG(wacao), nullptr, nullptr);
            GenBody(tmp, wacao);
            // GenCode(midtable, "ENDPROC", new ARG(wacao), nullptr, nullptr);
            break;
        }
        tmp = tmp->sibling;
    }
    return;
}

// 处理声明中的函数（即函数声明）
void GenProcDec(Node* ttt)
{
    /*if (t->nodekind == "StmLK")
    {
        GenCode(midtable, "PENTRY", nullptr, nullptr, nullptr);
        int last = 2;
        for (last = 2; last >= 0; --last) {
            if (t->child[last])
                break;
        }
        for (int i = 0; i <= last; i++)
        {
            ProcessChild(t->child[i]);
        }
        if(t->sibling)
            ProcessChild(t->sibling);
        GenCode(midtable, "ENDPROC", nullptr, nullptr, nullptr);
    }*/

    Node* t = ttt;

    while (t)
    {
        if (!t)
            return;
        string f_name = getBehind(t); // 得到函数的名字

        int p_flg = 0;

        // 先处理声明，这边递归处理声明
        if (t->nodekind == "ProcDecK") { // 声明中嵌套了函数声明
            // GenCode(midtable, "ENTRY", new ARG(f_name), nullptr, nullptr);
            p_flg = 1;
        }
        auto t1 = t->child[0];// 形参
        auto t2 = t->child[1];// 所有声明
        auto t3 = t->child[2];// 过程体
        // 正常情况是上面右边注释的那样，但实际上不是所有的函数都会有这三个的


        /*system("pause");
        if (t1) {
            cout << "t1" << endl;
            cout << t1->nodekind << endl;
        }
        if (t2) {
            cout << "t2" << endl;
            cout << t2->nodekind << endl;
        }
        if (t3) {
            cout << "t3" << endl;
            cout << t3->nodekind << endl;
        }
        cout << "end" << endl;
        system("pause");*/

        // 一共四种情况
        // 无参数 无声明
        if (!t1 && t2 && t2->nodekind == "StmLK") {
            // 直接处理body
            GenBody(t2, f_name);
        }
        // 有参数 无声明
        else if (t1 && t1->nodekind == "DecK" && !t2 && t3 && t3->nodekind == "StmLK") {
            // t1是参数部分 t3是body部分
            // 直接处理body
            GenBody(t3, f_name);
        }
        // 无参数 有声明
        else if (t1 && t2 && t2->nodekind == "StmLK" && !t3) {
            // t1是声明
            GenDeclare(t1->child[0]);
            // t2是body
            GenBody(t2, f_name); // 处理body
        }
        // 有参数 有声明
        else if (t1 && t1->nodekind == "DecK" && t2 && t3 && t3->nodekind == "StmLK") {
            // cout << NodePrintInfo(t2) << endl;
            GenDeclare(t2->child[0]);
            GenDeclare(t2); // 加这个是因为如果在"有参数有声明"的情况下, t2的sibling有可能是会有另外的函数申明在
            // GenDeclare(t2->child[1]);
            GenBody(t3, f_name);
        }

        if (p_flg) {
            // 之前创建过函数的入口
            // GenCode(midtable, "ENDPROC", new ARG(f_name), nullptr, nullptr);
        }
        t = t->sibling; // 遍历剩下的函数声明
    }

}

// 处理函数body部分
void GenBody(Node* t)
{
    if (!t) {
        return;
    }

    // 指向第一个语句
    Node* t_sib = t->child[0];
    // 循环处理每一个body里的语句
    while (t_sib)
    {
        GenStatement(t_sib);
        t_sib = t_sib->sibling;
    }

    --level;
}

void GenBody(Node* t, string f_name)
{
    if (!t) {
        return;
    }

    // 指向第一个语句
    Node* t_sib = t->child[0];
    // 循环处理每一个body里的语句
    GenCode(midtable, "ENTRY", new ARG(f_name), nullptr, nullptr);
    while (t_sib)
    {
        GenStatement(t_sib);
        t_sib = t_sib->sibling;
    }
    GenCode(midtable, "ENDPROC", new ARG(f_name), nullptr, nullptr);
    --level;
}

string getBehind(Node* t) {
    string strs = "";

    string str = t->nodekind;
    if (str == "ExpK")
    {
        string tmp = string(t->kind.exp);
        if (tmp == "ConstK") {
            strs += to_string(t->attr.ExpAttr.val);
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (t->name[i] != "")
        {
            if (i != 0)
            {
                strs += "";
            }
            else
            {
                strs += "";
            }
            strs += t->name[i];
        }
    }
    return strs;
}

void GenStatement(Node* t)
{
    if (!t)
        return;
    // 一个语句
    if (t->nodekind == "StmtK")
    {
        string operation = t->kind.stmt;
        if (operation == "IfK")
        {
            GenIfS(t);
        }
        else if (operation == "WhileK")
        {
            GenWhileS(t);
        }
        else if (operation == "AssignK")
        {
            GenAssignS(t);
        }
        else if (operation == "ReadK")
        {
            GenReadS(t);
        }
        else if (operation == "WriteK")
        {
            GenWriteS(t);
        }
        else if (operation == "CallK")
        {
            GenCallS(t);
        }
        else if (operation == "ReturnK")
        {
            GenCode(midtable, "RETURN", nullptr, nullptr, nullptr);
        }
        else
        {
            cout << "Error Match\n" << endl;
            system("pause");
        }
    }
}

ARG* GenField(Node* t)
{
    if (!t)
        return nullptr;

    // 找到声明的地方
    /*string arr_name = getBehind(t);
    Node* tt = nullptr;
    for (int lev = level; lev >= 0; --lev)
    {
        for (int i = mapRecord[lev].size() - 1; i >= 0; --i)
        {
            auto node = mapRecord[lev][i].second;
            string node_name = getBehind(node);
            if (node_name == arr_name) {
                tt = node;
                break;
            }
        }
    }
    if (!tt) {
        cout << "this error " << endl;
        longjmp(jump_buffer, 1);
    }

    auto t1 = tt->child[0];
    if (t1)
        cout << "t1: " << NodePrintInfo(t1) << endl;
    auto tt1 = t1->sibling;
    if (tt1)
        cout << "tt1: " << NodePrintInfo(tt1) << endl;
        */

    auto t_1 = t->child[0];
    auto r = NewTemp(1);
    string bianlaing = "Off" + getBehind(t_1);
    GenCode(midtable, "[+]", new ARG(getBehind(t)), new ARG(bianlaing), r);
    return r;

}

ARG* processQiantao(Node* t)
{
    if (!t)
        return nullptr;

    string exp = t->kind.exp;
    /*cout << "刚刚进入: " << " " << getBehind(t) << endl;
    cout << test << " " << exp << " " << (exp == "VariK") <<" " << (exp == "ConstK") << endl;
    system("pause");
    */
    // 暂时只考虑这两个情况，变量或者常树时
    if (exp == "VariK" || exp == "ConstK")
    {
        // cout << "进入函数" << endl;
        string yifangwanyi = t->attr.ExpAttr.varkind;
        if (exp == "VariK" && yifangwanyi == "ArrayMembV") {
            // 如果这个条件满足，说明是数组的开始，特殊处理
            auto m = GenArray(t);
            return m;
        }
        else if (exp == "VariK" && yifangwanyi == "FieldMembV") { // field
            return GenField(t);
        }
        else
        {
            // 说明到终点了，不用递归了
            return new ARG(getBehind(t));
        }
    }
    else
    {
        // 如这里是OpK的话
        // 先去处理两个操作分量
        auto t1 = processQiantao(t->child[0]);
        auto t2 = processQiantao(t->child[1]);
        // 得到具体的操作是什么
        int op = t->attr.ExpAttr.op;
        string str = "";
        if (op == LT)
            str += "< ";
        else if (op == EQ)
            str += "= ";
        else if (op == PLUS)
            str += "+ ";
        else if (op == MINUS)
            str += "- ";
        else if (op == TIMES)
            str += "* ";
        else if (op == OVER)
            str += "/ ";

        auto r = NewTemp(1); // 创建一个temp节点
        GenCode(midtable, str, t1, t2, r);
        return r; // 把创建的节点返回
    }
}


void GenAssignS(Node* t)
{
    if (!t)
        return;

    auto first = t->child[0];

    auto second = t->child[1];


    /*string strf = getBehind(first); // :=的左边 不会 是右值
    auto miao1 = new ARG(strf);*/

    auto miao1 = processQiantao(first);
    /*cout << "函数的左值: " << endl;
    cout << strf << endl;
    */
    // 等于号的右边要看情况的
    auto miao2 = processQiantao(second);
    /*cout << "函数的右边: " << endl;
    cout << miao2->name << endl;
    */
    // system("pause");

    // miao1 := miao2
    GenCode(midtable, "ASSIGN", miao2, nullptr, miao1);
}


// 这个还没实现，可以看processQiantao中调用它的部分
// 写这个前看一下a[1][2][3]这种数组在语法树里的格式长什么样
ARG* GenArray(Node* t) {

    ARG* val = processQiantao(t->child[0]); // 得到数组的val

    string arr_name = getBehind(t);
    Node* tt = nullptr;
    /*cout << level << " " << endl;
    system("pause");
    cout << mapArray[0].size() << endl;
    cout << mapArray[1].size() << endl;
    cout << mapArray[2].size() << endl;
    system("pause");*/

    for (int lev = level; lev >= 0; --lev)
    {
        for (int i = mapArray[lev].size() - 1; i >= 0; --i)
        {
            auto node = mapArray[lev][i].second;
            string node_name = getBehind(node);
            if (node_name == arr_name) {
                tt = node;
                break;
            }
        }
    }

    for (int i = 0; i < smbltable->index; i++)
    {
        auto s_name = smbltable->table[i]->name;
        auto s_kind = smbltable->table[i]->kind;
        auto s_type = smbltable->table[i]->type;
        if (smbltable->table[i]->next)
        {

        }
    }


    if (!tt) {
        cout << "error" << endl;
        longjmp(jump_buffer, 1);
        // exit(0);
    }


    int n_low = tt->attr.ArrayAttr.low; // 得到下标的最小数值
    int n_high = tt->attr.ArrayAttr.up;

    string s_low = to_string(n_low);
    ARG* t1 = NewTemp(1);
    GenCode(midtable, "-", val, new ARG(s_low), t1);
    string type = t->attr.ArrayAttr.childType;
    type += "Size";
    ARG* t2 = NewTemp(1);
    ARG* t3 = NewTemp(1);
    GenCode(midtable, "*", t1, new ARG(type), t2);
    GenCode(midtable, "[+]", new ARG(arr_name), t2, t3);


    // exit(0);
    return t3;
}


/* 6略了 （其实processQiantao 里面已近解决了）
// 包含GenVar	GenArray	GenField

*/

// 暂时好像没屌用    根据op计算返回的数值
ARG* Calculate(string op, ARG* left, ARG* right)
{
    return nullptr;
}

// 这个函数已经没屌用了，用的上面的processQiantao(Node* t)来替代
ARG* GenExpr(Node* t)
{
    if (!t)
        return nullptr;
    if (t->nodekind == "ExpK")
    {
        if (t->kind.exp == "OpK") // 节点类别操作符
        {
            auto left = GenExpr(t->child[0]);
            auto right = GenExpr(t->child[1]);
            auto tmp = NewTemp(1);


            string str = getBehind(t);

            tmp->value = Calculate(str, left, right)->value;

            // 生成
            GenCode(midtable, str, left, right, tmp);
            return tmp;
        }
        else if (t->kind.exp == "ConstK") // 常表达式类型
        {
            auto tmp = new ARG();
            tmp->form = "ValueForm";
            tmp->value = 2;
            return tmp;
        }
        else if (t->kind.exp == "IdK") // 变量类型
        {
            // 调用GenVar，但是还没实现
        }
    }
}


// 只处理了值参的情况
void GenCallS(Node* t)
{
    auto t_1 = t->child[0]; // 这里应该是有过程的名字
    string t_1_name = getBehind(t_1);

    auto tmp = t->child[1]; // 确定了是child[1]，就是说函数名字在child[0]里，但是传入的参数在child[1]中

    int offset = 0;
    while (tmp)
    {
        string na = getBehind(tmp);

        // 暂时先当数 参处理
        // ValACT, t1, offset1, Size1
        GenCode(midtable, "ValACT", new ARG(na), new ARG(to_string(offset)), new ARG(to_string(1)));
        offset += 1; // 偏移暂时先当1处理
        tmp = tmp->sibling;
    }
    GenCode(midtable, "CALL", new ARG(t_1_name), new ARG("true"), new ARG("NULL"));
}

// 简单处理
void GenReadS(Node* t)
{
    // cout << NodePrintInfo(t) << " " << (getBehind(t)) <<  endl;
    // system("pause");
    auto tmp = ARGAddr((getBehind(t)).data(), 0, 0, 1);
    GenCode(midtable, "READC", new ARG(getBehind(t)), nullptr, nullptr);
}

void GenWriteS(Node* t)
{
    // auto tmp = GenExpr(t->child[0]);
    auto tmp = processQiantao(t->child[0]);
    GenCode(midtable, "WRITEC", tmp, nullptr, nullptr);
}

// child还是sibiligg
void GenIfS(Node* t)
{
    if (!t)
        return;
    auto t_1 = t->child[0]; // if(A)的A的部分
    auto res = processQiantao(t_1);
    GenCode(midtable, "THEN", res, nullptr, nullptr);

    // if后else前的部分
    auto t_2 = t->child[1]; //盲猜有1
    auto tmp = t_2;
    while (tmp) {
        GenStatement(tmp);
        tmp = tmp->sibling;
    }
    // else后，这部分可能没有

    auto t_3 = t->child[2];

    if (t_3)
    {
        GenCode(midtable, "ELSE", nullptr, nullptr, nullptr);
        tmp = t_3;
        while (tmp) {
            GenStatement(tmp);
            tmp = tmp->sibling;
        }
    }
    GenCode(midtable, "ENDIF", nullptr, nullptr, nullptr);
}

// 同if
void GenWhileS(Node* t)
{
    GenCode(midtable, "WHILE", nullptr, nullptr, nullptr);
    auto t_1 = t->child[0];
    auto res = processQiantao(t_1);
    GenCode(midtable, "DO", res, nullptr, nullptr);

    // while() {A} A的部分
    auto t_2 = t->child[1]; //盲猜有1
    auto tmp = t_2;
    while (tmp)
    {
        GenStatement(tmp);
        tmp = tmp->sibling;
    }
    GenCode(midtable, "ENDWHILE", nullptr, nullptr, nullptr);
}



void printMidCode(vector<Argnode>& vec) {
    cout << "************************************** mid code **************************************" << endl;
    /*for (int i = 0; i < vec.size(); ++i){
        Argnode& s = vec[i];
        // cout << "( " << s.codekind << ", " << s.Arg1 << ", " << s.Arg2 << ", " << s.res << " )";
        cout << "( " << s.codekind << ", ";

        if (s.Arg1) {
            cout << s.Arg1->name << ", ";
        }
        else {
            cout << "-, " << endl;
        }

        if (s.Arg2) {
            cout << s.Arg2->name << ", ";
        }
        else {
            cout << "-, ";
        }

        if (s.res) {
            cout << s.res->name << " )";
        }
        else {
            cout << "- )";
        }

        cout << endl;
    }
    cout << endl;*/

    for (auto& s : MidPrintNodeTable) {
        cout << "( "
            << "\t" << std::left << setw(8)
            << s.op << " , "
            << "\t" << std::left << setw(8)
            << s.m1 << " , "
            << "\t" << std::left << setw(8)
            << s.m2 << " , "
            << "\t" << std::left << setw(8)
            << s.m3 << " ) " << endl;
    }
}

void GenCode(vector<Argnode>& vec, string codekind, ARG* arg1, ARG* arg2, ARG* arg3)
{
    /*Argnode* tmp = new Argnode;
    tmp->codekind = codekind;
    tmp->Arg1 = arg1;
    tmp->Arg2 = arg2;
    tmp->res = arg3;

    vec.push_back((* tmp));

    printMidCode(vec);
    system("pause");*/
    string op = codekind;
    string n1 = arg1 ? arg1->name : "-";
    string n2 = arg2 ? arg2->name : "-";
    string n3 = arg3 ? arg3->name : "-";
    // cout << "( " << codekind << " , " << n1 << " , " << n2 << " , " << n3 << " ) " << endl;
    MidPrintNodeTable.push_back({ op, n1, n2, n3 });
}
