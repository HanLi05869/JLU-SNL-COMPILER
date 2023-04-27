#include "func.h"
#include "global.h"
#include "globalVar.h"
#include "lexical.h"
#include <iostream>

using namespace std;

void print_error(string strn) {
    string str = "ERROR " + strn + ": ";
    str += "Line: ";
    str += to_string(tokenList[Index].line);
    str += " Str:";
    str += tokenList[Index].wd.str;
    cout << str << endl;
    // exit(0);
    return;
}

void readToken()
{
    currentToken = &tokenList[Index];
    Index++;
}

TreeNode* matchToken(LexType tok)
{
    readToken();
    TreeNode* t = nullptr;
    if (currentToken->wd.tok == tok) {
        t = new TreeNode(currentToken->wd.str);
        t->tk = currentToken;
        //cout << std::left << setw(16) << getString(t->tk->wd.tok) << "\t" ;
        //cout<< "m_success" << endl ;
    }
    else {
        print_error("Match");
        //cout << "m_error" << endl << endl;
    }
    return t;
}


TreeNode* program() {
    Index = 0;
    TreeNode* root = nullptr;
    if (tokenList[Index].wd.tok == PROGRAM) {
        root = new TreeNode("Program");
        root->addChild(programHead());
        root->addChild(declarePart());
        root->addChild(programBody());
        root->addChild(matchToken(DOT));
    }
    else { print_error("Program"); }
    return root;
}

TreeNode* programHead() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == PROGRAM) {
        t = new TreeNode("ProgramHead");
        t->addChild(matchToken(PROGRAM));
        t->addChild(programName());
    }
    else { print_error("ProgramHead"); }
    return t;
}

TreeNode* programName() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("ProgramName");
        t->addChild(matchToken(ID));
    }
    else { print_error("ProgramHead"); }
    return t;
}

TreeNode* declarePart() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == TYPE || tokenList[Index].wd.tok == VAR || tokenList[Index].wd.tok == PROCEDURE || tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("DeclarePart");
        t->addChild(typeDecPart());
        t->addChild(varDecPart());
        t->addChild(procDecpart());
    }
    else { print_error("DeclarePart"); }
    return t;
}

TreeNode* typeDecPart() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == VAR || tokenList[Index].wd.tok == PROCEDURE || tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("TypeDecPart");
    }
    else if (tokenList[Index].wd.tok == TYPE) {
        t = new TreeNode("TypeDecPart");
        t->addChild(typeDec());
    }
    else { print_error("TypeDecPart"); }
    return t;
}

TreeNode* typeDec() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == TYPE) {
        t = new TreeNode("TypeDec");
        t->addChild(matchToken(TYPE));
        t->addChild(typeDecList());
    }
    else { print_error("TypeDec"); }
    return t;
}

TreeNode* typeDecList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("TypeDecList");
        t->addChild(typeID());//TypeId
        t->addChild(matchToken(EQ));
        t->addChild(typeDef());
        t->addChild(matchToken(SEMI));
        t->addChild(typeDecMore());
    }
    else { print_error("TypeDecList"); }
    return t;
}

TreeNode* typeID() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("TypeID");
        t->addChild(matchToken(ID));
    }
    else { print_error("TypeID"); }
    return t;
}

TreeNode* typeDecMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == VAR || tokenList[Index].wd.tok == PROCEDURE || tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("TypeDecMore");
    }
    else if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("TypeDecMore");
        t->addChild(typeDecList());
    }
    else { print_error("TypeDecMore"); }
    return t;
}

TreeNode* typeDef() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR) {
        t = new TreeNode("TypeDef");
        t->addChild(baseType());
    }
    else if (tokenList[Index].wd.tok == ARRAY || tokenList[Index].wd.tok == RECORD) {
        t = new TreeNode("TypeDef");
        t->addChild(structureType());
    }
    else if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("TypeDef");
        t->addChild(matchToken(ID));
    }
    else { print_error("TypeDef"); }
    return t;
}

TreeNode* baseType() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTEGER) {
        t = new TreeNode("BaseType");
        t->addChild(matchToken(INTEGER));
    }
    else if (tokenList[Index].wd.tok == CHAR) {
        t = new TreeNode("BaseType");
        t->addChild(matchToken(CHAR));
    }
    else { print_error("BaseType"); }
    return t;
}

TreeNode* structureType() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ARRAY) {
        t = new TreeNode("StructureType");
        t->addChild(arrayType());
    }
    else if (tokenList[Index].wd.tok == RECORD) {
        t = new TreeNode("StructureType");
        t->addChild(recType());
    }
    else { print_error("StructureType"); }
    return t;
}


TreeNode* arrayType() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ARRAY) {
        t = new TreeNode("ArrayType");
        t->addChild(matchToken(ARRAY));
        t->addChild(matchToken(LMIDPAREN));
        t->addChild(low());
        t->addChild(matchToken(UNDERANGE));
        t->addChild(top());
        t->addChild(matchToken(RMIDPAREN));
        t->addChild(matchToken(OF));
        t->addChild(baseType());
    }
    else { print_error("ArrayType"); }
    return t;
}

TreeNode* low() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTC) {
        t = new TreeNode("Low");
        t->addChild(matchToken(INTC));
    }
    else { print_error("Low"); }
    return t;
}

TreeNode* top() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTC) {
        t = new TreeNode("Top");
        t->addChild(matchToken(INTC));
    }
    else { print_error("Top"); }
    return t;
}

TreeNode* recType() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == RECORD) {
        t = new TreeNode("RecType");
        t->addChild(matchToken(RECORD));
        t->addChild(fieldDecList());
        t->addChild(matchToken(END1));
    }
    else { print_error("RecType"); }
    return t;
}

TreeNode* fieldDecList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR) {
        t = new TreeNode("FieldDecList");
        t->addChild(baseType());
        t->addChild(IDList());
        t->addChild(matchToken(SEMI));
        t->addChild(fieldDecMore());
    }
    else if (tokenList[Index].wd.tok == ARRAY) {
        t = new TreeNode("FieldDecList");
        t->addChild(arrayType());
        t->addChild(IDList());
        t->addChild(matchToken(SEMI));
        t->addChild(fieldDecMore());
    }
    else { print_error("FieldDecList"); }
    return t;
}

TreeNode* fieldDecMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == END1) {
        t = new TreeNode("FieldDecMore");
    }
    else if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR || tokenList[Index].wd.tok == ARRAY) {
        t = new TreeNode("FieldDecMore");
        t->addChild(fieldDecList());
    }
    else { print_error("FieldDecMore"); }
    return t;
}

TreeNode* IDList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("IDList");
        t->addChild(matchToken(ID));
        t->addChild(IDMore());
    }
    else { print_error("IDList"); }
    return t;
}

TreeNode* IDMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == SEMI) {
        t = new TreeNode("IDMore");
    }
    else if (tokenList[Index].wd.tok == COMMA) {
        t = new TreeNode("IDMore");
        t->addChild(matchToken(COMMA));
        t->addChild(IDList());
    }
    else { print_error("IDMore"); }
    return t;
}

TreeNode* varDecPart() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == PROCEDURE || tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("VarDecPart");
    }
    else if (tokenList[Index].wd.tok == VAR) {
        t = new TreeNode("VarDecPart");
        t->addChild(varDec());
    }
    else { print_error("VarDecPart"); }
    return t;
}

TreeNode* varDec() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == VAR) {
        t = new TreeNode("VarDec");
        t->addChild(matchToken(VAR));
        t->addChild(varDecList());
    }
    else { print_error("VarDec"); }
    return t;
}

TreeNode* varDecList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR || tokenList[Index].wd.tok == ARRAY ||
        tokenList[Index].wd.tok == RECORD || tokenList[Index].wd.tok == ID) {
        t = new TreeNode("VarDecList");
        t->addChild(typeDef());
        t->addChild(varIDList());
        t->addChild(matchToken(SEMI));
        t->addChild(varDecMore());
    }
    else { print_error("VarDecList"); }
    return t;
}

TreeNode* varDecMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == PROCEDURE || tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("VarDecMore");
    }
    else if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR || tokenList[Index].wd.tok == ARRAY ||
        tokenList[Index].wd.tok == RECORD || tokenList[Index].wd.tok == ID) {
        t = new TreeNode("VarDecMore");
        t->addChild(varDecList());
    }
    else { print_error("VarDecMore"); }
    return t;
}

TreeNode* varIDList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("VarIDList");
        t->addChild(matchToken(ID));
        t->addChild(varIDMore());
    }
    else { print_error("VarIDList"); }
    return t;
}

TreeNode* varIDMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == SEMI) {
        t = new TreeNode("VarIDMore");
    }
    else if (tokenList[Index].wd.tok == COMMA) {
        t = new TreeNode("VarIDMore");
        t->addChild(matchToken(COMMA));
        t->addChild(varIDList());
    }
    else { print_error("VarIDMore"); }
    return t;
}

TreeNode* procDecpart() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("ProcDecpart");
    }
    else if (tokenList[Index].wd.tok == PROCEDURE) {
        t = new TreeNode("ProcDecpart");
        t->addChild(procDec());
    }
    else { print_error("ProcDecpart"); }
    return t;
}

TreeNode* procDec() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == PROCEDURE) {
        t = new TreeNode("ProcDec");
        t->addChild(matchToken(PROCEDURE));
        t->addChild(procName());
        t->addChild(matchToken(LPAREN));
        t->addChild(paramList());
        t->addChild(matchToken(RPAREN));
        t->addChild(matchToken(SEMI));
        t->addChild(procDecPart());
        t->addChild(procBody());
        t->addChild(procDecpart());
    }
    else { print_error("ProcDec"); }
    return t;
}

TreeNode* procName() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("ProcName");
        t->addChild(matchToken(ID));
    }
    else { print_error("ProcName"); }
    return t;
}

TreeNode* paramList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == RPAREN) {
        t = new TreeNode("ParamList");
    }
    else if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR || tokenList[Index].wd.tok == ARRAY ||
        tokenList[Index].wd.tok == RECORD || tokenList[Index].wd.tok == ID || tokenList[Index].wd.tok == VAR) {
        t = new TreeNode("ParamList");
        t->addChild(paramDecList());
    }
    else { print_error("ParamList"); }
    return t;
}

TreeNode* paramDecList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR || tokenList[Index].wd.tok == ARRAY ||
        tokenList[Index].wd.tok == RECORD || tokenList[Index].wd.tok == ID || tokenList[Index].wd.tok == VAR) {
        t = new TreeNode("ParamDecList");
        t->addChild(param());
        t->addChild(paramMore());
    }
    else { print_error("ParamDecList"); }
    return t;
}

TreeNode* paramMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == RPAREN) {
        t = new TreeNode("ParamMore");
    }
    else if (tokenList[Index].wd.tok == SEMI) {
        t = new TreeNode("ParamMore");
        t->addChild(matchToken(SEMI));
        t->addChild(paramDecList());
    }
    else { print_error("ParamMore"); }
    return t;
}

TreeNode* param() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == INTEGER || tokenList[Index].wd.tok == CHAR || tokenList[Index].wd.tok == ARRAY ||
        tokenList[Index].wd.tok == RECORD || tokenList[Index].wd.tok == ID) {
        t = new TreeNode("Param");
        t->addChild(typeDef());
        t->addChild(formList());
    }
    else if (tokenList[Index].wd.tok == VAR) {
        t = new TreeNode("Param");
        t->addChild(matchToken(VAR));
        t->addChild(typeDef());
        t->addChild(formList());
    }
    else { print_error("Param"); }
    return t;
}

TreeNode* formList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("FormList");
        t->addChild(matchToken(ID));
        t->addChild(fidMore());
    }
    else { print_error("FormList"); }
    return t;
}

TreeNode* fidMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == SEMI || tokenList[Index].wd.tok == RPAREN) {
        t = new TreeNode("FidMore");
    }
    else if (tokenList[Index].wd.tok == COMMA) {
        t = new TreeNode("FidMore");
        t->addChild(matchToken(COMMA));
        t->addChild(formList());
    }
    else { print_error("FidMore"); }
    return t;
}

TreeNode* procDecPart() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == TYPE || tokenList[Index].wd.tok == VAR || tokenList[Index].wd.tok == PROCEDURE ||
        tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("ProDecPart");
        t->addChild(declarePart());
    }
    else { print_error("ProDecPart"); }
    return t;
}

TreeNode* procBody() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("ProcBody");
        t->addChild(programBody());
    }
    else { print_error("ProcBody"); }
    return t;
}

TreeNode* programBody() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == BEGIN) {
        t = new TreeNode("ProgramBody");
        t->addChild(matchToken(BEGIN));
        t->addChild(stmList());
        t->addChild(matchToken(END1));
    }
    else { print_error("ProgramBody"); }
    return t;
}

TreeNode* stmList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID || tokenList[Index].wd.tok == IF || tokenList[Index].wd.tok == WHILE ||
        tokenList[Index].wd.tok == RETURN1 || tokenList[Index].wd.tok == READ || tokenList[Index].wd.tok == WRITE) {
        t = new TreeNode("StmList");
        t->addChild(stm());
        t->addChild(stmMore());
    }
    else { print_error("StmList"); }
    return t;
}

TreeNode* stmMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ELSE || tokenList[Index].wd.tok == FI || tokenList[Index].wd.tok == END1 ||
        tokenList[Index].wd.tok == ENDWH) {
        t = new TreeNode("StmMore");
    }
    else if (tokenList[Index].wd.tok == SEMI) {
        t = new TreeNode("StmMore");
        t->addChild(matchToken(SEMI));
        t->addChild(stmList());
    }
    else { print_error("StmMore"); }
    return t;
}

TreeNode* stm() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == IF) {
        t = new TreeNode("Stm");
        t->addChild(conditionalStm());
    }
    else if (tokenList[Index].wd.tok == WHILE) {
        t = new TreeNode("Stm");
        t->addChild(loopStm());
    }
    else if (tokenList[Index].wd.tok == READ) {
        t = new TreeNode("Stm");
        t->addChild(inputStm());
    }
    else if (tokenList[Index].wd.tok == WRITE) {
        t = new TreeNode("Stm");
        t->addChild(outputStm());
    }
    else if (tokenList[Index].wd.tok == RETURN1) {
        t = new TreeNode("Stm");
        t->addChild(returnStm());
    }
    else if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("Stm");
        t->addChild(matchToken(ID));
        t->addChild(assCall());
    }
    else { print_error("Stm"); }
    return t;
}

TreeNode* assCall() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ASSIGN || tokenList[Index].wd.tok == LMIDPAREN
        || tokenList[Index].wd.tok == DOT) {
        t = new TreeNode("AssCall");
        t->addChild(assignmentRest());
    }
    else if (tokenList[Index].wd.tok == LPAREN) {
        t = new TreeNode("AssCall");
        t->addChild(callStmRest());
    }
    else { print_error("AssCall"); }
    return t;
}

TreeNode* assignmentRest() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LMIDPAREN || tokenList[Index].wd.tok == DOT || tokenList[Index].wd.tok == ASSIGN) {
        t = new TreeNode("AssignmentRest");
        t->addChild(variMore());
        t->addChild(matchToken(ASSIGN));
        t->addChild(exp());
    }
    else { print_error("AssignmentRest"); }
    return t;
}

TreeNode* conditionalStm() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == IF) {
        t = new TreeNode("ConditionalStm");
        t->addChild(matchToken(IF));
        t->addChild(relExp());
        t->addChild(matchToken(THEN));
        t->addChild(stmList());
        t->addChild(matchToken(ELSE));
        t->addChild(stmList());
        t->addChild(matchToken(FI));
    }
    else { print_error("ConditionalStm"); }
    return t;
}

TreeNode* loopStm() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == WHILE) {
        t = new TreeNode("LoopStm");
        t->addChild(matchToken(WHILE));
        t->addChild(relExp());
        t->addChild(matchToken(DO));
        t->addChild(stmList());
        t->addChild(matchToken(ENDWH));
    }
    else { print_error("LoopStm"); }
    return t;
}

TreeNode* inputStm() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == READ) {
        t = new TreeNode("InputStm");
        t->addChild(matchToken(READ));
        t->addChild(matchToken(LPAREN));
        t->addChild(inVar());
        t->addChild(matchToken(RPAREN));
    }
    else { print_error("InputStm"); }
    return t;
}

TreeNode* inVar() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("InVar");
        t->addChild(matchToken(ID));
    }
    else { print_error("InVar"); }
    return t;
}

TreeNode* outputStm() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == WRITE) {
        t = new TreeNode("OutputStm");
        t->addChild(matchToken(WRITE));
        t->addChild(matchToken(LPAREN));
        t->addChild(exp());
        t->addChild(matchToken(RPAREN));
    }
    else { print_error("OutputStm"); }
    return t;
}

TreeNode* returnStm() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == RETURN1) {
        t = new TreeNode("ReturnStm");
        t->addChild(matchToken(RETURN1));
    }
    else { print_error("ReturnStm"); }
    return t;
}

TreeNode* callStmRest() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LPAREN) {
        t = new TreeNode("CallStmRest");
        t->addChild(matchToken(LPAREN));
        t->addChild(actparamList());
        t->addChild(matchToken(RPAREN));
    }
    else { print_error("CallStmRest"); }
    return t;
}

TreeNode* actparamList() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == RPAREN) {
        t = new TreeNode("ActparamList");
    }
    else if (tokenList[Index].wd.tok == LPAREN || tokenList[Index].wd.tok == INTC || tokenList[Index].wd.tok == ID) {
        t = new TreeNode("ActparamList");
        t->addChild(exp());
        t->addChild(actparamMore());
    }
    else { print_error("ActparamList"); }
    return t;
}

TreeNode* actparamMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == RPAREN) {
        t = new TreeNode("ActparamMore");
    }
    else if (tokenList[Index].wd.tok == COMMA) {
        t = new TreeNode("ActparamMore");
        t->addChild(matchToken(COMMA));
        t->addChild(actparamList());
    }
    else { print_error("ActparamMore"); }
    return t;
}

TreeNode* relExp() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LPAREN || tokenList[Index].wd.tok == INTC || tokenList[Index].wd.tok == ID) {
        t = new TreeNode("RelExp");
        t->addChild(exp());
        t->addChild(otherRelE());
    }
    else { print_error("RelExp"); }
    return t;
}

TreeNode* otherRelE() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LT || tokenList[Index].wd.tok == EQ) {
        t = new TreeNode("OtherRelE");
        t->addChild(cmpOp());
        t->addChild(exp());
    }
    else { print_error("OtherRelE"); }
    return t;
}

TreeNode* exp() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LPAREN || tokenList[Index].wd.tok == INTC || tokenList[Index].wd.tok == ID) {
        t = new TreeNode("Exp");
        t->addChild(term());
        t->addChild(otherTerm());
    }
    else { print_error("Exp"); }
    return t;
}

TreeNode* otherTerm() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LT || tokenList[Index].wd.tok == EQ || tokenList[Index].wd.tok == RMIDPAREN ||
        tokenList[Index].wd.tok == THEN || tokenList[Index].wd.tok == ELSE || tokenList[Index].wd.tok == FI ||
        tokenList[Index].wd.tok == DO || tokenList[Index].wd.tok == ENDWH || tokenList[Index].wd.tok == RPAREN ||
        tokenList[Index].wd.tok == END1 || tokenList[Index].wd.tok == SEMI || tokenList[Index].wd.tok == COMMA) {
        t = new TreeNode("OtherTerm");
    }
    else if (tokenList[Index].wd.tok == PLUS || tokenList[Index].wd.tok == MINUS) {
        t = new TreeNode("OtherTerm");
        t->addChild(addOp());
        t->addChild(exp());
    }
    else { print_error("OtherTerm"); }
    return t;
}

TreeNode* term() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LPAREN || tokenList[Index].wd.tok == INTC || tokenList[Index].wd.tok == ID) {
        t = new TreeNode("Term");
        t->addChild(factor());
        t->addChild(otherFactor());
    }
    else { print_error("Term"); }
    return t;
}

TreeNode* otherFactor() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == PLUS || tokenList[Index].wd.tok == MINUS || tokenList[Index].wd.tok == LT ||
        tokenList[Index].wd.tok == EQ || tokenList[Index].wd.tok == RMIDPAREN || tokenList[Index].wd.tok == THEN ||
        tokenList[Index].wd.tok == ELSE || tokenList[Index].wd.tok == FI || tokenList[Index].wd.tok == DO ||
        tokenList[Index].wd.tok == ENDWH || tokenList[Index].wd.tok == RPAREN || tokenList[Index].wd.tok == END1 ||
        tokenList[Index].wd.tok == SEMI || tokenList[Index].wd.tok == COMMA) {
        t = new TreeNode("OtherFactor");
    }
    else if (tokenList[Index].wd.tok == TIMES || tokenList[Index].wd.tok == OVER) {
        t = new TreeNode("OtherFactor");
        t->addChild(multOp());
        t->addChild(term());
    }
    else { print_error("OtherFactor"); }
    return t;
}

TreeNode* factor() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LPAREN) {
        t = new TreeNode("Factor");
        t->addChild(matchToken(LPAREN));
        t->addChild(exp());
        t->addChild(matchToken(RPAREN));
    }
    else if (tokenList[Index].wd.tok == INTC) {
        t = new TreeNode("Factor");
        t->addChild(matchToken(INTC));
    }
    else if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("Factor");
        t->addChild(variable());
    }
    else { print_error("Factor"); }
    return t;
}

TreeNode* variable() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("Variable");
        t->addChild(matchToken(ID));
        t->addChild(variMore());
    }
    else { print_error("Variable"); }
    return t;
}

TreeNode* variMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ASSIGN || tokenList[Index].wd.tok == TIMES || tokenList[Index].wd.tok == OVER ||
        tokenList[Index].wd.tok == PLUS || tokenList[Index].wd.tok == MINUS || tokenList[Index].wd.tok == LT ||
        tokenList[Index].wd.tok == EQ || tokenList[Index].wd.tok == THEN || tokenList[Index].wd.tok == ELSE ||
        tokenList[Index].wd.tok == FI || tokenList[Index].wd.tok == DO || tokenList[Index].wd.tok == ENDWH ||
        tokenList[Index].wd.tok == RPAREN || tokenList[Index].wd.tok == END1 || tokenList[Index].wd.tok == SEMI ||
        tokenList[Index].wd.tok == COMMA || tokenList[Index].wd.tok == RMIDPAREN) {
        t = new TreeNode("VariMore");

    }
    else if (tokenList[Index].wd.tok == LMIDPAREN) {
        t = new TreeNode("VariMore");
        t->addChild(matchToken(LMIDPAREN));
        t->addChild(exp());
        t->addChild(matchToken(RMIDPAREN));
    }
    else if (tokenList[Index].wd.tok == DOT) {
        t = new TreeNode("VariMore");
        t->addChild(matchToken(DOT));
        t->addChild(fieldVar());
    }
    else { print_error("VariMore"); }
    return t;
}

TreeNode* fieldVar() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ID) {
        t = new TreeNode("FieldVar");
        t->addChild(matchToken(ID));
        t->addChild(fieldVarMore());
    }
    else { print_error("FieldVar"); }
    return t;
}


TreeNode* fieldVarMore() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == ASSIGN || tokenList[Index].wd.tok == TIMES || tokenList[Index].wd.tok == OVER ||
        tokenList[Index].wd.tok == PLUS || tokenList[Index].wd.tok == MINUS || tokenList[Index].wd.tok == LT ||
        tokenList[Index].wd.tok == EQ || tokenList[Index].wd.tok == THEN || tokenList[Index].wd.tok == ELSE ||
        tokenList[Index].wd.tok == FI || tokenList[Index].wd.tok == DO || tokenList[Index].wd.tok == ENDWH ||
        tokenList[Index].wd.tok == RPAREN || tokenList[Index].wd.tok == END1 || tokenList[Index].wd.tok == SEMI ||
        tokenList[Index].wd.tok == COMMA) {
        t = new TreeNode("FieldVarMore");
    }
    else if (tokenList[Index].wd.tok == LMIDPAREN) {
        t = new TreeNode("FieldVarMore");
        t->addChild(matchToken(LMIDPAREN));
        t->addChild(exp());
        t->addChild(matchToken(RMIDPAREN));
    }
    else { print_error("FieldVarMore"); }
    return t;
}

TreeNode* cmpOp() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == LT) {
        t = new TreeNode("CmpOP");
        t->addChild(matchToken(LT));
    }
    else if (tokenList[Index].wd.tok == EQ) {
        t = new TreeNode("CmpOP");
        t->addChild(matchToken(EQ));
    }
    else { print_error("CmpOP"); }
    return t;
}

TreeNode* addOp() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == PLUS) {
        t = new TreeNode("AddOP");
        t->addChild(matchToken(PLUS));
    }
    else if (tokenList[Index].wd.tok == MINUS) {
        t = new TreeNode("AddOP");
        t->addChild(matchToken(MINUS));
    }
    else { print_error("AddOP"); }
    return t;
}

TreeNode* multOp() {
    TreeNode* t = nullptr;
    if (tokenList[Index].wd.tok == TIMES) {
        t = new TreeNode("MultOP");
        t->addChild(matchToken(TIMES));
    }
    else if (tokenList[Index].wd.tok == OVER) {
        t = new TreeNode("MultOP");
        t->addChild(matchToken(OVER));
    }
    else { print_error("MultOP"); }
    return t;
}



void printTree(TreeNode* t, int layer, bool islastson) {
    if (t == NULL)
        return;
    int last = t->child.size() - 1;

    for (int i = 0; i < layer; i++)
    {
        outstr[strline] += "   ";
    }

    if (layer == 0)
    {
        outstr[strline] += "   ";
        outstr[strline] += t->name;
    }
    else
    {
        if (islastson == true)
        {
            outstr[strline] += "└ ";
            outstr[strline] += t->name;

        }
        else
        {
            outstr[strline] += "├ ";
            outstr[strline] += t->name;

        }
    }

    outstr[strline] += "\n";
    strline++;
    for (int i = 0; i <= last; i++)
    {
        if (i != last)
            printTree(t->child[i], layer + 1, false);
        else
            printTree(t->child[i], layer + 1, true);
    }


    if (layer == 0)
    {

        for (int i = 1; i < strline; i++)
        {
            int j = 0;
            while (outstr[i][j] != '\n')
            {
                if (outstr[i - 1][j] == '\n')
                    break;

                int flg1 = 1;
                int flg2 = 1;
                int flg3 = 1;

                for(int u = i - 1, v = 0;v < ch2.size();v++){
                    if(outstr[u][j + v] != ch2[v]){
                        flg1 = 0;
                    }
                }
                for(int u = i - 1, v = 0;v < ch1.size();v++){
                    if(outstr[u][j + v] != ch1[v]){
                        flg2 = 0;
                    }
                }
                for(int u = i - 1, v = 0;v < ch3.size();v++){
                    if(outstr[u][j + v] != ch3[v]){
                        flg3 = 0;
                    }
                }



                if (flg1)
                {
                }
                else if (flg2 && outstr[i][j] == ' ')
                {
                    for(int u = 0;u < ch3.size(); ++u){
                        outstr[i][j + u] = ch3[u];
                    }
                    // outstr[i][j] = ch3[0];
                    // outstr[i][j + 1] = ch3[1];
                }
                else if (flg3 && outstr[i][j] == ' ')
                {
                    for(int u = 0;u < ch3.size(); ++u){
                        outstr[i][j + u] = ch3[u];
                    }
                    // outstr[i][j] = ch3[0];
                    // outstr[i][j + 1] = ch3[1];
                }
                j++;
            }
        }

        for (int i = 0; i <= strline; i++)
        {
            cout << outstr[i];
        }
    }
}
