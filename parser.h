#include"exp.h"
#ifndef PARSER_H
#define PARSER_H


class Parser
{
private:
    Expression *exp;
public:
    Parser();
    Parser(Expression *p);
    ~Parser();
    int parseExpression(EvaluationContext &context);
    //string
};

#endif // PARSER_H
