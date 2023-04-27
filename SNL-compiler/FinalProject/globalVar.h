#pragma once

#include "global.h"

extern Word reservedWords[21];


extern Token tokenList[1024];// token琛?

extern int Index;
extern string ch1;
extern string ch2;
extern string ch3;

extern AnalysisStack* anlsstack;

extern TreeNode* treeroot;
extern TreeNode* root;

extern int LL1Tbl[104][104]; // LL1

extern TreeNode* currentTree;

extern Token* currentToken;



extern int strline;

extern string outstr[512];

extern int is_newLL1_correct;
