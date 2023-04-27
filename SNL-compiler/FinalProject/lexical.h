#pragma once
#include "global.h"
#include <iostream>
#include <string>
using namespace std;
enum StateType { s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13 };

extern string getString(int lexNum);

extern bool isReserved(string ch);

extern bool isLetter(char ch);

extern bool isDigit(char ch);

extern bool isOperator(char ch);

extern Word getReserved(string s);

extern bool isBlank(char ch);

extern int ischar(char ch);

extern void lexicalAnalyse(FILE* fp);

extern void printTokenList();

extern void readToken();

extern int lexical_correct;
