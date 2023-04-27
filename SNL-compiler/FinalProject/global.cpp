#include "global.h"
#include <iostream>

using namespace std;

Word reservedWords[21] =
{
    {"program",PROGRAM},
    {"type",TYPE},
    {"var",VAR},
    {"procedure",PROCEDURE},
    {"begin",BEGIN},
    {"end",END1},
    {"array",ARRAY},
    {"of",OF},
    {"record",RECORD},
    {"if",IF},{"then",THEN},
    {"else",ELSE},
    {"fi",FI},
    {"while",WHILE},
    {"do",DO},
    {"endwh",ENDWH},
    {"read",READ},
    {"write",WRITE},
    {"return",RETURN1},
    {"integer",INTEGER},
    {"char",CHAR}
};


Token tokenList[1024];// token琛?
int Index = 0;
string ch1 = "├";
string ch2 = "└ ";
string ch3 = "│";

AnalysisStack* anlsstack = new AnalysisStack();

TreeNode* treeroot = nullptr;

int LL1Tbl[104][104]; // LL1

TreeNode* currentTree;

Token* currentToken;


int strline = 0;

string outstr[51200];


// LL1
string outstrLL1[51200];
int strlineLL1 = 0;

string outstrLL1new[51200];
int strlineLL1new = 0;
int is_newLL1_correct = 1;


jmp_buf jump_buffer;
