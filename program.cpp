#include"program.h"
Program::Program()
{
    head=new lineNode;
    tail=new lineNode;
    head->next=tail;
    head->pre=nullptr;
    tail->pre=head;
    tail->next=nullptr;
}

Program::~Program()
{
    lineNode *next = nullptr;
    while(head !=  tail)
    {
        next=head->next;
        delete head;
        head=next;
    }
    delete head;
}
//在program中间添加语句
bool Program::addNode(lineNode *p)
{
    if(head->next==tail)
    {
        head->next=p;
        tail->pre=p;
        p->pre=head;
        p->next=tail;
        return true;
    }
    lineNode *tmpNode=tail->pre;
    if(tmpNode->lineNumber < p->lineNumber)
    {
        tmpNode->next = p;
        p->pre = tmpNode;
        tail->pre = p;
        p->next = tail;
        tmpNode = nullptr;
        return true;
    }
    tmpNode = head->next;
    while(tmpNode != tail)
    {
        if(tmpNode->lineNumber > p->lineNumber)
        {
            p->next=tmpNode;
            p->pre=tmpNode->pre;
            p->pre->next=p;
            tmpNode->pre=p;
            return true;
        }
        else if(tmpNode->lineNumber==p->lineNumber)
        {
            tmpNode->pre->next=p;
            tmpNode->next->pre=p;
            p->pre=tmpNode->pre;
            p->next=tmpNode->next;
            return true;
        }
        tmpNode=tmpNode->next;
    }
    return false;
}
//删除语句
bool Program::removeNode(int line)
{
    if(head->next==tail)
    {
        return false;
    }
    lineNode *tmpNode=head->next;
    while(tmpNode != tail)
    {
        if(tmpNode->lineNumber==line)
        {
            tmpNode->pre->next=tmpNode->next;
            tmpNode->next->pre=tmpNode->pre;
            tmpNode->~lineNode();
            return true;
        }
        tmpNode=tmpNode->next;
    }
    return false;
}
//在末尾添加语句
bool Program::addNode1(lineNode *p)
{
    if(head->next==tail)
    {
        head->next=p;
        tail->pre=p;
        p->pre=head;
        p->next=tail;
        return true;
    }
    lineNode *tmpNode=tail->pre;
    tmpNode->next=p;
    p->pre=tmpNode;
    tail->pre=p;
    p->next=tail;
    tmpNode = nullptr;
    return true;
}
void Program::clear()
{
    lineNode *p = head->next;
    lineNode *pnext = nullptr;
    while(p != tail)
    {
        pnext = p->next;
        delete p;
        p = pnext;
    }
    p = nullptr;
    head->next = tail;
    tail->pre = head;
}

