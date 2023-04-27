#include "newLL1.h"
#include "LL1.h"

#include <iostream>
#include <tuple>
#include <sstream>

using namespace std;

SyntaxTree::SyntaxTree()
{
    root = nullptr;
}

SyntaxTree::~SyntaxTree()
{
    delete root;
}

void SyntaxTree::setRoot(Node* node)
{
    this->root = node;
}

Node* SyntaxTree::getRoot()
{
    return this->root;
}


Node* ParserLL1::newSpecNode(string kind)
{
    auto t = new Node(kind);
    int i;
    for (i = 0; i < 3; i++)
    {
        t->child[i] = nullptr;
    }
    t->sibling = nullptr;

    // t->lineno = ParserLL1::nowLine;
    t->lineno = currentToken->line;
    t->idnum = 0;
    for (i = 0; i < 10; i++)
    {
        t->table[i] = nullptr;
    }
    return t;
}

Node* ParserLL1::newStmtNode(string kind)
{
    // cout << "进入函数时候的参数" << kind << endl;
    Node* t = ParserLL1::newSpecNode("StmtK");
    string* tt = new string(kind);
    t->kind.stmt = (* tt).c_str();
    // cout << "函数后：" <<  t->kind.stmt << endl;
    return t;
}

Node* ParserLL1::newExpNode(string kind)
{
    auto t = ParserLL1::newSpecNode("ExpK");
    string* tt = new string(kind);
    t->kind.exp = (*tt).c_str();
    if ((*tt) == "OpK") {
        t->attr.ExpAttr.op = currentToken->wd.tok;
    }
    // cout << "函数里" << kind << " " << t->kind.exp << endl;
    return t;
}


SyntaxTree* ParserLL1::run()
{
    Index = 0; // readToken  reassgin 0 to it if reuse again
    NonTerminal stacktopN;
    Terminal stacktopT;

    initLL1(); // 初始化LL1分析表

    readToken();

    SyntaxTree* syntaxTree = new SyntaxTree();

    syntaxTree->setRoot(ParserLL1::newSpecNode("ProK"));

    symbal_stack.push({ 0, Program, "Program" });


    syntaxtree_stack.push(&syntaxTree->getRoot()->child[2]);
    syntaxtree_stack.push(&syntaxTree->getRoot()->child[1]);
    syntaxtree_stack.push(&syntaxTree->getRoot()->child[0]);


    while (!symbal_stack.empty())
    {
        auto TopElm = symbal_stack.top();

        if (get<0>(TopElm) == 1) // 终极符
        {
            stacktopT = (Terminal)get<1>(TopElm); // 记录一下终极符是什么
            // cout << "current token " << currentToken->wd.tok << endl;
            // system("pause");

            if (stacktopT == currentToken->wd.tok) // 比较一下 如果正确
            {

                symbal_stack.pop();
                readToken(); // 再往后读一个token
            }
            else
            {
                // 匹配失败，报错信息
                cout << "ERROR in line: " << " " << currentToken->line << endl;
                is_newLL1_correct = 0;

                longjmp(jump_buffer, 1); // 跳转回去
                readToken();
                // exit(0);
            }
        }
        else // 非终极符
        {

            stacktopN = (NonTerminal)get<1>(TopElm); // restore nonterminal
            // cuowu
            // cout << get<2>(TopElm) << endl;
            // system("pause");
            int num = LL1Tbl[stacktopN][currentToken->wd.tok]; // LL1

            symbal_stack.pop();

            process(num);


        }
    }
    return syntaxTree;
}

void display(Node* node)
{
    string str = node->nodekind;
    if (str == "DecK") {
        str += " ";
        str += string(node->kind.dec);
        str += " ";
    }
    else if (str == "StmtK") {
        str += " ";
        str += string(node->kind.stmt);
        str += " ";
    }
    else if (str == "ExpK") {
        str += " ";
        str += string(node->kind.exp);
        str += " ";
    }

    // 以下用于显示标识符
    for (int i = 0; i < 10; i++)
    {
        if (node->name[i] != "")
        {
            if (i != 0)
            {
                str += ",";
            }
            else
            {
                str += ":";
            }
            str += node->name[i];
        }
    }
    cout << str << endl;
}

void show(Node* root, int depth, vector<int>& v, bool islast) {
    if (!root) return;
    cout << endl;
    for (int i = 0; i < depth; i++) {
        if (v[i] == 1) cout << " │";
        else cout << "  ";
    }
    if (islast) {
        cout << " └  ";
        v[depth] = 0;
    }
    else cout << " ├  ";
    if (depth + 1 == (int)v.size())
        v.push_back(1);
    v[depth + 1] = 1;

    display(root);

    for (int i = 0; i < 3; i++) {
        if (i == 2)
            show(root->child[i], depth + 1, v, true);
        else
            show(root->child[i], depth + 1, v, false);
    }

    show(root->sibling, depth, v, false);
    return;
}

int ParserLL1::getPriosity(int op)
{
    int pri = 0;
    if (op == END1) {
        pri = -1;
    }
    else if (op == LPAREN) {
        pri = 0;
    }
    else if (op == LT || op == EQ) {
        pri = 1;
    }
    else if (op == PLUS || op == MINUS) {
        pri = 2;
    }
    else if (op == TIMES || op == OVER) {
        pri = 3;
    }
    else
        pri = -1;
    return pri;
}

void ParserLL1::process(int id)
{
    // cout << id << endl;
    switch (id)
    {
        case 1:
        {
            symbal_stack.push({ 1, DOT, "DOT" });
            symbal_stack.push({ 0, ProgramBody, "ProgramBody" });
            symbal_stack.push({ 0, DeclarePart, "DeclarePart" });
            symbal_stack.push({ 0, ProgramHead, "ProgramHead" });
            break;
        }
        case 2:
        {
            symbal_stack.push({ 0, ProgramName, "ProgramName" });
            symbal_stack.push({ 1, PROGRAM, "PROGRAM" });


            current_treenode = ParserLL1::newSpecNode("PheadK");
            auto item = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            (*item) = current_treenode;
            break;
        }
        case 3:
        {
            symbal_stack.push({ 1, ID, "ID" });

            //记录该单词的语义信息
            // current_treenode->name[0] = tokenList[head].getSem();
            current_treenode->name[0] = currentToken->wd.str;
            ++current_treenode->idnum;
            break;
        }
        case 4:
        {
            symbal_stack.push({ 0, ProcDec, "ProcDec" });
            symbal_stack.push({ 0, VarDec, "VarDec" });
            symbal_stack.push({ 0, TypeDec, "TypeDec" });
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            symbal_stack.push({ 0, TypeDeclaration, "TypeDeclaration" });
            break;
        }
        case 7:
        {
            symbal_stack.push({ 0, TypeDecList, "TypeDecList" });
            symbal_stack.push({ 1, TYPE, "TYPE" });

            auto t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newSpecNode("TypeK");

            (*t) = current_treenode;
            syntaxtree_stack.push(&(current_treenode->sibling));
            syntaxtree_stack.push(&(current_treenode->child[0]));

            break;
        }
        case 8:
        {
            symbal_stack.push({ 0, TypeDecMore, "TypeDecMore" });
            symbal_stack.push({ 1, SEMI, "SEMI" });
            symbal_stack.push({ 0, TypeName, "TypeName" });
            symbal_stack.push({ 1, EQ, "EQ" });
            symbal_stack.push({ 0, TypeId, "TypeID" });

            auto t = syntaxtree_stack.top();
            syntaxtree_stack.pop();

            current_treenode = ParserLL1::newSpecNode("DecK");
            (*t) = current_treenode;

            syntaxtree_stack.push(&(current_treenode->sibling));
            break;
        }
        case 9:
        {
            syntaxtree_stack.pop();
            break;
        }
        case 10:
        {
            symbal_stack.push({ 0, TypeDecList, "TypeDecList" });
            break;
        }
        case 11:
        {
            symbal_stack.push({ 1, ID, "ID" });
            // 记录该单词的语义信息
            // current_treenode->name[0] = tokenList[head].getSem();
            current_treenode->name[0] = currentToken->wd.str;
            current_treenode->idnum++;
            break;
        }
        case 12:
        {
            symbal_stack.push({ 0, BaseType, "BaseType" });
            temp = &(current_treenode->kind.dec);// no idea
            break;
        }
        case 13:
        {
            symbal_stack.push({ 0, StructureType, "StructureType" });
            break;
        }
        case 14:
        {
            symbal_stack.push({ 1, ID, "ID" });
            current_treenode->kind.dec = "IdK";
            // 记录该单词的语义信息
            // current_treenode->type_name = tokenList[head].getSem();
            current_treenode->type_name = currentToken->wd.str;
            break;
        }
        case 15:
        {
            symbal_stack.push({ 1, INTEGER, "INTEGER" });
            (*temp) = "IntegerK";
            break;
        }
        case 16:
        {
            symbal_stack.push({ 1, CHAR, "CHAR" });
            (*temp) = "CharK";
            break;
        }
        case 17:
        {
            symbal_stack.push({ 0, ArrayType, "ArrayType" });

            break;
        }
        case 18:
        {
            symbal_stack.push({ 0, RecType, "RecType" });
            break;
        }
        case 19:
        {

            symbal_stack.push({ 0, BaseType, "BaseType" });
            symbal_stack.push({ 1, OF, "OF" });
            symbal_stack.push({ 1, RMIDPAREN, "RMIDPAREN" });
            symbal_stack.push({ 0, Top, "Top" });
            symbal_stack.push({ 1, UNDERANGE, "UNDERANGE" });
            symbal_stack.push({ 0, Low, "Low" });
            symbal_stack.push({ 1, LMIDPAREN, "LMIDPAREN" });
            symbal_stack.push({ 1, ARRAY, "ARRAY" });

            // 感觉要炸这里
            current_treenode->kind.dec = "ArrayK";
            string tmp = current_treenode->attr.ArrayAttr.childType;
            const char* ttmp = tmp.data();
            temp = &ttmp;
            break;
        }
        case 20:
        {
            symbal_stack.push({ 1, INTC, "INTC" });
            // 记录该单词的语义信息
            // current_treenode->attr.ArrayAttr.low = tokenList[head].getSem().toInt();
            current_treenode->attr.ArrayAttr.low = atoi(currentToken->wd.str.c_str());
            break;
        }
        case 21:
        {
            symbal_stack.push({ 1, INTC, "INTC" });
            // 记录该单词的语义信息
            // current_treenode->attr.ArrayAttr.up = tokenList[head].getSem().toInt();
            current_treenode->attr.ArrayAttr.up = atoi(currentToken->wd.str.c_str());
            break;
        }
        case 22:
        {
            symbal_stack.push({ 1, END1, "END1" });
            symbal_stack.push({ 0, FieldDecList, "FieldDecList" });
            symbal_stack.push({ 1, RECORD, "RECORD" });
            current_treenode->kind.dec = "RecordK";
            saveP = current_treenode;
            syntaxtree_stack.push(&(current_treenode->child[0]));
            break;
        }
        case 23:
        {
            symbal_stack.push({ 0, FieldDecMore, "FieldDecMore" });
            symbal_stack.push({ 1, SEMI, "SEMI" });
            symbal_stack.push({ 0, IdList, "IDList" });
            symbal_stack.push({ 0, BaseType, "BaseType" });
            auto t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newSpecNode("DecK");
            temp = &current_treenode->kind.dec;
            (*t) = current_treenode;
            syntaxtree_stack.push(&current_treenode->sibling);
            break;
        }
        case 24:
        {
            symbal_stack.push({ 0, FieldDecMore, "FieldDecMore" });
            symbal_stack.push({ 1, SEMI, "SEMI" });
            symbal_stack.push({ 0, IdList, "IDList" });
            symbal_stack.push({ 0, ArrayType, "ArrayType" });

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newSpecNode("DecK"); // 生成记录类型的下一个域，不添任何信息
            (*t) = current_treenode; // 若是第一个，则是record类型的子结点指向当前结点，否则，是上一个纪录域声明的兄弟结点
            syntaxtree_stack.push(&((*current_treenode).sibling));

            break;
        }
        case 25:
        {
            // 后面没有记录类型的下一个域了，恢复当前纪录类型节点的指针
            syntaxtree_stack.pop();
            current_treenode = saveP;
            break;
        }

        case 26:
        {
            symbal_stack.push({ 0, FieldDecList, "FieldDecList" });
            break;
        }
        case 27:
        {
            symbal_stack.push({ 0, IdMore, "IDMore" });
            symbal_stack.push({ 1, ID, "ID" });

            // 纪录一个域中各个变量的语义信息
            // current_treenode->name[current_treenode->idnum] = tokenList[head].getSem();
            // current_treenode->idnum++;
            current_treenode->name[current_treenode->idnum] = currentToken->wd.str;
            current_treenode->idnum++;
            break;
        }
        case 28:
        {
            break;
        }

        case 29:
        {
            symbal_stack.push( { 0, IdList, "IDList" });
            symbal_stack.push({ 1, COMMA, "COMMA" });

            break;
        }
        case 30:
        {
            break;
        }

        case 31:
        {
            symbal_stack.push({ 0, VarDeclaration, "VarDeclaration" });

            break;
        }
        case 32:
        {
            symbal_stack.push({ 0, VarDecList, "VarDecList" });
            symbal_stack.push({ 1, VAR, "VAR" });

            current_treenode = ParserLL1::newSpecNode("VarK"); // 生成一个标志变量声明的节点
            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            (*t) = current_treenode;
            syntaxtree_stack.push(&((*current_treenode).sibling)); // 压入指向函数声明的指针
            syntaxtree_stack.push(&((*current_treenode).child[0])); // 压入指向第一个变量声明节点的指针

            break;
        }
        case 33:
        {
            symbal_stack.push({ 0, VarDecMore, "VarDecMore" });
            symbal_stack.push({ 1, SEMI, "SEMI" });
            symbal_stack.push({ 0, VarIdList, "VarIDList" });
            symbal_stack.push({ 0, TypeName, "TypeName" });

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newSpecNode("DecK"); // 建立一个新的声明节点，这里表示变量声明
            (*t) = current_treenode; // 若是第一个节点，则变量声明的头指针指向它，否则它是前一个变量声明的后继
            syntaxtree_stack.push(&((*current_treenode).sibling));

            break;
        }
        case 34:
        {
            syntaxtree_stack.pop();
            break;
        }

        case 35:
        {
            symbal_stack.push({0, VarDecList, "VarDecList"});
            break;
        }
        case 36:
        {
            symbal_stack.push({ 0, VarIdMore, "VarIDMore" });
            symbal_stack.push({ 1, ID, "ID" });
            // no idea
            // current_treenode->name[current_treenode->idnum] = tokenList[head].getSem();
            // current_treenode->idnum++;

            current_treenode->name[current_treenode->idnum] = currentToken->wd.str;
            current_treenode->idnum++;
            break;
        }
        case 37:
        {
            break;
        }

        case 38:
        {
            symbal_stack.push({ 0, VarIdList, "VarIDList" });
            symbal_stack.push({ 1, COMMA, "COMMA"});

            break;
        }
        case 39:
        {
            break;
        }

        case 40:
        {
            symbal_stack.push({ 0, ProcDeclaration, "ProcDeclaration" });

            break;
        }
        case 41:
        {
            symbal_stack.push({ 0, ProcDecMore, "ProcDecMore" });
            symbal_stack.push({ 0, ProcBody, "ProcBody" });
            symbal_stack.push({ 0, ProcDecPart, "ProcDecPart" });
            symbal_stack.push({ 1, SEMI, "SEMI" });
            symbal_stack.push({ 1, RPAREN, "RPAREN" });
            symbal_stack.push({ 0, ParamList, "ParamList" });
            symbal_stack.push({ 1, LPAREN, "LPAREN" });
            symbal_stack.push({ 0, ProcName, "ProcName" });
            symbal_stack.push({ 1, PROCEDURE, "PROCEDURE" });

            current_treenode = ParserLL1::newSpecNode("ProcDecK");
            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            (*t) = current_treenode;

            syntaxtree_stack.push(&(current_treenode->sibling));
            syntaxtree_stack.push(&(current_treenode->child[2])); // 语句序列
            syntaxtree_stack.push(&(current_treenode->child[1])); // 函数的声明部分
            syntaxtree_stack.push(&(current_treenode->child[0])); // 指向参数声明
            break;
        }

        case 42:
        {
            break;
        }

        case 43:
        {
            symbal_stack.push({0, ProcDeclaration, "ProcDeclaration"});
            break;
        }

        case 44:
        {
            symbal_stack.push({ 1, ID, "ID" });
            // no idea
            // current_treenode->name[0] = tokenList[head].getSem();
            // current_treenode->idnum++;
            current_treenode->name[0] = currentToken->wd.str;
            current_treenode->idnum++;
            break;
        }

        case 45:
        {
            // 形参部分为空，弹出指向形参的指针
            syntaxtree_stack.pop();
            break;
        }

        case 46:
        {
            symbal_stack.push({ 0, ParamDecList, "ParamDecList" });
            break;
        }
        case 47:
        {
            symbal_stack.push({ 0, ParamMore, "ParamMore" });
            symbal_stack.push({ 0, Param, "Param" });
            break;
        }
        case 48:
        {
            syntaxtree_stack.pop();
            break;
        }

        case 49:
        {
            symbal_stack.push({ 0, ParamDecList, "ParamDecList" });
            symbal_stack.push({ 1, SEMI, "SEMI" });
            break;
        }
        case 50:
        {
            symbal_stack.push({ 0, FormList, "FormList" });
            symbal_stack.push({ 0, TypeName, "TypeName" });

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newSpecNode("DecK");
            // 函数的参数类型是值类型
            current_treenode->attr.ProcAttr.paramt = "valparamType";
            (*t) = current_treenode;
            syntaxtree_stack.push(&(current_treenode->sibling));

            break;
        }
        case 51:
        {
            symbal_stack.push({ 0, FormList, "FormList" });
            symbal_stack.push({ 0, TypeName, "TypeName" });
            symbal_stack.push({ 1, VAR, "VAR" });

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newSpecNode("DecK");
            // 函数的参数类型是变量类型
            current_treenode->attr.ProcAttr.paramt = "varparamType";
            (*t) = current_treenode;
            syntaxtree_stack.push(&(current_treenode->sibling));

            break;
        }
        case 52:
        {
            symbal_stack.push({ 0, FidMore, "FidMore" });
            symbal_stack.push({ 1, ID, "ID" });
            // no idea
            // current_treenode->name[current_treenode->idnum] = tokenList[head].getSem();
            // current_treenode->idnum++;
            current_treenode->name[current_treenode->idnum] = currentToken->wd.str;
            current_treenode->idnum++;

            break;
        }
        case 53:
        {
            break;
        }

        case 54:
        {
            symbal_stack.push({ 0, FormList, "FormList" });
            symbal_stack.push({ 1, COMMA, "COMMA" });
            break;
        }

        case 55:
        {
            symbal_stack.push({ 0, DeclarePart, "DeclarePart" });
            break;
        }
        case 56:
        {
            symbal_stack.push({ 0, ProgramBody, "ProgramBody" });
            break;
        }

        case 57:
        {
            symbal_stack.push({ 1, END1, "END1" });
            symbal_stack.push({ 0, StmList, "StmList" });
            symbal_stack.push({ 1, BEGIN, "BEGIN" });

            /*注意，若没有声明部分，则弹出的是程序或过程根节点中指向
              声明部分的指针child[1];若有声明部分，则弹出的是语句序列前
              的最后一个声明标识节点的兄弟指针；不管是哪种情况，都正好
              需要弹出语法树栈中的一个指针*/
            syntaxtree_stack.pop();

            /*建立语句序列标识节点*/
            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newSpecNode("StmLK");
            (*t) = current_treenode;
            syntaxtree_stack.push(&(current_treenode->child[0]));
            break;
        }
        case 58:
        {
            symbal_stack.push({ 0, StmMore, "StmMore" });
            symbal_stack.push({ 0, Stm, "Stm" });
            break;
        }
        case 59:
        {
            syntaxtree_stack.pop();
            break;
        }

        case 60:
        {
            symbal_stack.push({ 0, StmList, "StmList" });
            symbal_stack.push({ 1, SEMI, "SEMI" });
            break;
        }
        case 61:
        {
            symbal_stack.push({ 0, ConditionalStm, "ConditionalStm" });

            current_treenode = ParserLL1::newStmtNode("IfK");
            //current_treenode->kind.stmt=;

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            (*t) = current_treenode;
            syntaxtree_stack.push(&current_treenode->sibling);

            break;
        }
        case 62:
        {
            symbal_stack.push({0, LoopStm, "LoopStm"});

            current_treenode = ParserLL1::newStmtNode("WhileK");
            //current_treenode->kind.stmt=;

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            (*t) = current_treenode;
            syntaxtree_stack.push(&current_treenode->sibling);

            break;
        }

        case 63:
        {
            symbal_stack.push({ 0, InputStm, "InputStm" });

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newStmtNode("ReadK");
            //current_treenode->kind.stmt=;
            (*t) = current_treenode;
            syntaxtree_stack.push(&current_treenode->sibling);

            break;
        }
        case 64:
        {
            symbal_stack.push({ 0, OutputStm, "OutputStm" });

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newStmtNode("WriteK");
            (*t) = current_treenode;
            syntaxtree_stack.push(&current_treenode->sibling);

            break;
        }
        case 65:
        {
            symbal_stack.push({ 0, ReturnStm, "ReturnStm" });

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            current_treenode = ParserLL1::newStmtNode("ReturnK");
            (*t) = current_treenode;
            syntaxtree_stack.push(&current_treenode->sibling);

            break;
        }

        case 66:
        {
            symbal_stack.push({0, AssCall, "AssCall"});
            symbal_stack.push({ 1, ID, "ID" });

            current_treenode = ParserLL1::newStmtNode("AssignK");
            // current_treenode->kind.stmt = "AssignK";

            // cout << "current_treenode: " << current_treenode->nodekind << " " << current_treenode->kind.stmt << " over " << endl;
            /*赋值语句左部变量节点*/
            Node* t = ParserLL1::newExpNode("VariK");

            // t->kind.exp = "VariK";

            // cout << "miao 66: " << t->kind.exp << endl;
            // system("pause");

            // no idea
            // t->name[0] = tokenList[head].getSem();
            // t->idnum++;
            t->name[0] = currentToken->wd.str;
            t->idnum++;
            /*赋值语句的child[0]指向左部的变量节点*/
            current_treenode->child[0] = t;

            Node** t1 = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            (*t1) = current_treenode;
            syntaxtree_stack.push(&current_treenode->sibling);

            break;
        }
        case 67:
        {
            symbal_stack.push({ 0, AssignmentRest, "AssignmentRest" });

            current_treenode->kind.stmt = "AssignK";
            break;
        }
        case 68:
        {
            symbal_stack.push({ 0, CallStmRest, "CallStmRest" });
            /*过程调用语句左部是标识符变量，表示过程名*/
            current_treenode->child[0]->attr.ExpAttr.varkind = "IdV";

            current_treenode->kind.stmt = "CallK";
            break;
        }
        case 69:
        {
            symbal_stack.push({ 0, Exp, "Exp" });
            symbal_stack.push({ 1, ASSIGN, "ASSIGN" });
            symbal_stack.push({ 0, VariMore, "VariMore" });
            /*压入指向赋值右部的指针*/
            syntaxtree_stack.push(&(current_treenode->child[1]));
            /*当前指针指向赋值左部*/
            current_treenode = current_treenode->child[0];

            Node* t = ParserLL1::newExpNode("OpK");
            t->attr.ExpAttr.op = END1;  // why operation?
            op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志
            break;
        }
        case 70:
        {
            symbal_stack.push({ 1, FI, "FI" });
            symbal_stack.push({ 0, StmList, "StmList" });
            symbal_stack.push({ 1, ELSE, "ELSE" });
            symbal_stack.push({ 0, StmList, "StmList" });
            symbal_stack.push({ 1, THEN, "THEN" });
            symbal_stack.push({ 0, RelExp, "RelExp" });
            symbal_stack.push({ 1, IF, "IF" });

            syntaxtree_stack.push(&(current_treenode->child[2]));
            syntaxtree_stack.push(&(current_treenode->child[1]));
            syntaxtree_stack.push(&(current_treenode->child[0]));

            break;
        }

        case 71:
        {
            // symbal_stack.push({ 1, SEMI, "SEMI" }); // 这行拿掉，似乎是错了
            symbal_stack.push({ 1, ENDWH,"ENDWH" });
            symbal_stack.push( { 0, StmList, "StmList" });
            symbal_stack.push({ 1, DO, "DO" });
            symbal_stack.push({ 0, RelExp, "RelExp" });
            symbal_stack.push({ 1, WHILE, "WHILE" });

            syntaxtree_stack.push(&(current_treenode->child[1]));
            syntaxtree_stack.push(&(current_treenode->child[0]));

            break;
        }

        case 72:
        {
            symbal_stack.push({ 1, RPAREN, "RPAREN" });
            symbal_stack.push({ 0, InVar, "InVar" });
            symbal_stack.push({ 1, LPAREN, "LPAREN" });
            symbal_stack.push({ 1, READ, "READ" });
            break;
        }
        case 73:
        {
            symbal_stack.push({ 1, ID, "ID" });
            // no idea
            // current_treenode->name[0] = tokenList[head].getSem();
            // current_treenode->idnum++;
            current_treenode->name[0] = currentToken->wd.str;
            current_treenode->idnum++;
            break;
        }
        case 74:
        {

            symbal_stack.push({ 1, RPAREN, "RPAREN" });
            symbal_stack.push({ 0, Exp, "Exp" });
            symbal_stack.push({ 1, LPAREN, "LPAREN" });
            symbal_stack.push({ 1, WRITE, "WRITE" });

            syntaxtree_stack.push(&(current_treenode->child[0]));

            Node* t = ParserLL1::newExpNode("OpK");
            t->attr.ExpAttr.op = END1; // why operation?
            op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志
            break;
        }
        case 75:
        {
            symbal_stack.push({1, RETURN1, "RETURN1"});
            break;
        }

        case 76:
        {
            symbal_stack.push({ 1, RPAREN, "RPAREN" });
            symbal_stack.push({ 0, ActParamList, "ActParamList" });
            symbal_stack.push({ 1, LPAREN, "LPAREN" });

            syntaxtree_stack.push(&(current_treenode->child[1]));
            break;
        }
        case 77:
        {
            syntaxtree_stack.pop();
            break;
        }
        case 78:
        {
            // symbal_stack.push({ 1, LPAREN, "LPAREN" }); 这一行也有可能是错了的
            symbal_stack.push({ 0, ActParamMore, "ActParamMore" });
            symbal_stack.push({ 0, Exp, "Exp" });

            Node* t = ParserLL1::newExpNode("OpK");
            t->attr.ExpAttr.op = END1; // why operation?
            op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志
            break;
        }

        case 79:
        {
            break;
        }
        case 80:
        {
            symbal_stack.push({ 0, ActParamList, "ActParamList" });
            symbal_stack.push({ 1, COMMA, "COMMA" });

            syntaxtree_stack.push(&(current_treenode->sibling));
            break;
        }

        case 81:
        {
            symbal_stack.push({ 0, OtherRelE, "OtherRelE" });
            symbal_stack.push({ 0, Exp, "Exp" });

            Node* t = ParserLL1::newExpNode("OpK");
            t->attr.ExpAttr.op = END1;
            op_stack.push(t); //操作符栈的栈底存入一个特殊的操作符作为标志

            getExpResult = false;
            break;
        }

        case 82:
        {
            symbal_stack.push({ 0, Exp, "Exp" });
            symbal_stack.push({ 0, CmpOp, "CmpOp" });

            Node* current_treenode = ParserLL1::newExpNode("OpK");
            /*no idea
            // current_treenode->attr.ExpAttr.op = tokenList[head].getLexType();
            // no idea
            // LexType sTop = op_stack.top()->attr.ExpAttr.op;
            // while (this->getPriosity(sTop) >= this->getPriosity(tokenList[head].getLexType()))
                // 如果操作符栈顶运算符的优先级高于或等于当前读到的操作符
            // {
            //    TreeNode* t = op_stack.pop();
            //    TreeNode* Rnum = num_stack.pop();
            //    TreeNode* Lnum = num_stack.pop();
            //    t->child[1] = Rnum;
            //    t->child[0] = Lnum;
            //    num_stack.push(t);

            //    sTop = op_stack.top()->attr.ExpAttr.op;
            //}*/
            auto sTop = op_stack.top()->attr.ExpAttr.op;
            // cout << 82 << endl;
            // system("pause");
            while (this->getPriosity(sTop) >= this->getPriosity(currentToken->wd.tok))
                // 如果操作符栈顶运算符的优先级高于或等于当前读到的操作符
            {
                cout << "进去了啊" << endl;
                Node* t = op_stack.top();

                op_stack.pop();
                Node* Rnum = num_stack.top();



                num_stack.pop();
                if (num_stack.size() == 0)
                {
                    cout << "error" << endl;
                    cout << currentToken->line << " " << currentToken->wd.str << endl;
                    // exit(0);
                    longjmp(jump_buffer, 1);
                }
                Node* Lnum = num_stack.top();
                num_stack.pop();
                t->child[1] = Rnum;
                t->child[0] = Lnum;
                num_stack.push(t);

                sTop = op_stack.top()->attr.ExpAttr.op;
            }

            op_stack.push(current_treenode);
            /*处理完关系操作符右部的表达式时，要弹语法树栈，故
              设置getExpResult为真*/
            getExpResult = true;
            break;
        }

        case 83:
        {
            symbal_stack.push({ 0, OtherTerm, "OtherTerm" });
            symbal_stack.push({ 0, Term, "Term" });
            break;
        }

        case 84:
        {
            if ((currentToken->wd.tok == RPAREN) && (expflag != 0))
                //说明当前右括号是表达式中的一部分
            {
                while (op_stack.top()->attr.ExpAttr.op != LPAREN)
                {
                    Node* t = op_stack.top();
                    op_stack.pop();
                    Node* Rnum = num_stack.top();
                    num_stack.pop();
                    Node* Lnum = num_stack.top();
                    num_stack.pop();

                    t->child[1] = Rnum;
                    t->child[0] = Lnum;
                    num_stack.push(t);
                }
                op_stack.pop(); //弹出左括号
                expflag--;
            }
            else
            {
                if ((getExpResult) || (getExpResult2))
                {
                    while (op_stack.top()->attr.ExpAttr.op != END1)
                    {
                        Node* t = op_stack.top();
                        op_stack.pop();
                        Node* Rnum = num_stack.top();
                        num_stack.pop();
                        Node* Lnum = num_stack.top();
                        num_stack.pop();

                        t->child[1] = Rnum;
                        t->child[0] = Lnum;
                        num_stack.push(t);
                    }
                    op_stack.pop(); //弹出栈底标志
                    current_treenode = num_stack.top();
                    num_stack.pop();
                    Node** t = syntaxtree_stack.top();
                    syntaxtree_stack.pop();
                    (*t) = current_treenode;

                    //处理完数组变量，标志恢复初始值假，
                    //  遇到下一个数组下标表达式时，再将其设置为真值
                    if (getExpResult2 == true)
                        getExpResult2 = false;
                }
            }/**/
            break;
        }

        case 85:
        {
            symbal_stack.push({ 0, Exp, "Exp" });
            symbal_stack.push({ 0, AddOp, "AddOp" });

            Node* current_treenode = ParserLL1::newExpNode("OpK");
            // no idea

            current_treenode->attr.ExpAttr.op = currentToken->wd.tok;

            // cout << "85: " << current_treenode->attr.ExpAttr.op << endl;


            auto sTop = op_stack.top()->attr.ExpAttr.op;
            while (this->getPriosity(sTop) >= this->getPriosity(currentToken->wd.tok))
            {
                Node* t = op_stack.top();
                op_stack.pop();
                Node* Rnum = num_stack.top();
                num_stack.pop();
                Node* Lnum = num_stack.top();
                num_stack.pop();
                t->child[1] = Rnum;
                t->child[0] = Lnum;
                num_stack.push(t);
                sTop = op_stack.top()->attr.ExpAttr.op;
            }
            op_stack.push(current_treenode);
            /**/

            break;
        }

        case 86:
        {
            symbal_stack.push({ 0, OtherFactor, "OtherFactor" });
            symbal_stack.push({ 0, Factor, "Factor" });
            break;
        }
        case 87:
        {
            break;
        }
        case 88:
        {
            symbal_stack.push({ 0, Term, "Term" });
            symbal_stack.push({ 0, MultOp, "MultOp" });

            Node* current_treenode = ParserLL1::newExpNode("OpK");
            // no idea
            /*
            current_treenode->attr.ExpAttr.op = tokenList[head].getLexType();

            LexType sTop = op_stack.top()->attr.ExpAttr.op;
            while (this->getPriosity(sTop) >= this->getPriosity(tokenList[head].getLexType()))
                //如果操作符栈顶运算符的优先级高于或等于当前读到的操作符
            {
                TreeNode* t = op_stack.pop();
                TreeNode* Rnum = num_stack.pop();
                TreeNode* Lnum = num_stack.pop();
                t->child[1] = Rnum;
                t->child[0] = Lnum;
                num_stack.push(t);

                sTop = op_stack.top()->attr.ExpAttr.op;
            }
            op_stack.push(current_treenode);
            */

            // 以下的内容正在做
            current_treenode->attr.ExpAttr.op = currentToken->wd.tok;
            // cout << "88: " << current_treenode->attr.ExpAttr.op << endl;


            auto sTop = op_stack.top()->attr.ExpAttr.op;

            while (this->getPriosity(sTop) >= this->getPriosity(currentToken->wd.tok))
                //如果操作符栈顶运算符的优先级高于或等于当前读到的操作符
            {
                Node* t = op_stack.top();
                op_stack.pop();
                Node* Rnum = num_stack.top();
                num_stack.pop();
                Node* Lnum = num_stack.top();
                num_stack.pop();
                t->child[1] = Rnum;
                t->child[0] = Lnum;
                num_stack.push(t);

                sTop = op_stack.top()->attr.ExpAttr.op;
            }
            op_stack.push(current_treenode);
            /**/

            break;
        }

        case 89:
        {
            symbal_stack.push({ 1, RPAREN, "RPAREN" });
            symbal_stack.push({ 0, Exp, "Exp" });
            symbal_stack.push({ 1, LPAREN, "LPAREN" });

            Node* t = ParserLL1::newExpNode("OpK");
            // no idea
            // t->attr.ExpAttr.op = tokenList[head].getLexType(); /*把左括号也压入栈中*/
            t->attr.ExpAttr.op = currentToken->wd.tok;
            // cout << 89 << ": " << t->attr.ExpAttr.op << endl;


            op_stack.push(t);
            expflag++;
            break;
        }

        case 90:
        {
            symbal_stack.push({ 1, INTC, "INTC" });

            Node* t = ParserLL1::newExpNode("ConstK");
            // cout << t->kind.exp << endl;
            // system("pause");
            // no idea
            // t->attr.ExpAttr.val = tokenList[head].getSem().toInt();
            // 常数节点入操作数栈
            t->attr.ExpAttr.val = atoi(currentToken->wd.str.c_str());
            // cout << t->attr.ExpAttr.val << endl;
            // system("pause");
            // cout << "address: " << t << endl;
            num_stack.push(t);

            break;
        }

        case 91:
        {
            symbal_stack.push({ 0, Variable, "Variable" });
            break;
        }

        case 92:
        {
            symbal_stack.push({ 0, VariMore, "VariMore" });
            symbal_stack.push({ 1, ID, "ID" });

            current_treenode = ParserLL1::newExpNode("VariK");
            // no idea
            // current_treenode->name[0] = tokenList[head].getSem();
            // current_treenode->idnum++;
            // 变量节点入操作数栈
            current_treenode->name[0] = currentToken->wd.str;
            current_treenode->idnum++;
            num_stack.push(current_treenode);

            break;
        }

        case 93:
        {
            // 标识符变量
            current_treenode->attr.ExpAttr.varkind = "IdV";
            break;
        }

        case 94:
        {
            symbal_stack.push({ 1, RMIDPAREN, "RMIDPAREN" });
            symbal_stack.push({ 0, Exp, "Exp" });
            symbal_stack.push({ 1, LMIDPAREN, "LMIDPAREN" });
            // 数组成员变量
            current_treenode->attr.ExpAttr.varkind = "ArrayMembV";
            syntaxtree_stack.push(&current_treenode->child[0]);

            // 要进入表达式处理，初始化操作符栈
            //操作符栈的栈底存入一个特殊的操作符作为标志
            Node* t = ParserLL1::newExpNode("OpK");
            t->attr.ExpAttr.op = END1;
            op_stack.push(t);

            // 要进入数组下标表达式处理，在函数process84处理中，要
             //  操作语法树栈，故将标志getExpResult2设置为真值
            getExpResult2 = true;

            break;
        }

        case 95:
        {
            symbal_stack.push({ 0, FieldVar, "FieldVar"});
            symbal_stack.push({ 1, DOT, "DOT" });
            // 域成员变量
            current_treenode->attr.ExpAttr.varkind = "FieldMembV";
            syntaxtree_stack.push(&current_treenode->child[0]);
            break;
        }

        case 96:
        {
            symbal_stack.push({ 0, FieldVarMore, "FieldVarMore" });
            symbal_stack.push({ 1, ID, "ID" });

            // 纪录域的成员
            current_treenode = ParserLL1::newExpNode("VariK");
            // no idea
            // current_treenode->name[0] = tokenList[head].getSem();
            // current_treenode->idnum++;


            current_treenode->name[0] = currentToken->wd.str;
            current_treenode->idnum++;

            Node** t = syntaxtree_stack.top();
            syntaxtree_stack.pop();
            (*t) = current_treenode;

            break;
        }

        case 97:
        {
            /*域成员是标识符变量*/
            current_treenode->attr.ExpAttr.varkind = "IdV";
            break;
        }

        case 98:
        {
            symbal_stack.push({ 1, RMIDPAREN, "RMIDPAREN" });
            symbal_stack.push({ 0, Exp, "Exp" });
            symbal_stack.push({ 1, LMIDPAREN, "LMIDPAREN" });
            // 域成员是数组变量
            current_treenode->attr.ExpAttr.varkind = "ArrayMembV";
            // 指向数组成员表达式
            syntaxtree_stack.push(&current_treenode->child[0]);

            // 操作符栈的栈底存入一个特殊的操作符作为标志
            Node* t = ParserLL1::newExpNode("OpK");
            t->attr.ExpAttr.op = END1;
            op_stack.push(t);

            /*要进入数组下标表达式处理，在函数process84处理中，要
              操作语法树栈，故将标志getExpResult2设置为真值*/
            getExpResult2 = true;
            break;
        }
        case 99:
        {
            symbal_stack.push({ 1, LT, "LT" });
            break;
        }

        case 100:
        {
            symbal_stack.push({ 1, EQ, "EQ" });
            break;
        }

        case 101:
        {
            symbal_stack.push({ 1, PLUS, "PLUS" });
            break;
        }

        case 102:
        {
            symbal_stack.push({ 1, MINUS, "MINUS" });
            break;
        }

        case 103:
        {
            symbal_stack.push({ 1, TIMES, "TIMES" });
            break;
        }

        case 104:
        {
            symbal_stack.push({ 1, OVER, "OVER" });
            break;
        }

        default:
            cout << "ERROR in line: " << " " << currentToken->line << endl;
            is_newLL1_correct = 0;

            longjmp(jump_buffer, 1); // 跳转回去
            break;
    }
}


string NodePrintInfo(Node* node) {
    string str = node->nodekind;
    int is_array = 0;
    if (str == "DecK") {
        str += " ";
        str += string(node->kind.dec);
        str += " ";
        if (string(node->kind.dec) == "ArrayK")
        {
            is_array = 1;
        }
    }
    else if (str == "StmtK") {
        str += " ";
        str += string(node->kind.stmt);
        str += " ";
    }
    else if (str == "ExpK") {
        str += " ";
        string tmp = string(node->kind.exp);
        str += tmp;
        str += " ";
        if (tmp == "ConstK") {
            str += ":";
            stringstream strStream;
            strStream << node->attr.ExpAttr.val;
            str += strStream.str();
            str += " ";
        }
        if (tmp == "OpK") {
            str += " ";
            int op = node->attr.ExpAttr.op;
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
                // str += node->attr.ExpAttr.op
        }
        // 以下内容仅仅是测试
        if (node->attr.ExpAttr.varkind == "IdV" || node->attr.ExpAttr.varkind == "ArrayMembV" ||
            node->attr.ExpAttr.varkind == "FieldMembV") {
            str += node->attr.ExpAttr.varkind;
        }
    }

    // 以下用于显示标识符
    for (int i = 0; i < 10; i++)
    {
        if (node->name[i] != "")
        {
            if (i != 0)
            {
                // str += ",";
                str += " ";
            }
            else
            {
                str += ":";
            }
            str += node->name[i];
        }
    }
    if (is_array) {
        str += " ";
        str += to_string(node->attr.ArrayAttr.low);
        str += "..";
        str += to_string(node->attr.ArrayAttr.up);
    }
    return str;
}


void printTreeNewLL1(Node* t, int layer, bool islastson)
{
    if (!t)
        return;
    int last = 2;
    for (last = 2; last >= 0; --last) {
        if (t->child[last])
            break;
    }

    for (int i = 0; i < layer; i++)
    {
        outstrLL1new[strlineLL1new] += "   ";
    }

    if (layer == 0)
    {
        outstrLL1new[strlineLL1new] += "   ";
        outstrLL1new[strlineLL1new] += NodePrintInfo(t);
        //cout << t->name;
    }
    else
    {
        if (islastson)
        {
            // outstrLL1new[strlineLL1new] += "└ ";
            outstrLL1new[strlineLL1new] += "  ";
            outstrLL1new[strlineLL1new] += NodePrintInfo(t);
            //cout << "└ " << t->name;
        }
        else
        {
            // outstrLL1new[strlineLL1new] += "├ ";
            outstrLL1new[strlineLL1new] += "  ";
            outstrLL1new[strlineLL1new] += NodePrintInfo(t);
            //cout << "├ " << t->name;
        }
    }

    outstrLL1new[strlineLL1new] += "\n";
    strlineLL1new++;
    //outstr += "\n";
    //cout << endl;

    for (int i = 0; i <= last; i++)
    {
        if (i != last)
            printTreeNewLL1(t->child[i], layer + 1, false);
        else
            printTreeNewLL1(t->child[i], layer + 1, true);
    }

    if (t->sibling && t->sibling->sibling)
        printTreeNewLL1(t->sibling, layer, false);
    else
        printTreeNewLL1(t->sibling, layer, true);

    if (layer == 0)
    {
        // 首先对字符串进行修改
        for (int i = 1; i < strlineLL1new; i++)
        {
            int j = 0;
            /*while (outstrLL1new[i][j] != '\n')
            {
                if (outstrLL1new[i - 1][j] == '\n')
                    break;

                if (outstrLL1new[i - 1][j] == ch2[0] && outstrLL1new[i - 1][j + 1] == ch2[1]) // 这个很重要 不加的话会出问题
                {
                    ; // 上一行是拐
                }
                else if (outstrLL1new[i - 1][j] == ch1[0] && outstrLL1new[i - 1][j + 1] == ch1[1] && outstrLL1new[i][j] == ' ')
                {
                    // 上一行是卜 这一行是空格

                    for(int u = 0;u < ch3.size(); ++u){
                        outstrLL1new[i][j + u] = ch3[u];
                    }
                    // outstrLL1new[i][j] = ch3[0];
                    // outstrLL1new[i][j + 1] = ch3[1];
                }
                else if (outstrLL1new[i - 1][j] == ch3[0] && outstrLL1new[i][j] == ' ')
                {
                    // 上一行是棍 这一行是空格
                    for(int u = 0;u < ch3.size(); ++u){
                        outstrLL1new[i][j + u] = ch3[u];
                    }
                    // outstrLL1new[i][j] = ch3[0];
                    // outstrLL1new[i][j + 1] = ch3[1];
                }
                j++;
            }*/
        }

        for (int i = 0; i < strlineLL1new; i++)
        {
            cout << outstrLL1new[i];
        }
    }
}
