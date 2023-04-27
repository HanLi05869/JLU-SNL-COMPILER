#include "func.h"
#include "global.h"
#include "lexical.h"
#include "globalVar.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

int lexical_correct = 1;

string getString(int lexNum)
{
    if (lexNum == 0)
        return "ENDFILE";
    else if (lexNum == 1)
        return "ERROR";
    /*reserved word*/
    else if (lexNum == 2)
        return "PROGRAM";
    else if (lexNum == 3)
        return "PROCEDURE";
    else if (lexNum == 4)
        return "TYPE";
    else if (lexNum == 5)
        return "VAR";
    else if (lexNum == 6)
        return "IF";
    else if (lexNum == 7)
        return "THEN";
    else if (lexNum == 8)
        return "ELSE";
    else if (lexNum == 9)
        return "FI";
    else if (lexNum == 10)
        return "WHILE";
    else if (lexNum == 11)
        return "DO";
    else if (lexNum == 12)
        return "ENDWH";
    else if (lexNum == 13)
        return "BEGIN";
    else if (lexNum == 14)
        return "END";
    else if (lexNum == 15)
        return "READ";
    else if (lexNum == 16)
        return "WRITE";
    else if (lexNum == 17)
        return "ARRAY";
    else if (lexNum == 18)
        return "OF";
    else if (lexNum == 19)
        return "RECORD";
    else if (lexNum == 20)
        return "RETURN";
    else if (lexNum == 21)
        return "INTEGER";
    else if (lexNum == 22)
        return "CHAR";
    /*multi char*/
    else if (lexNum == 23)
        return "ID";
    else if (lexNum == 24)
        return "INTC";
    else if (lexNum == 25)
        return "CHARC";
    /*special*/
    else if (lexNum == 26)
        return "ASSIGN";
    else if (lexNum == 27)
        return "EQ";
    else if (lexNum == 28)
        return "LT";
    else if (lexNum == 29)
        return "PLUS";
    else if (lexNum == 30)
        return "MINUS";
    else if (lexNum == 31)
        return "TIMES";
    else if (lexNum == 32)
        return "OVER";
    else if (lexNum == 33)
        return "LPAREN";
    else if (lexNum == 34)
        return "RPAREN";
    else if (lexNum == 35)
        return "DOT";
    else if (lexNum == 36)
        return "COLON";
    else if (lexNum == 37)
        return "SEMI";
    else if (lexNum == 38)
        return "COMMA";
    else if (lexNum == 39)
        return "LMIDPAREN";
    else if (lexNum == 40)
        return "RMIDPAREN";
    else if (lexNum == 41)
        return "UNDERANGE";
}


// 检查是不是保留字
bool isReserved(string ch)
{
    for (int i = 0; i < 21; i++)
        if (ch == reservedWords[i].str)
            return true;
    return false;
}

// letter check
// 检查是不是字母
bool isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    else
        return false;
}

// digit check
// 检查是不是数字
bool isDigit(char ch)
{
    if (ch >= '0' && ch <= '9')
        return true;
    else
        return false;
}

// operator check
// 检查是不是单分界符
bool isOperator(char ch)
{
    if (ch == '+')
        return true;
    else if (ch == '-')
        return true;
    else if (ch == '*')
        return true;
    else if (ch == '/')
        return true;
    else if (ch == '<')
        return true;
    else if (ch == '=')
        return true;
    else if (ch == '(')
        return true;
    else if (ch == ')')
        return true;
    else if (ch == ';')
        return true;
    else if (ch == '[')
        return true;
    else if (ch == ']')
        return true;
    else if (ch == ',')
        return true;
    else
        return false;
}
//得到保留字数据
Word getReserved(string s)
{
    for (int i = 0; i < 21; i++)
    {
        if (reservedWords[i].str == s)
            return reservedWords[i];
    }
}

// 检查是不是 blank
// 空格 制表符 回车 换行
bool isBlank(char ch)
{

    if (ch == ' ') // 空格
        return true;
    else if (ch == '\n') // 换行
        return true;
    else if (ch == '\t') // 鍒惰〃绗?
        return true;
    else if (ch == '\r') // 鍥炶溅
        return true;
    else
        return false;
}


// 检查是哪一类跳转
int ischar(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        return 0;
    }
    if (ch >= '0' && ch <= '9') {
        return 1;
    }
    if (isOperator(ch)) {
        return 2;
    }
    if (ch == ':') {
        return 3;
    }
    if (ch == '{') {
        return 4;
    }
    if (ch == '.') {
        return 5;
    }
    if (ch == '\'') {
        return 6;
    }
    if (isBlank(ch)) {
        return 7;
    }
    return 8;
}


void lexicalAnalyse(FILE* fp)
{
    int lineNum = 1; // line number, start from 1 // 记录行号 在后面写token的时候进行记录
    int index = 0; // tokenList array index // 用于token的记录
    char ch = fgetc(fp); // f get char // 初始化 取出第一个char
    string record = ""; // 当收集到char 存入string record
    // 当遇到 end of file 时暂停
    enum StateType state;
    state = s0;
    while (ch != EOF)
    {
        switch (state) {

        case s0:
        {record = "";
        switch (ischar(ch)) {
        case 0:state = s1; break;	//标识符状态
        case 1:state = s2; break;  //数字状态
        case 2:state = s3; break;  //单分界符状态
        case 3:state = s4; break;	//准双分界符符状态
        case 4:state = s6; break;  //注释状态
        case 5:state = s7; break;  //准数组状态
        case 6:state = s9; break;  //准字符状态
        case 7:state = s12; break; //空格，换行
        case 8:state = s13; break;  //出错状态
        }}break;

        case s1:

        {if (isLetter(ch) || isDigit(ch)) {
            record = record + ch;
            ch = fgetc(fp);
        }
        else {
            if (isReserved(record)) // 检查是ID 还是保留字
            {
                // 是保留字
                tokenList[index].line = lineNum; // 记录行号
                tokenList[index].wd.str = getReserved(record).str; // 记录字符串
                tokenList[index].wd.tok = getReserved(record).tok; // 记录token
                index++;
            }
            else
            {
                // 是ID
                tokenList[index].line = lineNum; // 记录行号
                tokenList[index].wd.str = record; // 记录字符串
                tokenList[index].wd.tok = ID; // 记录token
                index++;
            }
            state = s0;
        }break;

        case s2:

            if (isDigit(ch) || isLetter(ch)) {
                record = record + ch;
                ch = fgetc(fp);
            }
            else {
                int flag = 1; // all digit? 目前的版本中 只允许全部为数字的
                for (int i = 0; i < record.length(); i++)
                {
                    if (isLetter(record[i])) // 只要有字母 就退出
                    {
                        flag = 0;
                        tokenList[index].wd.tok = ERROR;
                        break;
                    }
                }
                /*
                    INNUM数字状态
                */
                if (flag == 1) // 全部为数字 记作INTC
                    tokenList[index].wd.tok = INTC;

                tokenList[index].line = lineNum;
                tokenList[index].wd.str = record;
                index++;
                state = s0;
            }}break;


        case s3:

        {string temp = ""; // char转string
        temp = temp + ch;

        if (temp == "+")
            tokenList[index].wd.tok = PLUS;
        else if (temp == "-")
            tokenList[index].wd.tok = MINUS;
        else if (temp == "*")
            tokenList[index].wd.tok = TIMES;
        else if (temp == "/")
            tokenList[index].wd.tok = OVER;
        else if (temp == "<")
            tokenList[index].wd.tok = LT;
        else if (temp == "=")
            tokenList[index].wd.tok = EQ;
        else if (temp == "(")
            tokenList[index].wd.tok = LPAREN;
        else if (temp == ")")
            tokenList[index].wd.tok = RPAREN;
        else if (temp == "[")
            tokenList[index].wd.tok = LMIDPAREN;
        else if (temp == "]")
            tokenList[index].wd.tok = RMIDPAREN;
        else if (temp == ";")
            tokenList[index].wd.tok = SEMI;
        else if (temp == ",")
            tokenList[index].wd.tok = COMMA;
        tokenList[index].line = lineNum;
        tokenList[index].wd.str = temp;

        index++;

        state = s0;
        ch = fgetc(fp); }
        break;

        case s4:

        {record += ch;
        if ((ch = fgetc(fp)) == '=')
        {
            /*
                INASSIGN赋值状态
            */
            record += ch;
            tokenList[index].line = lineNum;
            tokenList[index].wd.str = record;
            tokenList[index].wd.tok = ASSIGN;
            index++;
            ch = fgetc(fp);
        }
        else
        {
            tokenList[index].line = lineNum;
            tokenList[index].wd.str = record;
            tokenList[index].wd.tok = COLON;
            index++;

        }
        state = s0; }
        break;

        case s6:
        {if (ch != '}') {
            if (ch == '\n') {
                lineNum += 1;
            }
        }
        else {
            state = s0;
        }
        ch = fgetc(fp); }
        break;
        case s7:
        {record += ch;
        if ((ch = fgetc(fp)) == '.') // 记录'..'
        {
            /*
                INRANGE数组下标界限状态
            */
            record += ch;
            tokenList[index].line = lineNum;
            tokenList[index].wd.str = record;
            tokenList[index].wd.tok = UNDERANGE;
            index++;
            ch = fgetc(fp);
        }
        else // 记录'.'
        {
            tokenList[index].line = lineNum;
            tokenList[index].wd.str = record;
            tokenList[index].wd.tok = DOT;
            index++;
        }
        state = s0; }
        break;
        case 9:
        {ch = fgetc(fp);
        if (isLetter(ch) || isDigit(ch) || ch == ' ') {
            record += ch;
            state = s10;
        }
        else {
            printf("%d: '%c' error\n", lineNum, ch);
            state = s0;
            lexical_correct = 0;
        }
        }
        break;

        case 10:
        {ch = fgetc(fp);
        if (ch == '\'') {
            tokenList[index].line = lineNum;
            tokenList[index].wd.tok = CHARC;
            tokenList[index].wd.str = record;
            index++;
            ch = fgetc(fp);
        }
        else {
            printf("%d: '%c' error\n", lineNum, ch);
            lexical_correct = 0;
        }
        state = s0;
        }
        break;
        case 12:
        {if (ch == '\n') // 换行符特殊处理 行号+1
            lineNum += 1;
        else
            ;
        state = s0;
        ch = fgetc(fp); }
        break;

        case 13:
        {   printf("%d: '%c' error\n", lineNum, ch);
            lexical_correct = 0;
        state = s0;
        ch = fgetc(fp);	}
        break;
        }
    }
    tokenList[index].line = lineNum;
    tokenList[index].wd.str = ch;
    tokenList[index].wd.tok = ENDFILE;
}

void printTokenList()
{

    int i = 0;
    while (tokenList[i].wd.tok != ENDFILE)
    {
        cout << tokenList[i].line
            << "\t" << std::left << setw(16) << getString(tokenList[i].wd.tok)
            << "\t" << tokenList[i].wd.str <<"\t"<<tokenList[i].index<< endl;
        i++;
    }
    cout << tokenList[i].line
        << "\t" << std::left << setw(16) << getString(tokenList[i].wd.tok)
        << "\t" << tokenList[i].wd.str << endl;
    i++;
}
