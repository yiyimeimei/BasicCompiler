#include"constant.h"
//#include"evalstate.h"
#include<string>
#include<math.h>
#include<map>
#ifndef EXP_H
#define EXP_H

class EvaluationContext;
class Expression
{
public:
    Expression();
    //~Expression();
    virtual ~Expression()=0;
    virtual int eval(EvaluationContext & context) = 0;
    virtual string toString() = 0;
    virtual expType type() = 0;
    virtual bool judge(EvaluationContext & context) = 0;
    virtual int getConstantValue()=0;
    virtual string getIdentifierName()=0;
    virtual string getOperator()=0;
    virtual Expression *getLHS()=0;
    virtual Expression *getRHS()=0;


};

class ConstantExp: public Expression {
public:

   ConstantExp(int val);

   virtual int eval(EvaluationContext & context);
   virtual std::string toString();//返回该exp的字符串形式
   virtual expType type();//返回该exp的类型
   virtual int getConstantValue();//返回constant的值
   virtual ~ConstantExp(){}
   virtual string getIdentifierName(){}
   virtual string getOperator(){}
   virtual Expression* getLHS(){}
   virtual Expression* getRHS(){}
   virtual bool judge(EvaluationContext & context);
private:
   int value;

};

class IdentifierExp: public Expression {

public:

   IdentifierExp(string name);

   virtual int eval(EvaluationContext & context);
   virtual std::string toString();
   virtual expType type();
   virtual string getIdentifierName();

   virtual ~IdentifierExp(){}
   virtual int getConstantValue(){}
   virtual string getOperator(){}
   virtual Expression* getLHS(){}
   virtual Expression* getRHS(){}
   virtual bool judge(EvaluationContext & context);




private:

   std::string name;

};

class CompoundExp: public Expression {

public:

   CompoundExp(string op, Expression *lhs, Expression *rhs);
   virtual ~CompoundExp();

   virtual int eval(EvaluationContext & context);
   virtual std::string toString();
   virtual expType type();

   virtual std::string getOperator();
   virtual Expression *getLHS();
   virtual Expression *getRHS();

   virtual int getConstantValue(){}
   virtual string getIdentifierName(){}
   virtual bool judge(EvaluationContext & context);

private:

   std::string op;
   Expression *lhs, *rhs;

};

class EvaluationContext {

public:

   void setValue(std::string var, int value);//给变量赋值
   int getValue(std::string var);//获得变量的值
   bool isDefined(std::string var);//判断变量是否定义
   map<std::string,int> traverse();//返回整个symbolTable
   void clear();
private:

   map<std::string,int> symbolTable;

};
class StringTable {
public:
   void setValue(std::string var, std::string value);//给变量赋值
   std::string getValue(std::string var);//获得变量的值
   bool isDefined(std::string var);//判断变量是否定义
   map<std::string, std::string> traverse();//返回整个symbolTable
   void clear();
private:
   map<std::string, std::string> symbolTable;
};


#endif // EXP_H
