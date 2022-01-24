#include<cstring>
#include<stdio.h>
#include<string.h>
#include<string>
#include<QString>
#include<iostream>
#include<stack>
#include<vector>
#include<sstream>
#include"exp.h"
using namespace std;
#ifndef TOKENIZER_H
#define TOKENIZER_H


class Tokenizer
{
public:
    Tokenizer();
    ~Tokenizer();

    int getLineNumber(const string &inputCode,int *endPosition);
    commandType getCommandType(const string &inputCode,int *endPosition);
    string getRem(const string &inputCode,int *endPosition);
    bool PosChar(const string &inputCode,char ch,int *symbolPos);
    string getVar(const string &inputCode,int *endPosition);
    bool judgeCodeEnd(const string &inputCode,int *endPosition);
    Expression* dealExpression(const string &inputCode,int *endPosition);
    bool dealPrintf(vector<string> *p1, vector<printfNode> *p2, string *p3, const string &inputCode, int *endPosition);
    string dealString(const string &inputCode, int *endPosition);
    void jumpBlank(const string &inputCode,int *endPosition);
    bool isDigit(char ch);
    bool isLetter(char ch);

};



#endif // TOKENIZER_H
