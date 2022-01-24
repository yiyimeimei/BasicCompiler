#include"evalstate.h"
Evalstate::Evalstate()
{

}
Evalstate::~Evalstate()
{
    for(int i=varTable.size()-1;i>=0;++i)
    {
        var *temp=nullptr;
        temp=varTable[i];
        delete temp;
    }
}
bool Evalstate::addNewVar(string &name,int val)
{
    for(int i=0;i<varTable.size();++i)
    {
        if(varTable[i]->varName==name)
        {
            return false;
        }
    }
    var *temp=new var;
    temp->varName=name;
    temp->value=val;
    ++temp->defineCounter;
    varTable.push_back(temp);
    return true;
}
/*bool Evalstate::addNewConstant(string &name,int val)
{
    for(int i=0;i<varTable.size();++i)
    {
        if(varTable[i]->varName==name)
        {
            return false;
        }
    }
    var *temp=new var;
    temp->varName=name;
    temp->value=val;
    varTable.push_back(temp);
    return true;
}*/
bool Evalstate::changeValue(string &name,int val)
{
    for(int i=0;i<varTable.size();++i)
    {
        if(varTable[i]->varName==name)
        {
            varTable[i]->value=val;
            ++varTable[i]->defineCounter;
            return true;
        }
    }
    return false;
}
bool Evalstate::getVarValue(string &name,int *result)
{
    for(int i=0;i<varTable.size();++i)
    {
        if(varTable[i]->varName==name)
        {
            *result=varTable[i]->value;
            return true;
        }
    }
    return false;
}
bool Evalstate::removeVar(string &name)
{
    for (vector<var*>::iterator it = varTable.begin(); it != varTable.end(); ++it)
    {
        if ((*it)->varName==name)
        {
            it = varTable.erase(it);
            return true;
        }
    }
    return false;
}
