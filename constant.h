#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<QList>
#include<QString>
using namespace std;
#ifndef CONSTANT_H
#define CONSTANT_H

enum expType {CONSTANT=0,IDENTIFIER,COMPOUND};
enum commandType {REM=0,LET,PRINT,INPUT,GOTO,IF,END,RUN,LOAD,LIST,CLEAR,HELP,QUIT,STATEWRONG,BLANK,INPUTS,PRINTF};
enum opType{ADD=0,SUB,MUL,DIV,POWER,LEFTBRACKET,RIGHTBRACKET,NONE};

struct printfNode
{
    int type;//0代表可以直接输出parameter，1代表parameter是整型，2代表parameter是变量名。
    string parameter;
    printfNode(int x, string str)
    {
        type = x;
        parameter = str;
    }
};



#endif // CONSTANT_H
