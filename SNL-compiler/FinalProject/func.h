#pragma once
#include "global.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

extern TreeNode* programLL1();

extern void pAddChild(int, int, string, int);

extern void doSthInLL1(int);

extern void initLL1();  // init LL1

extern void printErrorMsgLL1(string);




extern TreeNode* matchToken(LexType tok);
extern TreeNode* program();//
extern TreeNode* programHead();
extern TreeNode* programName();
extern TreeNode* declarePart();
extern TreeNode* programBody();
extern TreeNode* typeDecPart();
extern TreeNode* varDecPart();
extern TreeNode* procDecpart();
extern TreeNode* typeDec();
extern TreeNode* typeDecList();
extern TreeNode* typeDef();
extern TreeNode* typeDecMore();
extern TreeNode* baseType();
extern TreeNode* structureType();
extern TreeNode* arrayType();
extern TreeNode* recType();
extern TreeNode* fieldDecList();
extern TreeNode* IDList();
extern TreeNode* fieldDecMore();
extern TreeNode* IDMore();
extern TreeNode* varDec();
extern TreeNode* varDecList();
extern TreeNode* varIDList();
extern TreeNode* varDecMore();
extern TreeNode* varIDMore();
extern TreeNode* procDec();
extern TreeNode* paramList();
extern TreeNode* procDecPart();
extern TreeNode* procBody();
extern TreeNode* paramDecList();
extern TreeNode* param();
extern TreeNode* paramMore();
extern TreeNode* formList();
extern TreeNode* fidMore();
extern TreeNode* stmList();
extern TreeNode* stm();
extern TreeNode* stmMore();
extern TreeNode* conditionalStm();
extern TreeNode* loopStm();
extern TreeNode* inputStm();
extern TreeNode* outputStm();
extern TreeNode* returnStm();
extern TreeNode* assCall();
extern TreeNode* assignmentRest();
extern TreeNode* callStmRest();
extern TreeNode* variMore();
extern TreeNode* exp();
extern TreeNode* relExp();
extern TreeNode* otherRelE();
extern TreeNode* cmpOp();
extern TreeNode* actparamList();
extern TreeNode* actparamMore();
extern TreeNode* term();
extern TreeNode* otherTerm();
extern TreeNode* addOp();
extern TreeNode* factor();
extern TreeNode* otherFactor();
extern TreeNode* multOp();
extern TreeNode* variable();
extern TreeNode* fieldVar();
extern TreeNode* fieldVarMore();
extern TreeNode* typeID();
extern TreeNode* low();
extern TreeNode* top();
extern TreeNode* procName();
extern TreeNode* inVar();

extern void printTree(TreeNode* t, int layer, bool islastson);
