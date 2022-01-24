#include"exp.h"
Expression::Expression(){}
Expression::~Expression(){}




//constant
ConstantExp::ConstantExp(int val)
{
    value=val;
}
std::string ConstantExp::toString()
{
    return to_string(value);//int转为string的全局函数
}
expType ConstantExp::type()
{
    return CONSTANT;
}

int ConstantExp::getConstantValue()
{
    return value;
}
int ConstantExp::eval(EvaluationContext & context)
{
    return value;
}
bool ConstantExp::judge(EvaluationContext & context)
{
    return true;
}






//identifier
IdentifierExp::IdentifierExp(string name)
{
    this->name=name;
}
int IdentifierExp::eval(EvaluationContext & context)
{
    return context.getValue(name);
}
std::string IdentifierExp::toString()
{
    return name;
}
expType IdentifierExp::type()
{
    return IDENTIFIER;
}
string IdentifierExp::getIdentifierName()
{
    return name;
}
bool IdentifierExp::judge(EvaluationContext & context)
{
    if (context.isDefined(name))
        return true;
    else
        return false;
}







//compound
CompoundExp::CompoundExp(string op, Expression *lhs, Expression *rhs)
{
    this->op=op;
    this->lhs=lhs;
    this->rhs=rhs;
}
CompoundExp::~CompoundExp()
{
    if(lhs->type()==COMPOUND)
    {
        delete lhs;
    }
    if(rhs->type()==COMPOUND)
    {
        delete rhs;
    }
}

int CompoundExp::eval(EvaluationContext & context)
{
    int right = rhs->eval(context);
    if (op == "=") {
       context.setValue(lhs->getIdentifierName(), right);
       return right;
    }
    int left = lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
       if (right == 0)
       {
           //error("Division by 0");
       }
       return left / right;
    }
    if(op == "**")
    {
        return pow(left,right);
    }
    return 0;
}
std::string CompoundExp::toString()
{
    string temp;
    temp=" ( "+lhs->toString()+" "+op+" "+rhs->toString()+" ) ";
    return temp;

}
expType CompoundExp::type()
{
    return COMPOUND;
}

std::string CompoundExp::getOperator()
{
    return op;
}
Expression* CompoundExp::getLHS()
{
    return lhs;
}
Expression* CompoundExp::getRHS()
{
    return rhs;
}
bool CompoundExp::judge(EvaluationContext & context)
{
    bool left = lhs->judge(context);
    bool right = rhs->judge(context);
    if(left && right)//两边都没问题
    {
        if (op == "/" && rhs->eval(context) == 0)
        {
           return false;
        }
        return true;
    }
    else
        return false;
}









void EvaluationContext::setValue(std::string var, int value)
{
    map<std::string,int>::iterator iter;
    iter=symbolTable.find(var);
    if(iter==symbolTable.end())
    {
        symbolTable.insert(pair<std::string,int>(var,value));
        return;
    }
    else
    {
        iter->second=value;
        return;
    }
}
int EvaluationContext::getValue(std::string var)
{
    map<std::string,int>::iterator iter;
    iter=symbolTable.find(var);
    return iter->second;
}
bool EvaluationContext::isDefined(std::string var)
{
    map<std::string,int>::iterator iter;
    iter=symbolTable.find(var);
    if(iter==symbolTable.end())
    {
        return false;
    }
    else
    {
        return true;;
    }
}
map<std::string, int> EvaluationContext::traverse()
{
    return symbolTable;
}
void EvaluationContext::clear()
{
    symbolTable.clear();
}


void StringTable::setValue(std::string var, std::string value)
{
    map<std::string, std::string>::iterator iter;
    iter=symbolTable.find(var);
    if(iter==symbolTable.end())
    {
        symbolTable.insert(pair<std::string, std::string>(var,value));
        return;
    }
    else
    {
        iter->second=value;
        return;
    }

}
std::string StringTable::getValue(std::string var)
{
    map<std::string, std::string>::iterator iter;
    iter=symbolTable.find(var);
    return iter->second;
}
bool StringTable::isDefined(std::string var)
{
    map<std::string, std::string>::iterator iter;
    iter=symbolTable.find(var);
    if(iter==symbolTable.end())
    {
        return false;
    }
    else
    {
        return true;;
    }
}
map<std::string, std::string> StringTable::traverse()
{
    return symbolTable;
}
void StringTable::clear()
{
    symbolTable.clear();
}
