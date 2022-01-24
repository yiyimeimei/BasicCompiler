#include"statement.h"
#ifndef PROGRAM_H
#define PROGRAM_H

struct lineNode
{
     int lineNumber;//行号
     commandType lineCommand;//操作类型
     QString code;  //输入的指令，用于输出
     Statement *statement;    //statement类型
     lineNode *pre;
     lineNode *next;
     int errorNumber=0;
     lineNode()
     {
         pre = nullptr;
         next = nullptr;
         statement = nullptr;
     }
     ~lineNode()
     {
         if(statement)
            delete statement;
         statement = nullptr;
     }
};


class Program
{

public:
    lineNode *head,*tail;
    //Evalstate *evalstate;
    Program();
    ~Program();
    bool addNode(lineNode *p);
    bool addNode1(lineNode *p);
    bool removeNode(int line);
    void clear();
};



#endif // PROGRAM_H
