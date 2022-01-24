#include"tokenizer.h"
Tokenizer::Tokenizer()
{


}
Tokenizer::~Tokenizer()
{

}
void Tokenizer::jumpBlank(const string &inputCode, int *endPosition)
{
    while('\0'!=inputCode[*endPosition])
    {
        if(' '!=inputCode[*endPosition])
        {
            break;
        }
        ++(*endPosition);
    }
    return;
}
bool Tokenizer::isDigit(char ch)
{
    if(ch<='9'&&ch>='0')
        return true;
    return false;
}
bool Tokenizer::isLetter(char ch)
{
    if((ch<='z'&&ch>='a')||(ch<='Z'&&ch>='A'))
        return true;
    return false;
}
int Tokenizer::getLineNumber(const string &inputCode,int *endPosition)
{
    int lineNumber=0;
    int start=0,end=0;
    //找到行号的开头位置
    jumpBlank(inputCode,endPosition);
    start=(*endPosition);
    end=start;
    //找到行号的结尾位置
    while('\0'!=inputCode[end])
    {
        if(' '==inputCode[end])
            break;
        end++;
    }
    if(end==start)
    {
        (*endPosition)=end;
        return -1;
    }
    //读取行数
    while(start < end)
    {
        if(isDigit(inputCode[start]))
        {
            lineNumber = lineNumber * 10 + inputCode[start]-'0';
            ++start;
        }
        else
        {
            return -1;
        }
        if(lineNumber>1000000)
        {
            return -1;
        }
    }
    (*endPosition)=end;
    return lineNumber;
}
commandType Tokenizer::getCommandType(const string &inputCode,int *endPosition)
{
    int start=0,end=0;
    char stateType[10];
    //找到命令类型部分的开头位置
    jumpBlank(inputCode,endPosition);
    start=(*endPosition);
    end=start;
    //找到命令类型部分的结尾位置
    while('\0'!=inputCode[end])
    {
        if(' '==inputCode[end])
            break;
        end++;
    }
    if(end==start)
    {
        (*endPosition)=end;
        return BLANK;
    }
    (*endPosition)=end;
    --end;
    //读取命令类型
    int bitCounter=0;
    while(end>=start&&bitCounter<10)
    {
        stateType[bitCounter]=inputCode[start];
        bitCounter++;
        start++;
    }
    stateType[bitCounter]='\0';
    if(strcmp("LET",stateType)==0)
          return LET;
    else if(strcmp("REM",stateType)==0)
          return REM;
    else if(strcmp("INPUT",stateType)==0)
          return INPUT;
    else if(strcmp("INPUTS",stateType)==0)
          return INPUTS;
    else if(strcmp("END",stateType)==0)
          return END;
    else if(strcmp("RUN",stateType)==0)
          return RUN;
    else if(strcmp("LOAD",stateType)==0)
          return LOAD;
    else if(strcmp("LIST",stateType)==0)
          return LIST;
    else if(strcmp("PRINT",stateType)==0)
          return PRINT;
    else if(strcmp("PRINTF",stateType)==0)
          return PRINTF;
    else if(strcmp("IF",stateType)==0)
          return IF;
    else if(strcmp("CLEAR",stateType)==0)
          return CLEAR;
    else if(strcmp("HELP",stateType)==0)
          return HELP;
    else if(strcmp("QUIT",stateType)==0)
          return QUIT;
    else if(strcmp("GOTO",stateType)==0)
          return GOTO;
    else
        return STATEWRONG;

}
string Tokenizer::getRem(const string &inputCode,int *endPostion)
{
    string rem;
    int start=(*endPostion)+1;
    rem.append(inputCode,start,inputCode.length()-start);
    rem.append("\0");
    return rem;
}
bool Tokenizer::PosChar(const string &inputCode,char ch,int *symbolPos)
{
    int i = (*symbolPos);
    while('\0' != inputCode[i])
    {
        if(ch == inputCode[i])
        {
            (*symbolPos) = i;
            return true;
        }
        ++i;
    }
    return false;
}
string Tokenizer::getVar(const string &inputCode, int *endPosition)
{
    int start=0,end=0;
    jumpBlank(inputCode,endPosition);
    start=(*endPosition);
    if((inputCode[start]>='A'&&inputCode[start]<='Z')||(inputCode[start]>='a'&&inputCode[start]<='z'))
    {
        end=start+1;
        while((inputCode[end]>='A'&&inputCode[end]<='Z')||(inputCode[end]>='a'&&inputCode[end]<='z')||(inputCode[end]>='0'&&inputCode[end]<='9')||inputCode[end]=='_')
        {
            ++end;
        }
        (*endPosition)=end;
        if((end-start)>40)
        {
            return "1";//变量命名不符合规则
        }
        string varName;
        varName.append(inputCode,start,end-start);
        varName.append("\0");
        return varName;
    }
    else
    {
        return "1";//变量命名不符合规则
    }
}
bool Tokenizer::judgeCodeEnd(const string &inputCode, int *endPosition)
{
    int start;
    start=(*endPosition);
    while('\0'!=inputCode[start])
    {
        if(' '!=inputCode[start])
        {
            return false;
        }
        ++start;
    }
    return true;
}
string Tokenizer::dealString(const string &inputCode, int *endPosition)
{
    int start = 0, end = 0;
    bool findBackQuotationMark = false;
    char quotation = inputCode[*endPosition];
    ++(*endPosition);
    //jumpBlank(inputCode, endPosition);
    start = (*endPosition);
    end = (*endPosition);
    if(quotation == '\'')//单引号
    {
        while(inputCode[end] != '\0')
        {
            if(inputCode[end] == '\'')
            {
                findBackQuotationMark = true;
                break;
            }
            if(inputCode[end] == '"')
            {
                break;
            }
            ++end;
        }
    }
    else if(quotation == '"')//双引号
    {
        while(inputCode[end] != '\0')
        {
            if(inputCode[end] == '\'')
            {
                break;
            }
            if(inputCode[end] == '"')
            {
                findBackQuotationMark = true;
                break;
            }
            ++end;
        }
    }
    *endPosition = end + 1;
    if(findBackQuotationMark && judgeCodeEnd(inputCode,endPosition))
    {
        string stringValue;
        stringValue.append(inputCode,start,end-start);
        stringValue.append("\0");
        return stringValue;
    }
    return "'";//错误输入
}

Expression* Tokenizer::dealExpression(const string &inputCode, int *endPosition)
{
    int start;
    stack<string> opstack;
    stack<Expression*> expstack;
    vector<string> expression;
    string input;

    start=(*endPosition);
    while(inputCode[start]!='\0')
    {
        if(inputCode[start]==' ')
        {
            ++start;
            continue;
        }
        input.push_back(inputCode[start]);
        ++start;
    }
    //处理每一个字符，将中缀表达式转换为后缀表达式
    int len=input.size();
    if(len==0)
    {
        return nullptr;
    }
    for(int i=0;i<len;++i)
    {
        //处理变量名
        if(isLetter(input[i]))
        {
            start=i;
            ++i;
            while(isLetter(input[i])||isDigit(input[i])||(input[i]=='_'))
            {
                ++i;
            }
            --i;
            string tempstr(input, start, i-start+1);
            expression.push_back(tempstr);
        }
        //处理无符号数字
        else if(isDigit(input[i]))
        {
            start=i;
            ++i;
            while(isDigit(input[i]))
            {
                ++i;
            }
            --i;
            string tempstr(input, start, i-start+1);
            expression.push_back(tempstr);
        }
        //处理‘-’号
        else if(input[i]=='-')
        {
            if(i==0)//单独的数字或变量前有负号
            {
                if(isDigit(input[i+1]))//单独的数字+负号
                {
                    i+=2;
                    while(isDigit(input[i]))
                    {
                        ++i;
                    }
                    --i;
                    string tempstr(input, 1, i);
                    expression.push_back("0");
                    opstack.push("-");
                    expression.push_back(tempstr);
                }
                else if(isLetter(input[i+1]))//单独的变量+负号
                {
                    i+=2;
                    while(isLetter(input[i])||isDigit(input[i])||(input[i]=='_'))
                    {
                        ++i;
                    }
                    --i;
                    string tempstr(input, 1, i);
                    expression.push_back("0");
                    opstack.push("-");
                    expression.push_back(tempstr);
                }
                else if(input[i+1] == '(')
                {
                    expression.push_back("0");
                    opstack.push("-");
                }
                else
                    return nullptr;
            }
            else if(input[i-1]=='(')//包含在一个复杂表达式里面的负号
            {
                if(isDigit(input[i+1]))//负数
                {
                    ++i;
                    start=i;
                    ++i;
                    while(isDigit(input[i]))
                    {
                        ++i;
                    }
                    --i;
                    expression.push_back("0");
                    opstack.push("-");
                    string tempstr(input, start, i-start+1);
                    expression.push_back(tempstr);
                }
                else if(isLetter(input[i+1]))//负的变量
                {
                    ++i;
                    start=i;
                    ++i;
                    while(isLetter(input[i])||isDigit(input[i])||(input[i]=='_'))
                    {
                        ++i;
                    }
                    --i;
                    string tempstr(input, start, i-start+1);
                    expression.push_back("0");
                    opstack.push("-");
                    expression.push_back(tempstr);
                }
                else if(input[i+1] == '(')
                {
                    expression.push_back("0");
                    opstack.push("-");
                }
                else
                    return nullptr;
            }
            else//减号
            {
                while(!opstack.empty())//判断栈是否为空，栈不为空，判断优先级，栈为空，直接push
                {
                    string str=opstack.top();
                    if(str=="(")//栈顶元素优先级低，该符号入栈
                    {
                        opstack.push("-");
                        break;
                    }
                    else//否则栈顶元素先出栈
                    {
                        expression.push_back(str);
                        opstack.pop();
                    }
                }
                if(opstack.empty())
                {
                    opstack.push("-");
                }
            }
        }
        else if(input[i]=='+')//加号
        {
            while(!opstack.empty())//判断栈是否为空,加减优先级相同
            {
                string str=opstack.top();
                if(str=="(")//栈顶元素优先级低，该符号入栈
                {
                    opstack.push("+");
                    break;
                }
                else//否则栈顶元素先出栈
                {
                    expression.push_back(str);
                    opstack.pop();
                }
            }
            if(opstack.empty())
            {
                opstack.push("+");
            }
        }
        else if(input[i]=='*')//乘号或乘方
        {
            if(input[i+1]=='*')//乘方
            {
                while(!opstack.empty())//判断栈是否为空
                {
                    string str=opstack.top();
                    if(str=="**")//栈顶元素优先级大于等于该符号，栈顶元素先出栈
                    {
                        expression.push_back(str);
                        opstack.pop();
                    }
                    else//栈顶元素优先级低，该符号入栈
                    {
                        opstack.push("**");
                        break;
                    }
                }
                if(opstack.empty())
                {
                    opstack.push("**");
                }
                ++i;
            }
            else//乘号
            {
                while(!opstack.empty())//判断栈是否为空
                {
                    string str=opstack.top();
                    if(str=="("||str=="+"||str=="-")//栈顶元素优先级低，该符号入栈
                    {
                        opstack.push("*");
                        break;
                    }
                    else//否则栈顶元素先出栈
                    {
                        expression.push_back(str);
                        opstack.pop();
                    }
                }
                if(opstack.empty())
                {
                    opstack.push("*");
                }
            }
        }
        else if(input[i]=='/')//除号
        {
            while(!opstack.empty())//判断栈是否为空
            {
                string str=opstack.top();
                if(str=="("||str=="+"||str=="-")//栈顶元素优先级低，该符号入栈
                {
                    opstack.push("/");
                    break;
                }
                else//否则栈顶元素先出栈
                {
                    expression.push_back(str);
                    opstack.pop();
                }
            }
            if(opstack.empty())
            {
                opstack.push("/");
            }
        }
        else if(input[i]=='(')
        {
            opstack.push("(");
        }
        else if(input[i]==')')
        {
            string str=opstack.top();
            while(str!="(")
            {
                expression.push_back(str);
                opstack.pop();
                str=opstack.top();
            }
            opstack.pop();
        }
        else
        {
            return nullptr;
        }
    }


    //处理栈中剩余的操作符
    while(!opstack.empty())
    {
        expression.push_back(opstack.top());
        opstack.pop();
    }


    //将后缀表达式转化为表达式树
    len=expression.size();
    string str;
    for(int i=0;i<len;++i)
    {
        str=expression[i];
        if(isDigit(str[0]))
        {
            int number = atoi(str.c_str());
            Expression *constant=new ConstantExp(number);
            expstack.push(constant);
        }
        else if(isLetter(str[0]))
        {
            Expression *identifier=new IdentifierExp(str);
            expstack.push(identifier);
        }
        else
        {
            Expression *right=expstack.top();
            expstack.pop();
            Expression *left=expstack.top();
            expstack.pop();
            Expression *compound=new CompoundExp(str,left,right);
            expstack.push(compound);
        }
    }


    //栈中仅剩的一个指针指向表达式树的根节点
    Expression *result = nullptr;
    result = expstack.top();
    expstack.pop();
    if(expstack.empty())
        return result;
    else
        return nullptr;
}
bool Tokenizer::dealPrintf(vector<string> *p1, vector<printfNode> *p2, string *p3, const string &inputCode, int *endPosition)
{
    /*string *str = new string("s");
    p3 = str;
    return true;*/
    jumpBlank(inputCode, endPosition);
    char quotation = inputCode[*endPosition];
    if(quotation != '\'' && quotation != '"')
    {
        return false;
    }
    //读取第一个参数
    int start = 0, end = 0;
    bool findBackQuotationMark = false;
    ++(*endPosition);
    start = (*endPosition);
    end = (*endPosition);
    if(quotation == '\'')//单引号
    {
        while(inputCode[end] != '\0')
        {
            if(inputCode[end] == '\'')
            {
                findBackQuotationMark = true;
                break;
            }
            if(inputCode[end] == '"')
            {
                break;
            }
            ++end;
        }
    }
    else if(quotation == '"')//双引号
    {
        while(inputCode[end] != '\0')
        {
            if(inputCode[end] == '\'')
            {
                break;
            }
            if(inputCode[end] == '"')
            {
                findBackQuotationMark = true;
                break;
            }
            ++end;
        }
    }
    if(!findBackQuotationMark)
        return false;
    (*endPosition) = end + 1;
    if(start == end)
    {
        if(judgeCodeEnd(inputCode, endPosition))
            return true;
        else
            return false;
    }
    //处理第一个参数
    if(start == end)
    {
        *p3 = "";
        if(judgeCodeEnd(inputCode,endPosition))
            return true;
        else
            return false;
    }
    string parameter;
    parameter.append(inputCode,start,end-start);
    parameter.append("\0");
    *p3 = parameter;
    start = 0;
    end = 0;
    string temp;
    while(parameter[end] != '\0')
    {
        if(parameter[end] == '{')
        {
            if(parameter[end + 1] != '}')
                return false;
            else
            {
                if(start == end)
                    p1->push_back("");
                else
                {
                    temp.clear();
                    temp.append(parameter, start, end - start);
                    p1->push_back(temp);
                }
                end += 2;
                start = end;
            }
        }
        else
            ++end;
    }
    if(start == end)
        p1->push_back("");
    else
    {
        temp.clear();
        temp.append(parameter, start, end - start);
        p1->push_back(temp);
    }
    if(p1->size() == 1)//仅有一个参数
    {
        if(judgeCodeEnd(inputCode, endPosition))
            return true;
        return false;
    }

    //读取并处理2-n个参数
    while(inputCode[*endPosition] != '\0')
    {
        jumpBlank(inputCode, endPosition);
        if(inputCode[*endPosition] == ',')
        {
            ++(*endPosition);
            jumpBlank(inputCode, endPosition);
            if(inputCode[*endPosition] == '"')
            {
                ++(*endPosition);
                findBackQuotationMark = false;
                start = (*endPosition);
                while(inputCode[*endPosition] != '\0')
                {
                    if(inputCode[*endPosition] == '\'')
                    {
                        break;
                    }
                    if(inputCode[*endPosition] == '"')
                    {
                        findBackQuotationMark = true;
                        break;
                    }
                    ++(*endPosition);
                }
                if(!findBackQuotationMark)
                    return  false;
                else
                {
                    if(start == (*endPosition))
                    {
                        printfNode newPrintfNode(0, "");
                        p2->push_back(newPrintfNode);
                    }
                    else
                    {
                        temp.clear();
                        temp.append(inputCode, start, (*endPosition) - start);
                        printfNode newPrintfNode(0, temp);
                        p2->push_back(newPrintfNode);
                    }
                    ++(*endPosition);
                }
            }
            else if(inputCode[*endPosition] == '\'')
            {
                ++(*endPosition);
                findBackQuotationMark = false;
                start = (*endPosition);
                while(inputCode[*endPosition] != '\0')
                {
                    if(inputCode[*endPosition] == '\'')
                    {
                        findBackQuotationMark = true;
                        break;
                    }
                    if(inputCode[*endPosition] == '"')
                    {
                        break;
                    }
                    ++(*endPosition);
                }
                if(!findBackQuotationMark)
                    return  false;
                else
                {
                    if(start == (*endPosition))
                    {
                        printfNode newPrintfNode(0, "");
                        p2->push_back(newPrintfNode);
                    }
                    else
                    {
                        temp.clear();
                        temp.append(inputCode, start, (*endPosition) - start);
                        printfNode newPrintfNode(0, temp);
                        p2->push_back(newPrintfNode);
                    }
                    ++(*endPosition);
                }
            }
            else if(isDigit(inputCode[*endPosition]))
            {
                start = (*endPosition)++;
                while(isDigit(inputCode[*endPosition]))
                {
                    ++(*endPosition);
                }
                temp.clear();
                temp.append(inputCode, start, (*endPosition) - start);
                printfNode newPrintfNode(1, temp);
                p2->push_back(newPrintfNode);
            }
            else if(isLetter(inputCode[*endPosition]))
            {
                temp = getVar(inputCode, endPosition);
                if(temp == "1")
                    return false;
                printfNode newPrintfNode(2, temp);
                p2->push_back(newPrintfNode);
            }
        }
        else if(judgeCodeEnd(inputCode, endPosition))
        {
            break;
        }
        else
        {
            return false;
        }
    }
    if(p1->size() != p2->size() + 1)
        return false;
    return true;
}
