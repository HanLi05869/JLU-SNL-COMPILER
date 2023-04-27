#pragma once
#include "func.h"
#include "global.h"

extern TreeNode* programLL1();


extern void initLL1();

extern void pAddChild(int istmnl, int lex, string name, int childindex);

extern void doSthInLL1(int num);

extern void printErrorMsgLL1(string reason);

extern string outstrLL1[512]; //

extern int strlineLL1;

extern void printTreeLL1(TreeNode* t, int layer, bool islastson);

extern void processAddChild(int istmnl, int lex, string name, int childindex);
