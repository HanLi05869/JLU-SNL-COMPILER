#include "func.h"
#include "global.h"
#include "globalVar.h"
#include "lexical.h"
#include "LL1.h"

using namespace std;


TreeNode* programLL1()
{
    Index = 0;
    NonTerminal stacktopN;
    Terminal stacktopT;
    initLL1();

    anlsstack->push(0, Program);
    (anlsstack->top())->p = new TreeNode("Program");
    TreeNode* root = (anlsstack->top())->p;
    readToken();
    while (!anlsstack->IsEmpty())
    {
        if ((anlsstack->top())->ntflag == 1)
        {

            stacktopT = ((anlsstack->top()))->t;
            if (stacktopT == currentToken->wd.tok)
            {
                auto tmp = (anlsstack->top());
                (anlsstack->top())->p->name = currentToken->wd.str;
                (anlsstack->top())->p->tk = currentToken;

                anlsstack->pop();
                readToken();
            }
            else
            {
                cout << "error in " << currentToken->wd.str << " " << " line: " << currentToken->line << endl;
                // readToken();
                longjmp(jump_buffer, 1);
            }
        }
        else // 非终极符
        {

            stacktopN = (anlsstack->top())->n; // restore nonterminal
            int num = LL1Tbl[stacktopN][currentToken->wd.tok]; // LL1
            currentTree = (anlsstack->top())->p; // restore the node being pop
            // 后面将要扩展它的子树
            anlsstack->pop();

            doSthInLL1(num);
        }
    }

    if (currentToken->wd.tok != ENDFILE)
    {
        /*/string temp;
        temp += "NotEndFile Error!\n";
        // printErrorMsg(temp);
        */
        cout << "ERROR in: " << currentToken->wd.str << " " << currentToken->line << endl;
        // exit(0);
        longjmp(jump_buffer, 1);
    }
    return  root;
}


//LL1表的初始化
void initLL1()
{
    memset(LL1Tbl, 0, sizeof(LL1Tbl));
    LL1Tbl[Program][PROGRAM] = 1;
    LL1Tbl[ProgramHead][PROGRAM] = 2;
    LL1Tbl[ProgramName][ID] = 3;
    LL1Tbl[DeclarePart][TYPE] = 4;
    LL1Tbl[DeclarePart][VAR] = 4;
    LL1Tbl[DeclarePart][PROCEDURE] = 4;
    LL1Tbl[DeclarePart][BEGIN] = 4;
    LL1Tbl[TypeDec][VAR] = 5;
    LL1Tbl[TypeDec][PROCEDURE] = 5;
    LL1Tbl[TypeDec][BEGIN] = 5;
    LL1Tbl[TypeDec][TYPE] = 6;
    LL1Tbl[TypeDeclaration][TYPE] = 7;
    LL1Tbl[TypeDecList][ID] = 8;
    LL1Tbl[TypeDecMore][VAR] = 9;
    LL1Tbl[TypeDecMore][PROCEDURE] = 9;
    LL1Tbl[TypeDecMore][BEGIN] = 9;
    LL1Tbl[TypeDecMore][ID] = 10;
    LL1Tbl[TypeId][ID] = 11;
    LL1Tbl[TypeName][INTEGER] = 12;
    LL1Tbl[TypeName][CHAR] = 12;

    LL1Tbl[TypeName][ARRAY] = 13;
    LL1Tbl[TypeName][RECORD] = 13;
    LL1Tbl[TypeName][ID] = 14;
    LL1Tbl[BaseType][INTEGER] = 15;
    LL1Tbl[BaseType][CHAR] = 16;
    LL1Tbl[StructureType][ARRAY] = 17;
    LL1Tbl[StructureType][RECORD] = 18;

    LL1Tbl[ArrayType][ARRAY] = 19;
    LL1Tbl[Low][INTC] = 20;
    LL1Tbl[Top][INTC] = 21;
    LL1Tbl[RecType][RECORD] = 22;
    LL1Tbl[FieldDecList][INTEGER] = 23;
    LL1Tbl[FieldDecList][CHAR] = 23;
    LL1Tbl[FieldDecList][ARRAY] = 24;
    LL1Tbl[FieldDecMore][END1] = 25;

    LL1Tbl[FieldDecMore][INTEGER] = 26;
    LL1Tbl[FieldDecMore][CHAR] = 26;
    LL1Tbl[FieldDecMore][ARRAY] = 26;
    LL1Tbl[IdList][ID] = 27;
    LL1Tbl[IdMore][SEMI] = 28;
    LL1Tbl[IdMore][COMMA] = 29;
    LL1Tbl[VarDec][PROCEDURE] = 30;
    LL1Tbl[VarDec][BEGIN] = 30;
    LL1Tbl[VarDec][VAR] = 31;
    LL1Tbl[VarDeclaration][VAR] = 32;

    LL1Tbl[VarDecList][INTEGER] = 33;
    LL1Tbl[VarDecList][CHAR] = 33;
    LL1Tbl[VarDecList][ARRAY] = 33;
    LL1Tbl[VarDecList][RECORD] = 33;
    LL1Tbl[VarDecList][ID] = 33;

    LL1Tbl[VarDecMore][PROCEDURE] = 34;
    LL1Tbl[VarDecMore][BEGIN] = 34;

    LL1Tbl[VarDecMore][INTEGER] = 35;
    LL1Tbl[VarDecMore][CHAR] = 35;
    LL1Tbl[VarDecMore][ARRAY] = 35;
    LL1Tbl[VarDecMore][RECORD] = 35;
    LL1Tbl[VarDecMore][ID] = 35;
    LL1Tbl[VarIdList][ID] = 36;
    LL1Tbl[VarIdMore][SEMI] = 37;
    LL1Tbl[VarIdMore][COMMA] = 38;
    LL1Tbl[ProcDec][BEGIN] = 39;
    LL1Tbl[ProcDec][PROCEDURE] = 40;
    LL1Tbl[ProcDeclaration][PROCEDURE] = 41;
    LL1Tbl[ProcDecMore][BEGIN] = 42;
    LL1Tbl[ProcDecMore][PROCEDURE] = 43;
    LL1Tbl[ProcName][ID] = 44;
    // LL1Tbl[ParamList][RPAREN] = 45;
    LL1Tbl[ParamList][RMIDPAREN] = 45;

    LL1Tbl[ParamList][INTEGER] = 46;
    LL1Tbl[ParamList][CHAR] = 46;
    LL1Tbl[ParamList][ARRAY] = 46;
    LL1Tbl[ParamList][RECORD] = 46;
    LL1Tbl[ParamList][ID] = 46;
    LL1Tbl[ParamList][VAR] = 46;
    LL1Tbl[ParamDecList][INTEGER] = 47;
    LL1Tbl[ParamDecList][CHAR] = 47;
    LL1Tbl[ParamDecList][ARRAY] = 47;
    LL1Tbl[ParamDecList][RECORD] = 47;
    LL1Tbl[ParamDecList][ID] = 47;
    LL1Tbl[ParamDecList][VAR] = 47;
    LL1Tbl[ParamMore][RPAREN] = 48;
    LL1Tbl[ParamMore][SEMI] = 49;
    LL1Tbl[Param][INTEGER] = 50;
    LL1Tbl[Param][CHAR] = 50;
    LL1Tbl[Param][ARRAY] = 50;
    LL1Tbl[Param][RECORD] = 50;
    LL1Tbl[Param][ID] = 50;
    LL1Tbl[Param][VAR] = 51;
    LL1Tbl[FormList][ID] = 52;
    LL1Tbl[FidMore][SEMI] = 53;
    LL1Tbl[FidMore][RPAREN] = 53;
    LL1Tbl[FidMore][COMMA] = 54;
    LL1Tbl[ProcDecPart][TYPE] = 55;
    LL1Tbl[ProcDecPart][VAR] = 55;
    LL1Tbl[ProcDecPart][PROCEDURE] = 55;
    LL1Tbl[ProcDecPart][BEGIN] = 55;

    LL1Tbl[ProcBody][BEGIN] = 56;
    LL1Tbl[ProgramBody][BEGIN] = 57;
    LL1Tbl[StmList][ID] = 58;
    LL1Tbl[StmList][IF] = 58;
    LL1Tbl[StmList][WHILE] = 58;
    LL1Tbl[StmList][RETURN1] = 58;
    LL1Tbl[StmList][READ] = 58;
    LL1Tbl[StmList][WRITE] = 58;

    LL1Tbl[StmMore][END1] = 59;
    LL1Tbl[StmMore][ENDWH] = 59;
    LL1Tbl[StmMore][ELSE] = 59;
    LL1Tbl[StmMore][FI] = 59;
    LL1Tbl[StmMore][SEMI] = 60;
    LL1Tbl[Stm][IF] = 61;
    LL1Tbl[Stm][WHILE] = 62;
    LL1Tbl[Stm][READ] = 63;
    LL1Tbl[Stm][WRITE] = 64;
    LL1Tbl[Stm][RETURN1] = 65;
    LL1Tbl[Stm][ID] = 66;
    LL1Tbl[AssCall][ASSIGN] = 67;
    LL1Tbl[AssCall][LMIDPAREN] = 67;
    LL1Tbl[AssCall][DOT] = 67;
    LL1Tbl[AssCall][LPAREN] = 68;
    LL1Tbl[AssignmentRest][ASSIGN] = 69;
    LL1Tbl[AssignmentRest][LMIDPAREN] = 69;
    LL1Tbl[AssignmentRest][DOT] = 69;

    LL1Tbl[ConditionalStm][IF] = 70;
    LL1Tbl[LoopStm][WHILE] = 71;
    LL1Tbl[InputStm][READ] = 72;
    LL1Tbl[InVar][ID] = 73;
    LL1Tbl[OutputStm][WRITE] = 74;
    LL1Tbl[ReturnStm][RETURN1] = 75;
    LL1Tbl[CallStmRest][LPAREN] = 76;
    LL1Tbl[ActParamList][RPAREN] = 77;
    LL1Tbl[ActParamList][ID] = 78;
    LL1Tbl[ActParamList][INTC] = 78;
    LL1Tbl[ActParamList][LPAREN] = 78;
    LL1Tbl[ActParamMore][RPAREN] = 79;
    LL1Tbl[ActParamMore][COMMA] = 80;

    LL1Tbl[RelExp][LPAREN] = 81;
    LL1Tbl[RelExp][INTC] = 81;
    LL1Tbl[RelExp][ID] = 81;
    LL1Tbl[OtherRelE][LT] = 82;
    LL1Tbl[OtherRelE][EQ] = 82;
    LL1Tbl[Exp][LPAREN] = 83;
    LL1Tbl[Exp][INTC] = 83;
    LL1Tbl[Exp][ID] = 83;

    LL1Tbl[OtherTerm][LT] = 84;
    LL1Tbl[OtherTerm][EQ] = 84;
    LL1Tbl[OtherTerm][THEN] = 84;
    LL1Tbl[OtherTerm][DO] = 84;
    LL1Tbl[OtherTerm][RPAREN] = 84;
    LL1Tbl[OtherTerm][END1] = 84;
    LL1Tbl[OtherTerm][SEMI] = 84;
    LL1Tbl[OtherTerm][COMMA] = 84;
    LL1Tbl[OtherTerm][ENDWH] = 84;
    LL1Tbl[OtherTerm][ELSE] = 84;
    LL1Tbl[OtherTerm][FI] = 84;
    LL1Tbl[OtherTerm][RMIDPAREN] = 84;

    LL1Tbl[OtherTerm][PLUS] = 85;
    LL1Tbl[OtherTerm][MINUS] = 85;

    LL1Tbl[Term][LPAREN] = 86;
    LL1Tbl[Term][INTC] = 86;
    LL1Tbl[Term][ID] = 86;

    LL1Tbl[OtherFactor][PLUS] = 87;
    LL1Tbl[OtherFactor][MINUS] = 87;
    LL1Tbl[OtherFactor][LT] = 87;
    LL1Tbl[OtherFactor][EQ] = 87;
    LL1Tbl[OtherFactor][THEN] = 87;
    LL1Tbl[OtherFactor][ELSE] = 87;
    LL1Tbl[OtherFactor][FI] = 87;
    LL1Tbl[OtherFactor][DO] = 87;
    LL1Tbl[OtherFactor][ENDWH] = 87;
    LL1Tbl[OtherFactor][RPAREN] = 87;
    LL1Tbl[OtherFactor][END1] = 87;
    LL1Tbl[OtherFactor][SEMI] = 87;
    LL1Tbl[OtherFactor][COMMA] = 87;
    LL1Tbl[OtherFactor][RMIDPAREN] = 87;

    LL1Tbl[OtherFactor][TIMES] = 88;
    LL1Tbl[OtherFactor][OVER] = 88;
    LL1Tbl[Factor][LPAREN] = 89;
    LL1Tbl[Factor][INTC] = 90;
    LL1Tbl[Factor][ID] = 91;
    LL1Tbl[Variable][ID] = 92;

    LL1Tbl[VariMore][ASSIGN] = 93;
    LL1Tbl[VariMore][TIMES] = 93;
    LL1Tbl[VariMore][OVER] = 93;
    LL1Tbl[VariMore][PLUS] = 93;
    LL1Tbl[VariMore][MINUS] = 93;
    LL1Tbl[VariMore][LT] = 93;
    LL1Tbl[VariMore][EQ] = 93;
    LL1Tbl[VariMore][THEN] = 93;
    LL1Tbl[VariMore][ELSE] = 93;
    LL1Tbl[VariMore][FI] = 93;
    LL1Tbl[VariMore][DO] = 93;
    LL1Tbl[VariMore][ENDWH] = 93;
    LL1Tbl[VariMore][RPAREN] = 93;
    LL1Tbl[VariMore][END1] = 93;
    LL1Tbl[VariMore][SEMI] = 93;
    LL1Tbl[VariMore][COMMA] = 93;
    LL1Tbl[VariMore][RMIDPAREN] = 93;
    LL1Tbl[VariMore][LMIDPAREN] = 94;
    LL1Tbl[VariMore][DOT] = 95;
    LL1Tbl[FieldVar][ID] = 96;

    LL1Tbl[FieldVarMore][ASSIGN] = 97;
    LL1Tbl[FieldVarMore][TIMES] = 97;
    LL1Tbl[FieldVarMore][OVER] = 97;
    LL1Tbl[FieldVarMore][PLUS] = 97;
    LL1Tbl[FieldVarMore][MINUS] = 97;
    LL1Tbl[FieldVarMore][LT] = 97;
    LL1Tbl[FieldVarMore][EQ] = 97;
    LL1Tbl[FieldVarMore][THEN] = 97;
    LL1Tbl[FieldVarMore][ELSE] = 97;
    LL1Tbl[FieldVarMore][FI] = 97;
    LL1Tbl[FieldVarMore][DO] = 97;
    LL1Tbl[FieldVarMore][ENDWH] = 97;
    LL1Tbl[FieldVarMore][RPAREN] = 97;
    LL1Tbl[FieldVarMore][END1] = 97;
    LL1Tbl[FieldVarMore][SEMI] = 97;
    LL1Tbl[FieldVarMore][COMMA] = 97;
    // new
    LL1Tbl[FieldVarMore][RMIDPAREN] = 97;

    LL1Tbl[FieldVarMore][LMIDPAREN] = 98;
    LL1Tbl[CmpOp][LT] = 99;
    LL1Tbl[CmpOp][EQ] = 100;
    LL1Tbl[AddOp][PLUS] = 101;
    LL1Tbl[AddOp][MINUS] = 102;
    LL1Tbl[MultOp][TIMES] = 103;
    LL1Tbl[MultOp][OVER] = 104;

}

// 对process中的一部分代码进行了封装 减小一点工作量
void pAddChild(int istmnl, int lex, string name, int childindex)
{
    anlsstack->push(istmnl, lex); // 压栈 逆着压 确保最左在最上

    (anlsstack->top())->p = new TreeNode(name); // 新建树结点

    if (currentTree->child.size() < childindex + 1) {
        currentTree->child.resize(childindex + 1);
    }

    currentTree->child[childindex] = (anlsstack->top())->p; // 连接在父结点上

}



/*
void processAddChild(int istmnl, int lex, string name, int childindex)
{
    anlsstack->push(istmnl, lex); // 压栈 逆着压 确保最左在最上

    (anlsstack->top())->p = new TreeNode(name); // 新建树结点

    if (currentTree->child.size() < childindex + 1) {
        currentTree->child.resize(childindex + 1);
    }

    currentTree->child[childindex] = (anlsstack->top())->p; // 连接在父结点上
}
*/
// 将书上104个process封装在一个函数中
void doSthInLL1(int num)
{
    // cout << num << endl;
    switch (num)
    {
    case 1: {
        int cnt = 4;
        pAddChild(1, DOT, "DOT", --cnt);
        pAddChild(0, ProgramBody, "ProgramBody", --cnt);
        pAddChild(0, DeclarePart, "DeclarePart", --cnt);
        pAddChild(0, ProgramHead, "ProgramHead", --cnt);
    }break;
    case 2: {
        int cnt = 2;
        pAddChild(0, ProgramName, "ProgramName", --cnt);
        pAddChild(1, PROGRAM, "PROGRAM", --cnt);
    }break;
    case 3: {
        int cnt = 1;
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 4: {
        int cnt = 3;
        pAddChild(0, ProcDec, "ProcDec", --cnt);
        pAddChild(0, VarDec, "VarDec", --cnt);
        pAddChild(0, TypeDec, "TypeDec", --cnt);
    }break;
    case 5: break;

    case 6: {
        int cnt = 1;
        pAddChild(0, TypeDeclaration, "TypeDeclaration", --cnt);
    }break;
    case 7: {
        int cnt = 2;
        pAddChild(0, TypeDecList, "TypeDecList", --cnt);
        pAddChild(1, TYPE, "TYPE", --cnt);
    }break;
    case 8: {
        int cnt = 5;
        pAddChild(0, TypeDecMore, "TypeDecMore", --cnt);
        pAddChild(1, SEMI, "SEMI", --cnt);
        pAddChild(0, TypeName, "TypeName", --cnt);
        pAddChild(1, EQ, "EQ", --cnt);
        pAddChild(0, TypeId, "TypeID", --cnt);
    }break;
    case 9: break;
    case 10: {
        int cnt = 1;
        pAddChild(0, TypeDecList, "TypeDecList", --cnt);
    }break;
    case 11: {
        int cnt = 1;
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 12: {
        int cnt = 1;
        pAddChild(0, BaseType, "BaseType", --cnt);
    }break;
    case 13: {
        int cnt = 1;
        pAddChild(0, StructureType, "StructureType", --cnt);
    }break;
    case 14: {
        int cnt = 1;
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 15: {
        int cnt = 1;
        pAddChild(1, INTEGER, "INTEGER", --cnt);
    }break;
    case 16: {
        int cnt = 1;
        pAddChild(1, CHAR, "CHAR", --cnt);
    }break;
    case 17: {
        int cnt = 1;
        pAddChild(0, ArrayType, "ArrayType", --cnt);
    }break;
    case 18: {
        int cnt = 1;
        pAddChild(0, RecType, "RecType", --cnt);
    }break;
    case 19: {
        int cnt = 8;
        pAddChild(0, BaseType, "BaseType", --cnt);
        pAddChild(1, OF, "OF", --cnt);
        pAddChild(1, RMIDPAREN, "RMIDPAREN", --cnt);
        pAddChild(0, Top, "Top", --cnt);
        pAddChild(1, UNDERANGE, "UNDERANGE", --cnt);
        pAddChild(0, Low, "Low", --cnt);
        pAddChild(1, LMIDPAREN, "LMIDPAREN", --cnt);
        pAddChild(1, ARRAY, "ARRAY", --cnt);
    }break;
    case 20: {
        int cnt = 1;
        pAddChild(1, INTC, "INTC", --cnt);
    }break;
    case 21: {
        int cnt = 1;
        pAddChild(1, INTC, "INTC", --cnt);
    }break;
    case 22: {
        int cnt = 3;
        pAddChild(1, END1, "END1", --cnt);
        pAddChild(0, FieldDecList, "FieldDecList", --cnt);
        pAddChild(1, RECORD, "RECORD", --cnt);
    }break;
    case 23: {
        int cnt = 4;
        pAddChild(0, FieldDecMore, "FieldDecMore", --cnt);
        pAddChild(1, SEMI, "SEMI", --cnt);
        pAddChild(0, IdList, "IDList", --cnt);
        pAddChild(0, BaseType, "BaseType", --cnt);
    }break;
    case 24: {
        int cnt = 4;
        pAddChild(0, FieldDecMore, "FieldDecMore", --cnt);
        pAddChild(1, SEMI, "SEMI", --cnt);
        pAddChild(0, IdList, "IDList", --cnt);
        pAddChild(0, ArrayType, "ArrayType", --cnt);
    }break;
    case 25: break;
    case 26: {
        int cnt = 1;
        pAddChild(0, FieldDecList, "FieldDecList", --cnt);
    }break;
    case 27: {
        int cnt = 2;
        pAddChild(0, IdMore, "IDMore", --cnt);
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 28: break;
    case 29: {
        int cnt = 2;
        pAddChild(0, IdList, "IDList", --cnt);
        pAddChild(1, COMMA, "COMMA", --cnt);
    }break;
    case 30: break;
    case 31: {
        int cnt = 1;
        pAddChild(0, VarDeclaration, "VarDeclaration", --cnt);
    }break;
    case 32: {
        int cnt = 2;
        pAddChild(0, VarDecList, "VarDecList", --cnt);
        pAddChild(1, VAR, "VAR", --cnt);
    }break;
    case 33: {
        int cnt = 4;
        pAddChild(0, VarDecMore, "VarDecMore", --cnt);
        pAddChild(1, SEMI, "SEMI", --cnt);
        pAddChild(0, VarIdList, "VarIDList", --cnt);
        pAddChild(0, TypeName, "TypeName", --cnt);
    }break;
    case 34: break;
    case 35: {
        int cnt = 1;
        pAddChild(0, VarDecList, "VarDecList", --cnt);
    }break;
    case 36: {
        int cnt = 2;
        pAddChild(0, VarIdMore, "VarIDMore", --cnt);
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 37: break;
    case 38: {
        int cnt = 2;
        pAddChild(0, VarIdList, "VarIDList", --cnt);
        pAddChild(1, COMMA, "COMMA", --cnt);
    }break;
    case 39: break;
    case 40: {
        int cnt = 1;
        pAddChild(0, ProcDeclaration, "ProcDeclaration", --cnt);
    }break;
    case 41: {
        int cnt = 9;
        pAddChild(0, ProcDecMore, "ProcDecMore", --cnt);
        pAddChild(0, ProcBody, "ProcBody", --cnt);
        pAddChild(0, ProcDecPart, "ProcDecPart", --cnt);
        pAddChild(1, SEMI, "SEMI", --cnt);
        pAddChild(1, RPAREN, "RPAREN", --cnt);
        pAddChild(0, ParamList, "ParamList", --cnt);
        pAddChild(1, LPAREN, "LPAREN", --cnt);
        pAddChild(0, ProcName, "ProcName", --cnt);
        pAddChild(1, PROCEDURE, "PROCEDURE", --cnt);
    }break;
    case 42: break;
    case 43: {
        int cnt = 1;
        pAddChild(0, ProcDeclaration, "ProcDeclaration", --cnt);
    }break;
    case 44: {
        int cnt = 1;
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 45: break;
    case 46: {
        int cnt = 1;
        pAddChild(0, ParamDecList, "ParamDecList", --cnt);
    }break;
    case 47: {
        int cnt = 2;
        pAddChild(0, ParamMore, "ParamMore", --cnt);
        pAddChild(0, Param, "Param", --cnt);
    }break;
    case 48: break;
    case 49: {
        int cnt = 2;
        pAddChild(0, ParamDecList, "ParamDecList", --cnt);
        pAddChild(1, SEMI, "SEMI", --cnt);
    }break;
    case 50: {
        int cnt = 2;
        pAddChild(0, FormList, "FormList", --cnt);
        pAddChild(0, TypeName, "TypeName", --cnt);
    }break;
    case 51: {
        int cnt = 3;
        pAddChild(0, FormList, "FormList", --cnt);
        pAddChild(0, TypeName, "TypeName", --cnt);
        pAddChild(1, VAR, "VAR", --cnt);
    }break;
    case 52: {
        int cnt = 2;
        pAddChild(0, FidMore, "FidMore", --cnt);
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 53: break;
    case 54: {
        int cnt = 2;
        pAddChild(0, FormList, "FormList", --cnt);
        pAddChild(1, COMMA, "COMMA", --cnt);
    }break;
    case 55: {
        int cnt = 1;
        pAddChild(0, DeclarePart, "DeclarePart", --cnt);
    }break;
    case 56: {
        int cnt = 1;
        pAddChild(0, ProgramBody, "ProgramBody", --cnt);
    }break;
    case 57: {
        int cnt = 3;
        pAddChild(1, END1, "END1", --cnt);
        pAddChild(0, StmList, "StmList", --cnt);
        pAddChild(1, BEGIN, "BEGIN", --cnt);
    }break;
    case 58: {
        int cnt = 2;
        pAddChild(0, StmMore, "StmMore", --cnt);
        pAddChild(0, Stm, "Stm", --cnt);
    }break;
    case 59: break;
    case 60: {
        int cnt = 2;
        pAddChild(0, StmList, "StmList", --cnt);
        pAddChild(1, SEMI, "SEMI", --cnt);
    }break;
    case 61: {
        int cnt = 1;
        pAddChild(0, ConditionalStm, "ConditionalStm", --cnt);
    }break;
    case 62: {
        int cnt = 1;
        pAddChild(0, LoopStm, "LoopStm", --cnt);
    }break;
    case 63: {
        int cnt = 1;
        pAddChild(0, InputStm, "InputStm", --cnt);
    }break;
    case 64: {
        int cnt = 1;
        pAddChild(0, OutputStm, "OutputStm", --cnt);
    }break;
    case 65: {
        int cnt = 1;
        pAddChild(0, ReturnStm, "ReturnStm", --cnt);
    }break;
    case 66: {
        int cnt = 2;
        pAddChild(0, AssCall, "AssCall", --cnt);
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 67: {
        int cnt = 1;
        pAddChild(0, AssignmentRest, "AssignmentRest", --cnt);
    }break;
    case 68: {
        int cnt = 1;
        pAddChild(0, CallStmRest, "CallStmRest", --cnt);
    }break;
    case 69: {
        int cnt = 3;
        pAddChild(0, Exp, "Exp", --cnt);
        pAddChild(1, ASSIGN, "ASSIGN", --cnt);
        pAddChild(0, VariMore, "VariMore", --cnt);
    }break;
    case 70: {
        int cnt = 7;
        pAddChild(1, FI, "FI", --cnt);
        pAddChild(0, StmList, "StmList", --cnt);
        pAddChild(1, ELSE, "ELSE", --cnt);
        pAddChild(0, StmList, "StmList", --cnt);
        pAddChild(1, THEN, "THEN", --cnt);
        pAddChild(0, RelExp, "RelExp", --cnt);
        pAddChild(1, IF, "IF", --cnt);
    }break;
    case 71: {
        int cnt = 5;
        //pAddChild(1, SEMI, "SEMI", --cnt);
        pAddChild(1, ENDWH, "ENDWH", --cnt);
        pAddChild(0, StmList, "StmList", --cnt);
        pAddChild(1, DO, "DO", --cnt);
        pAddChild(0, RelExp, "RelExp", --cnt);
        pAddChild(1, WHILE, "WHILE", --cnt);
    }break;
    case 72: {
        int cnt = 4;
        pAddChild(1, RPAREN, "RPAREN", --cnt);
        pAddChild(0, InVar, "InVar", --cnt);
        pAddChild(1, LPAREN, "LPAREN", --cnt);
        pAddChild(1, READ, "READ", --cnt);
    }break;
    case 73: {
        int cnt = 1;
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 74: {
        int cnt = 4;
        pAddChild(1, RPAREN, "RPAREN", --cnt);
        pAddChild(0, Exp, "Exp", --cnt);
        pAddChild(1, LPAREN, "LPAREN", --cnt);
        pAddChild(1, WRITE, "WRITE", --cnt);
    }break;
    case 75: {
        int cnt = 1;
        pAddChild(1, RETURN1, "RETURN1", --cnt);
    }break;
    case 76: {
        int cnt = 3;
        pAddChild(1, RPAREN, "RPAREN", --cnt);
        pAddChild(0, ActParamList, "ActParamList", --cnt);
        pAddChild(1, LPAREN, "LPAREN", --cnt);
    }break;
    case 77: break;
    case 78: {
        int cnt = 2;
        pAddChild(0, ActParamMore, "ActParamMore", --cnt);
        pAddChild(0, Exp, "Exp", --cnt);
    }break;
    case 79: break;
    case 80: {
        int cnt = 2;
        pAddChild(0, ActParamList, "ActParamList", --cnt);
        pAddChild(1, COMMA, "COMMA", --cnt);
    }break;
    case 81: {
        int cnt = 2;
        pAddChild(0, OtherRelE, "OtherRelE", --cnt);
        pAddChild(0, Exp, "Exp", --cnt);
    }break;
    case 82: {
        int cnt = 2;
        pAddChild(0, Exp, "Exp", --cnt);
        pAddChild(0, CmpOp, "CmpOp", --cnt);
    }break;
    case 83: {
        int cnt = 2;
        pAddChild(0, OtherTerm, "OtherTerm", --cnt);
        pAddChild(0, Term, "Term", --cnt);
    }break;
    case 84: break;
    case 85: {
        int cnt = 2;
        pAddChild(0, Exp, "Exp", --cnt);
        pAddChild(0, AddOp, "AddOp", --cnt);
    }break;
    case 86: {
        int cnt = 2;
        pAddChild(0, OtherFactor, "OtherFactor", --cnt);
        pAddChild(0, Factor, "Factor", --cnt);
    }break;
    case 87: break;
    case 88: {
        int cnt = 2;
        pAddChild(0, Term, "Term", --cnt);
        pAddChild(0, MultOp, "MultOp", --cnt);
    }break;
    case 89: {
        int cnt = 3;
        pAddChild(1, RPAREN, "RPAREN", --cnt);
        pAddChild(0, Exp, "Exp", --cnt);
        pAddChild(1, LPAREN, "LPAREN", --cnt);
    }break;
    case 90: {
        int cnt = 1;
        pAddChild(1, INTC, "INTC", --cnt);
    }break;
    case 91: {
        int cnt = 1;
        pAddChild(0, Variable, "Variable", --cnt);
    }break;
    case 92: {
        int cnt = 2;
        pAddChild(0, VariMore, "VariMore", --cnt);
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 93: break;
    case 94: {
        int cnt = 3;
        pAddChild(1, RMIDPAREN, "RMIDPAREN", --cnt);
        pAddChild(0, Exp, "Exp", --cnt);
        pAddChild(1, LMIDPAREN, "LMIDPAREN", --cnt);
    }break;
    case 95: {
        int cnt = 2;
        pAddChild(0, FieldVar, "FieldVar", --cnt);
        pAddChild(1, DOT, "DOT", --cnt);
    }break;
    case 96: {
        int cnt = 2;
        pAddChild(0, FieldVarMore, "FieldVarMore", --cnt);
        pAddChild(1, ID, "ID", --cnt);
    }break;
    case 97: break;
    case 98: {
        int cnt = 3;
        pAddChild(1, RMIDPAREN, "RMIDPAREN", --cnt);
        pAddChild(0, Exp, "Exp", --cnt);
        pAddChild(1, LMIDPAREN, "LMIDPAREN", --cnt);
    }break;
    case 99: {
        int cnt = 1;
        pAddChild(1, LT, "LT", --cnt);
    }break;
    case 100: {
        int cnt = 1;
        pAddChild(1, EQ, "EQ", --cnt);
    }break;
    case 101: {
        int cnt = 1;
        pAddChild(1, PLUS, "PLUS", --cnt);
    }break;
    case 102: {
        int cnt = 1;
        pAddChild(1, MINUS, "MINUS", --cnt);
    }break;
    case 103: {
        int cnt = 1;
        pAddChild(1, TIMES, "TIMES", --cnt);
    }break;
    case 104: {
        int cnt = 1;
        pAddChild(1, OVER, "OVER", --cnt);
    }break;

    default: break;
    }

}


/*
void doSthInLL1(int num)
{
    if (num == 1)
    {
        int count = 4;
        processAddChild(1, DOT, "DOT", --count);
        processAddChild(0, ProgramBody, "ProgramBody", --count);
        processAddChild(0, DeclarePart, "DeclarePart", --count);
        processAddChild(0, ProgramHead, "ProgramHead", --count);
    }
    else if (num == 2)
    {
        int count = 2;
        processAddChild(0, ProgramName, "ProgramName", --count);
        processAddChild(1, PROGRAM, "PROGRAM", --count);
    }
    else if (num == 3)
    {
        int count = 1;
        //		processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 4)
    {
        int count = 3;
        processAddChild(0, ProcDec, "ProcDec", --count);
        processAddChild(0, VarDec, "VarDec", --count);
        processAddChild(0, TypeDec, "TypeDec", --count);
    }
    else if (num == 5)
    {
        ;
    }
    else if (num == 6)
    {
        int count = 1;
        processAddChild(0, TypeDeclaration, "TypeDeclaration", --count);
    }
    else if (num == 7)
    {
        int count = 2;
        processAddChild(0, TypeDecList, "TypeDecList", --count);
        processAddChild(1, TYPE, "TYPE", --count);
    }
    else if (num == 8)
    {
        int count = 5;
        processAddChild(0, TypeDecMore, "TypeDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, TypeName, "TypeName", --count);
        processAddChild(1, EQ, "EQ", --count);
        processAddChild(0, TypeId, "TypeID", --count);
    }
    else if (num == 9)
    {
        ;
    }
    else if (num == 10)
    {
        int count = 1;
        processAddChild(0, TypeDecList, "TypeDecList", --count);
    }
    else if (num == 11)
    {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 12)
    {
        int count = 1;
        processAddChild(0, BaseType, "BaseType", --count);
    }
    else if (num == 13)
    {
        int count = 1;
        processAddChild(0, StructureType, "StructureType", --count);
    }
    else if (num == 14)
    {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 15)
    {
        int count = 1;
        processAddChild(1, INTEGER, "INTEGER", --count);
    }
    else if (num == 16)
    {
        int count = 1;
        processAddChild(1, CHAR, "CHAR", --count);
    }
    else if (num == 17)
    {
        int count = 1;
        processAddChild(0, ArrayType, "ArrayType", --count);
    }
    else if (num == 18)
    {
        int count = 1;
        processAddChild(0, RecType, "RecType", --count);
    }
    else if (num == 19)
    {
        int count = 8;
        processAddChild(0, BaseType, "BaseType", --count);
        processAddChild(1, OF, "OF", --count);
        processAddChild(1, RMIDPAREN, "RMIDPAREN", --count);
        processAddChild(0, Top, "Top", --count);
        processAddChild(1, UNDERANGE, "UNDERANGE", --count);
        processAddChild(0, Low, "Low", --count);
        processAddChild(1, LMIDPAREN, "LMIDPAREN", --count);
        processAddChild(1, ARRAY, "ARRAY", --count);
    }
    else if (num == 20)
    {
        int count = 1;
        processAddChild(1, INTC, "INTC", --count);
    }
    else if (num == 21)
    {
        int count = 1;
        processAddChild(1, INTC, "INTC", --count);
    }
    else if (num == 22)
    {
        int count = 3;
        processAddChild(1, END1, "END1", --count);
        processAddChild(0, FieldDecList, "FieldDecList", --count);
        processAddChild(1, RECORD, "RECORD", --count);
    }
    else if (num == 23)
    {
        int count = 4;
        processAddChild(0, FieldDecMore, "FieldDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, IdList, "IDList", --count);
        processAddChild(0, BaseType, "BaseType", --count);
    }
    else if (num == 24)
    {
        int count = 4;
        processAddChild(0, FieldDecMore, "FieldDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, IdList, "IDList", --count);
        processAddChild(0, ArrayType, "ArrayType", --count);
    }
    else if (num == 25)
    {
        ;
    }
    else if (num == 26)
    {
        int count = 1;
        processAddChild(0, FieldDecList, "FieldDecList", --count);
    }
    else if (num == 27)
    {
        int count = 2;
        processAddChild(0, IdMore, "IDMore", --count);
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 28)
    {
        ;
    }
    else if (num == 29)
    {
        int count = 2;
        processAddChild(0, IdList, "IDList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    }
    else if (num == 30)
    {
        ;
    }
    else if (num == 31)
    {
        int count = 1;
        processAddChild(0, VarDeclaration, "VarDeclaration", --count);
    }
    else if (num == 32)
    {
        int count = 2;
        processAddChild(0, VarDecList, "VarDecList", --count);
        processAddChild(1, VAR, "VAR", --count);
    }
    else if (num == 33)
    {
        int count = 4;
        processAddChild(0, VarDecMore, "VarDecMore", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(0, VarIdList, "VarIDList", --count);
        processAddChild(0, TypeName, "TypeName", --count);
    }
    else if (num == 34)
    {
        ;
    }
    else if (num == 35)
    {
        int count = 1;
        processAddChild(0, VarDecList, "VarDecList", --count);
    }
    else if (num == 36)
    {
        int count = 2;
        processAddChild(0, VarIdMore, "VarIDMore", --count);
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 37)
    {
        ;
    }
    else if (num == 38)
    {
        int count = 2;
        processAddChild(0, VarIdList, "VarIDList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    }
    else if (num == 39)
    {
        ;
    }
    else if (num == 40)
    {
        int count = 1;
        processAddChild(0, ProcDeclaration, "ProcDeclaration", --count);
    }
    else if (num == 41)
    {
        int count = 9;
        processAddChild(0, ProcDecMore, "ProcDecMore", --count);
        processAddChild(0, ProcBody, "ProcBody", --count);
        processAddChild(0, ProcDecPart, "ProcDecPart", --count);
        processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, ParamList, "ParamList", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
        processAddChild(0, ProcName, "ProcName", --count);
        processAddChild(1, PROCEDURE, "PROCEDURE", --count);
    }
    else if (num == 42)
    {
        ;
    }
    else if (num == 43)
    {
        int count = 1;
        processAddChild(0, ProcDeclaration, "ProcDeclaration", --count);
    }
    else if (num == 44)
    {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 45)
    {
        ;
    }
    else if (num == 46)
    {
        int count = 1;
        processAddChild(0, ParamDecList, "ParamDecList", --count);
    }
    else if (num == 47)
    {
        int count = 2;
        processAddChild(0, ParamMore, "ParamMore", --count);
        processAddChild(0, Param, "Param", --count);
    }
    else if (num == 48)
    {
        ;
    }
    else if (num == 49)
    {
        int count = 2;
        processAddChild(0, ParamDecList, "ParamDecList", --count);
        processAddChild(1, SEMI, "SEMI", --count);
    }
    else if (num == 50)
    {
        int count = 2;
        processAddChild(0, FormList, "FormList", --count);
        processAddChild(0, TypeName, "TypeName", --count);
    }
    else if (num == 51)
    {
        int count = 3;
        processAddChild(0, FormList, "FormList", --count);
        processAddChild(0, TypeName, "TypeName", --count);
        processAddChild(1, VAR, "VAR", --count);
    }
    else if (num == 52)
    {
        int count = 2;
        processAddChild(0, FidMore, "FidMore", --count);
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 53)
    {
        ;
    }
    else if (num == 54)
    {
        int count = 2;
        processAddChild(0, FormList, "FormList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    }
    else if (num == 55)
    {
        int count = 1;
        processAddChild(0, DeclarePart, "DeclarePart", --count);
    }
    else if (num == 56)
    {
        int count = 1;
        processAddChild(0, ProgramBody, "ProgramBody", --count);
    }
    else if (num == 57)
    {
        int count = 3;
        processAddChild(1, END1, "END1", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, BEGIN, "BEGIN", --count);
    }
    else if (num == 58)
    {
        int count = 2;
        processAddChild(0, StmMore, "StmMore", --count);
        processAddChild(0, Stm, "Stm", --count);
    }
    else if (num == 59)
    {
        ;
    }
    else if (num == 60)
    {
        int count = 2;
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, SEMI, "SEMI", --count);
    }
    else if (num == 61)
    {
        int count = 1;
        processAddChild(0, ConditionalStm, "ConditionalStm", --count);
    }
    else if (num == 62)
    {
        int count = 1;
        processAddChild(0, LoopStm, "LoopStm", --count);
    }
    else if (num == 63)
    {
        int count = 1;
        processAddChild(0, InputStm, "InputStm", --count);
    }
    else if (num == 64)
    {
        int count = 1;
        processAddChild(0, OutputStm, "OutputStm", --count);
    }
    else if (num == 65)
    {
        int count = 1;
        processAddChild(0, ReturnStm, "ReturnStm", --count);
    }
    else if (num == 66)
    {
        int count = 2;
        processAddChild(0, AssCall, "AssCall", --count);
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 67)
    {
        int count = 1;
        processAddChild(0, AssignmentRest, "AssignmentRest", --count);
    }
    else if (num == 68)
    {
        int count = 1;
        processAddChild(0, CallStmRest, "CallStmRest", --count);
    }
    else if (num == 69)
    {
        int count = 3;
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, ASSIGN, "ASSIGN", --count);
        processAddChild(0, VariMore, "VariMore", --count);
    }
    else if (num == 70)
    {
        int count = 7;
        processAddChild(1, FI, "FI", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, ELSE, "ELSE", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, THEN, "THEN", --count);
        processAddChild(0, RelExp, "RelExp", --count);
        processAddChild(1, IF, "IF", --count);
    }
    else if (num == 71)
    {
        int count = 5;
        //processAddChild(1, SEMI, "SEMI", --count);
        processAddChild(1, ENDWH, "ENDWH", --count);
        processAddChild(0, StmList, "StmList", --count);
        processAddChild(1, DO, "DO", --count);
        processAddChild(0, RelExp, "RelExp", --count);
        processAddChild(1, WHILE, "WHILE", --count);
    }
    else if (num == 72)
    {
        int count = 4;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, InVar, "InVar", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
        processAddChild(1, READ, "READ", --count);
    }
    else if (num == 73)
    {
        int count = 1;
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 74)
    {
        int count = 4;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
        processAddChild(1, WRITE, "WRITE", --count);
    }
    else if (num == 75)
    {
        int count = 1;
        processAddChild(1, RETURN1, "RETURN1", --count);
    }
    else if (num == 76)
    {
        int count = 3;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, ActParamList, "ActParamList", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
    }
    else if (num == 77)
    {
        ;
    }
    else if (num == 78)
    {
        int count = 2;
        processAddChild(0, ActParamMore, "ActParamMore", --count);
        processAddChild(0, Exp, "Exp", --count);
    }
    else if (num == 79)
    {
        ;
    }
    else if (num == 80)
    {
        int count = 2;
        processAddChild(0, ActParamList, "ActParamList", --count);
        processAddChild(1, COMMA, "COMMA", --count);
    }
    else if (num == 81)
    {
        int count = 2;
        processAddChild(0, OtherRelE, "OtherRelE", --count);
        processAddChild(0, Exp, "Exp", --count);
    }
    else if (num == 82)
    {
        int count = 2;
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(0, CmpOp, "CmpOp", --count);
    }
    else if (num == 83)
    {
        int count = 2;
        processAddChild(0, OtherTerm, "OtherTerm", --count);
        processAddChild(0, Term, "Term", --count);
    }
    else if (num == 84)
    {
        ;
    }
    else if (num == 85)
    {
        int count = 2;
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(0, AddOp, "AddOp", --count);
    }
    else if (num == 86)
    {
        int count = 2;
        processAddChild(0, OtherFactor, "OtherFactor", --count);
        processAddChild(0, Factor, "Factor", --count);
    }
    else if (num == 87)
    {
        ;
    }
    else if (num == 88)
    {
        int count = 2;
        processAddChild(0, Term, "Term", --count);
        processAddChild(0, MultOp, "MultOp", --count);
    }
    else if (num == 89)
    {
        int count = 3;
        processAddChild(1, RPAREN, "RPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LPAREN, "LPAREN", --count);
    }
    else if (num == 90)
    {
        int count = 1;
        processAddChild(1, INTC, "INTC", --count);
    }
    else if (num == 91)
    {
        int count = 1;
        processAddChild(0, Variable, "Variable", --count);
    }
    else if (num == 92)
    {
        int count = 2;
        processAddChild(0, VariMore, "VariMore", --count);
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 93)
    {
        ;
    }
    else if (num == 94)
    {
        int count = 3;
        processAddChild(1, RMIDPAREN, "RMIDPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LMIDPAREN, "LMIDPAREN", --count);
    }
    else if (num == 95)
    {
        int count = 2;
        processAddChild(0, FieldVar, "FieldVar", --count);
        processAddChild(1, DOT, "DOT", --count);
    }
    else if (num == 96)
    {
        int count = 2;
        processAddChild(0, FieldVarMore, "FieldVarMore", --count);
        processAddChild(1, ID, "ID", --count);
    }
    else if (num == 97)
    {
        ;
    }
    else if (num == 98)
    {
        int count = 3;
        processAddChild(1, RMIDPAREN, "RMIDPAREN", --count);
        processAddChild(0, Exp, "Exp", --count);
        processAddChild(1, LMIDPAREN, "LMIDPAREN", --count);
    }
    else if (num == 99)
    {
        int count = 1;
        processAddChild(1, LT, "LT", --count);
    }
    else if (num == 100)
    {
        int count = 1;
        processAddChild(1, EQ, "EQ", --count);
    }
    else if (num == 101)
    {
        int count = 1;
        processAddChild(1, PLUS, "PLUS", --count);
    }
    else if (num == 102)
    {
        int count = 1;
        processAddChild(1, MINUS, "MINUS", --count);
    }
    else if (num == 103)
    {
        int count = 1;
        processAddChild(1, TIMES, "TIMES", --count);
    }
    else if (num == 104)
    {
        int count = 1;
        processAddChild(1, OVER, "OVER", --count);
    }
}
*/

void printTreeLL1(TreeNode* t, int layer, bool islastson)
{
    if (t == NULL)
        return;
    int last = t->child.size() - 1;

    for (int i = 0; i < layer; i++)
    {
        outstrLL1[strlineLL1] += "   ";
    }

    if (layer == 0)
    {
        outstrLL1[strlineLL1] += "   ";
        outstrLL1[strlineLL1] += t->name;
        //cout << t->name;
    }
    else
    {
        if (islastson == true)
        {
            outstrLL1[strlineLL1] += "└ ";
            outstrLL1[strlineLL1] += t->name;
            //cout << "└ " << t->name;
        }
        else
        {
            outstrLL1[strlineLL1] += "├ ";
            outstrLL1[strlineLL1] += t->name;
            //cout << "├ " << t->name;
        }
    }

    outstrLL1[strlineLL1] += "\n";
    strlineLL1++;
    //outstr += "\n";
    //cout << endl;

    for (int i = 0; i <= last; i++)
    {
        if (i != last)
            printTreeLL1(t->child[i], layer + 1, false);
        else
            printTreeLL1(t->child[i], layer + 1, true);
    }

    if (layer == 0)
    {
        for (int i = 1; i < strlineLL1; i++)
        {
            int j = 0;
            while (outstrLL1[i][j] != '\n')
            {
                int flg1 = 1;
                int flg2 = 1;
                int flg3 = 1;

                for(int u = i - 1, v = 0;v < ch2.size();v++){
                    if(outstrLL1[u][j + v] != ch2[v]){
                        flg1 = 0;
                    }
                }
                for(int u = i - 1, v = 0;v < ch1.size();v++){
                    if(outstrLL1[u][j + v] != ch1[v]){
                        flg2 = 0;
                    }
                }
                for(int u = i - 1, v = 0;v < ch3.size();v++){
                    if(outstrLL1[u][j + v] != ch3[v]){
                        flg3 = 0;
                    }
                }

                if (outstrLL1[i - 1][j] == '\n')
                    break;
                if (flg1) // 这个很重要 不加的话会出问题
                {
                    ;
                }
                else if (flg2 && outstrLL1[i][j] == ' ')
                {
                    for(int u = 0;u < ch3.size(); ++u){
                        outstrLL1[i][j + u] = ch3[u];
                    }
                    // outstrLL1[i][j] = ch3[0];
                    // outstrLL1[i][j + 1] = ch3[1];
                }
                else if (flg3 && outstrLL1[i][j] == ' ')
                {
                    for(int u = 0;u < ch3.size(); ++u){
                        outstrLL1[i][j + u] = ch3[u];
                    }
                    // outstrLL1[i][j] = ch3[0];
                    // outstrLL1[i][j + 1] = ch3[1];
                }
                j++;
            }
        }

        for (int i = 0; i < strlineLL1; i++)
        {
            cout << outstrLL1[i];
        }
    }
}


/*
void printTreeLL1(TreeNode* t, int layer, bool islastson) {
    if (t == NULL)
        return;
    int last = t->child.size() - 1;

    for (int i = 0; i < layer; i++)
    {
        outstrLL1[strlineLL1] += "   ";
    }

    if (layer == 0)
    {
        outstrLL1[strlineLL1] += "   ";
        outstrLL1[strlineLL1] += t->name;
    }
    else
    {
        if (islastson == true)
        {
            outstrLL1[strlineLL1] += "└ ";
            outstrLL1[strlineLL1] += t->name;

        }
        else
        {
            outstrLL1[strlineLL1] += "├ ";
            outstrLL1[strlineLL1] += t->name;

        }
    }

    outstrLL1[strlineLL1] += "\n";
    strlineLL1++;
    for (int i = 0; i <= last; i++)
    {
        if (i != last)
            printTree(t->child[i], layer + 1, false);
        else
            printTree(t->child[i], layer + 1, true);
    }


    if (layer == 0)
    {

        for (int i = 1; i < strlineLL1; i++)
        {
            int j = 0;
            while (outstrLL1[i][j] != '\n')
            {
                if (outstrLL1[i - 1][j] == '\n')
                    break;
                if (outstrLL1[i - 1][j] == ch2[0] && outstrLL1[i - 1][j + 1] == ch2[1])
                {
                }
                else if (outstrLL1[i - 1][j] == ch1[0] && outstrLL1[i - 1][j + 1] == ch1[1] && outstrLL1[i][j] == ' ')
                {
                    outstrLL1[i][j] = ch3[0];
                    outstrLL1[i][j + 1] = ch3[1];
                }
                else if (outstrLL1[i - 1][j] == ch3[0] && outstrLL1[i][j] == ' ')
                {
                    outstrLL1[i][j] = ch3[0];
                    outstrLL1[i][j + 1] = ch3[1];
                }
                j++;
            }
        }

        for (int i = 0; i <= strlineLL1; i++)
        {
            cout << outstrLL1[i];
        }
    }
}
*/

