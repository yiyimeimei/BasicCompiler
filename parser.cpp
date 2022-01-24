#include"parser.h"
Parser::Parser()
{

}
Parser::Parser(Expression *p)
{
    exp=p;
}
Parser::~Parser()
{

}
int Parser::parseExpression(EvaluationContext &context)
{
    return exp->eval(context);

    /*switch(exp->type())
    {
    case CONSTANT:
        return exp->getConstantValue();
    case IDENTIFIER:
        return exp->eval(context);
    case COMPOUND:
        return exp->eval(context);

    }*/

}
