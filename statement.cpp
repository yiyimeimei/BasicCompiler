#include"statement.h"
Statement::Statement(){}
Statement::~Statement(){}


REMStatement::REMStatement(string rem)
{
    remark=rem;
}
string REMStatement::toString()
{
    string temp;
    temp=" REM "+remark;
    return temp;
}
commandType REMStatement::type()
{
    return REM;
}
string REMStatement::getVarName()
{
    return remark;
}


LETStatement::LETStatement(string &name, string value, Expression *expName)
{
    variableName = name;
    expression = expName;
    stringValue = value;
}
LETStatement::~LETStatement()
{
    delete expression;
}
commandType LETStatement::type()
{
    return LET;
}
string LETStatement::toString()
{
    string temp;
    if(expression != nullptr)
        temp=" LET "+variableName+" = "+expression->toString();
    else
        temp=" LET "+variableName+" = "+stringValue;
    return temp;
}
void LETStatement::get1Expression(Expression *&result1)
{
    result1 = expression;
}
string LETStatement::getVarName()
{
    return variableName;
}
string LETStatement::getStringValue()
{
    return stringValue;
}




PRINTStatement::PRINTStatement(Expression *expName)
{
    expression=expName;
}
PRINTStatement::~PRINTStatement()
{
    delete expression;
}
commandType PRINTStatement::type()
{
    return PRINT;
}
string PRINTStatement::toString()
{
    string temp;
    temp=" PRINT "+expression->toString();
    return temp;
}
void PRINTStatement::get1Expression(Expression *&result1)
{
    result1=expression;
    return;
}


PRINTFStatement::PRINTFStatement(vector<string> *p1, vector<printfNode> *p2, string p3)
{
    parameter1 = p1;
    parameter2 = p2;
    parameter1Value = p3;
}
PRINTFStatement::~PRINTFStatement()
{
    parameter1->clear();
    parameter2->clear();
    delete parameter1;
    delete parameter2;
}
commandType PRINTFStatement::type()
{
    return PRINTF;
}
string PRINTFStatement::getStringValue()
{
    return parameter1Value;
}
void PRINTFStatement::get2Vector(vector<string> *&p1, vector<printfNode> *&p2)
{
    p1 = parameter1;
    p2 = parameter2;
}


INPUTStatement::INPUTStatement(const string varName)
{
    variableName=varName;
}
INPUTStatement::~INPUTStatement()
{

}
void INPUTStatement::inputVarValue(int value)
{
    variableValue=value;
}
commandType INPUTStatement::type()
{
    return INPUT;
}
string INPUTStatement::toString()
{
    string temp;
    temp=" INPUT "+variableName;
    return temp;
}
string INPUTStatement::getVarName()
{
    return variableName;
}





INPUTSStatement::INPUTSStatement(const string varName)
{
    variableName=varName;
}
INPUTSStatement::~INPUTSStatement()
{

}
void INPUTSStatement::inputVarValue(int value)
{
}
commandType INPUTSStatement::type()
{
    return INPUTS;
}
string INPUTSStatement::toString()
{
    string temp;
    temp=" INPUTS "+variableName;
    return temp;
}
string INPUTSStatement::getVarName()
{
    return variableName;
}



GOTOStatement::GOTOStatement(int &val)
{
    gotoLineNumber=val;//可能行数会不存在或超过范围
}
GOTOStatement::~GOTOStatement()
{

}
commandType GOTOStatement::type()
{
    return GOTO;
}
string GOTOStatement::toString()
{
    string temp;
    temp=" GOTO "+to_string(gotoLineNumber);
    return temp;
}
int GOTOStatement::getTargetLineNumber()
{
    return gotoLineNumber;
}





IFStatement::IFStatement(Expression *exp1,Expression *exp2,string &opName,int &val)
{
    expression1=exp1;
    expression2=exp2;
    op=opName;
    gotoLineNumber=val;
}
IFStatement::~IFStatement()
{
    delete expression1;
    delete expression2;
}
commandType IFStatement::type()
{
    return IF;
}
string IFStatement::toString()
{
    string temp;
    temp=" IF "+expression1->toString()+" "+op+" "+expression2->toString()+" THEN "+to_string(gotoLineNumber);
    return temp;
}
void IFStatement::get2Expression(Expression *&result1,Expression *&result2)
{
    result1=expression1;
    result2=expression2;
    return;
}
int IFStatement::getTargetLineNumber()
{
    return gotoLineNumber;
}
string IFStatement::getVarName()
{
    return op;
}



ENDStatement::ENDStatement()
{

}
ENDStatement::~ENDStatement()
{

}
commandType ENDStatement::type()
{
    return END;
}
string ENDStatement::toString()
{
    string temp;
    temp=" END";
    return temp;
}
