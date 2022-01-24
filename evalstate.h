#ifndef EVALSTATE_H
#define EVALSTATE_H
#include<iostream>
#include<vector>
using namespace std;
struct var
{
    string varName;
    int value;
    int defineCounter=0;
    //bool constant;
    //bool defined=false;
};

class Evalstate
{
public:
    Evalstate();
    ~Evalstate();
    vector<var*> varTable;
    bool addNewVar(string &name,int val);
    //bool addNewConstant(string &name,int val);
    bool changeValue(string &name,int val);
    bool getVarValue(string &name,int *result);
    bool removeVar(string &name);
    //bool compareVarVar(string &name);


};


#endif // EVALSTATE_H
