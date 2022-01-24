#include"exp.h"
#ifndef STATEMENT_H
#define STATEMENT_H
class Statement
{
private:
    /*Expression * expression;
    commandType command;*/
public:
    Statement();
    virtual ~Statement()=0;
    virtual string toString()=0;
    virtual commandType type()=0;
    virtual void inputVarValue(int value)=0;
    virtual void get1Expression(Expression *&result1)=0;
    virtual void get2Expression(Expression *&result1,Expression *&result2)=0;
    virtual int getTargetLineNumber()=0;
    virtual string getVarName()=0;
    virtual string getStringValue()=0;
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2)=0;
};



class REMStatement: public Statement
{
private:
    string remark;
public:
    REMStatement(string rem);
    virtual ~REMStatement(){}
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value){}
    virtual void get1Expression(Expression *&result1){}
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber(){}
    virtual string getVarName();
    virtual string getStringValue(){}
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};




class LETStatement: public Statement
{
private:
    string variableName;
    string stringValue = "'";
    Expression *expression = nullptr;
public:
    LETStatement(string &varName, string value, Expression *expName);
    virtual ~LETStatement();
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value){}
    virtual void get1Expression(Expression *&result1);
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber(){}
    virtual string getVarName();
    virtual string getStringValue();
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};

class PRINTStatement: public Statement
{
private:
    Expression *expression;
public:
    PRINTStatement(Expression *expName);
    virtual ~PRINTStatement();
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value){}
    virtual void get1Expression(Expression *&result1);
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber(){}
    virtual string getVarName(){}
    virtual string getStringValue(){}
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};


class PRINTFStatement: public Statement
{
private:
    vector<string> *parameter1 = nullptr;
    vector<printfNode> *parameter2 = nullptr;
    string parameter1Value = "'";
public:
    PRINTFStatement(vector<string> *p1, vector<printfNode> *p2, string p3);
    virtual ~PRINTFStatement();
    virtual commandType type();
    virtual string toString(){}
    virtual void inputVarValue(int value){}
    virtual void get1Expression(Expression *&result1){}
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber(){}
    virtual string getVarName(){}
    virtual string getStringValue();
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2);
};

class INPUTStatement: public Statement
{
private:
    string variableName;
    int variableValue;
public:
    INPUTStatement(const string varName);
    virtual ~INPUTStatement();
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value);
    virtual void get1Expression(Expression *&result1){}
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber(){}
    virtual string getVarName();
    virtual string getStringValue(){}
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};

class INPUTSStatement: public Statement
{
private:
    string variableName;
    string variableValue;
public:
    INPUTSStatement(const string varName);
    virtual ~INPUTSStatement();
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value);
    virtual void get1Expression(Expression *&result1){}
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber(){}
    virtual string getVarName();
    virtual string getStringValue(){}
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};


class GOTOStatement: public Statement
{
private:
    int gotoLineNumber;
public:
    GOTOStatement(int &val);
    virtual ~GOTOStatement();
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value){}
    virtual void get1Expression(Expression *&result1){}
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber();
    virtual string getVarName(){}
    virtual string getStringValue(){}
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};

class IFStatement: public Statement
{
private:
    Expression *expression1,*expression2;
    string op;
    int gotoLineNumber;
public:
    IFStatement(Expression *exp1,Expression *exp2,string &opName,int &val);
    virtual ~IFStatement();
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value){}
    virtual void get1Expression(Expression *&result1){}
    virtual void get2Expression(Expression *&result1,Expression *&result2);
    virtual int getTargetLineNumber();
    virtual string getVarName();
    virtual string getStringValue(){}
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};

class ENDStatement: public Statement
{
private:

public:
    ENDStatement();
    virtual ~ENDStatement();
    virtual commandType type();
    virtual string toString();
    virtual void inputVarValue(int value){}
    virtual void get1Expression(Expression *&result1){}
    virtual void get2Expression(Expression *&result1,Expression *&result2){}
    virtual int getTargetLineNumber(){}
    virtual string getVarName(){}
    virtual string getStringValue(){}
    virtual void get2Vector(vector<string> *&p1, vector<printfNode> *&p2){}
};





#endif // STATEMENT_H
