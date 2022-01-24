#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tokenizer=new Tokenizer;
    program=new Program;
    evalContext=new EvaluationContext;
    stringTable = new StringTable;
    fileDialog = new QFileDialog(this);
    connect(ui->codeEdit,SIGNAL(returnPressed()),&loop,SLOT(quit()));
    connect(ui->stepButton,SIGNAL(clicked()),&loop2,SLOT(quit()));
    connect(ui->runButton,SIGNAL(clicked()),&loop2,SLOT(quit()));
    QString placeHolder = QString::fromStdString("Please Input Your Code.");
    ui->codeEdit->setPlaceholderText(placeHolder);
    ui->stepButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tokenizer;
    delete program;
    delete evalContext;
    delete stringTable;

}

//用户输入代码
void MainWindow::on_codeEdit_returnPressed()
{
    QString text = ui->codeEdit->text();
    std::string str = text.toStdString();
    if(str[0] == ' ' && str[1] == '?' && str[2] == ' ')
    {
        return;
    }
    else
    {
        dealNewLine(text);
        ui->codeEdit->clear();
        return;
    }
}
//错误输出
void MainWindow::error(const std::string errorReport)
{
    ui->resultBrowser->clear();
    QString qstr=QString::fromStdString(errorReport);
    ui->resultBrowser->append(qstr);
    return;
}
//处理一行代码
void MainWindow::dealNewLine(const QString text)
{

    std::string inputCode = text.toStdString();
    int *endPos = new int(0);
    lineNode *newLineNode = new lineNode;
    newLineNode->code = text;

    newLineNode->lineNumber = tokenizer->getLineNumber(inputCode,endPos);
    newLineNode->lineCommand = tokenizer->getCommandType(inputCode,endPos);
    if(newLineNode->lineNumber==-1)
    {
        //行号报错
        if(newLineNode->lineCommand==REM||newLineNode->lineCommand==GOTO||newLineNode->lineCommand==IF||newLineNode->lineCommand==END||newLineNode->lineCommand==BLANK||newLineNode->lineCommand==STATEWRONG)
        {
            error("Wrong LineNumber!");
            delete endPos;
            delete newLineNode;
            endPos = nullptr;
            newLineNode = nullptr;
            return;
        }
        //没有行号直接执行INPUT PRINT INPUTS PRINTF LET指令
        if(newLineNode->lineCommand==LET||newLineNode->lineCommand==PRINT||newLineNode->lineCommand==PRINTF||newLineNode->lineCommand==INPUT||newLineNode->lineCommand==INPUTS)
        {
            switch(newLineNode->lineCommand)
            {
            case LET:
                dealImmediateLET(inputCode,endPos);
                break;
            case INPUT:
                dealImmediateINPUT(inputCode,endPos);
                break;
            case PRINT:
                dealImmediatePRINT(inputCode,endPos);
                break;
            case PRINTF:
                dealImmediatePRINTF(inputCode,endPos);
                break;
            case INPUTS:
                dealImmediateINPUTS(inputCode,endPos);
                break;
            }
            return;
        }
        //没有行号直接执行RUN LOAD CLEAR HELP QUIT指令
        if(newLineNode->lineCommand==RUN||newLineNode->lineCommand==LOAD||newLineNode->lineCommand==CLEAR||newLineNode->lineCommand==HELP||newLineNode->lineCommand==QUIT)
        {
            if(!tokenizer->judgeCodeEnd(inputCode,endPos))
            {
                error("Wrong Command!");
                delete endPos;
                delete newLineNode;
                return;
            }
            switch(newLineNode->lineCommand)
            {
            case RUN:
                on_runButton_clicked();
                break;
            case LOAD:
                on_loadButton_clicked();
                break;
            case LIST:
                break;
            case CLEAR:
                on_clearButton_clicked();
                break;
            case HELP:
                dealHELP();
                break;
            case QUIT:
                if (!(QMessageBox::information(this,tr("Quit Basic"),tr("Do you really want quit ?"),tr("Yes"),tr("No"))))
                {
                    on_clearButton_clicked();
                    delete newLineNode;
                    delete tokenizer;
                    delete program;
                    delete endPos;
                    exit(0);
                }
            }
        }
        return;
    }
    //有行号
    if(newLineNode->lineCommand==RUN||newLineNode->lineCommand==LOAD||newLineNode->lineCommand==CLEAR||newLineNode->lineCommand==HELP||newLineNode->lineCommand==QUIT||newLineNode->lineCommand==LIST)
    {
        error("You Don't Need To Input A LineNumber!");
        delete endPos;
        delete newLineNode;
        return;
    }
    switch(newLineNode->lineCommand)
    {
    case BLANK:




        program->removeNode(newLineNode->lineNumber);
        break;
    case STATEWRONG:
        error("Wrong Statement!");
        return;
        break;
    case REM:
        dealREMStatement(inputCode,newLineNode,endPos);
        break;
    case LET:
        /*if(dealLETStatement(inputCode,newLineNode,endPos))
            break;
        else
            return;*/
        dealLETStatement(inputCode,newLineNode,endPos);
        break;
    case PRINT:
        /*if(dealPRINTStatement(inputCode,newLineNode,endPos))
            break;
        else
            return;*/
        dealPRINTStatement(inputCode,newLineNode,endPos);
        break;
    case PRINTF:
        dealPRINTFStatement(inputCode,newLineNode,endPos);
        break;
    case INPUT:
        /*if(dealINPUTStatement(inputCode,newLineNode,endPos))
            break;
        else
            return;*/
        dealINPUTStatement(inputCode,newLineNode,endPos);
        break;
    case INPUTS:
        dealINPUTSStatement(inputCode,newLineNode,endPos);
        break;
    case GOTO:
        /*if(dealGOTOStatement(inputCode,newLineNode,endPos))
            break;
        else
            return;*/
        dealGOTOStatement(inputCode,newLineNode,endPos);
        break;
    case IF:
        /*if(dealIFStatement(inputCode,newLineNode,endPos))
            break;
        else
            return;*/
        dealIFStatement(inputCode,newLineNode,endPos);
        break;
    case END:
        /*if(dealENDStatement(inputCode,newLineNode,endPos))
            break;
        else
            return;*/
        dealENDStatement(inputCode,newLineNode,endPos);
        break;
    default:
        break;
    }
    renewCodeBrowser();
    renewVarBrowser();
    return;
}
void MainWindow::dealREMStatement(string &inputCode,lineNode *newLineNode,int* endPos)
{
    newLineNode->statement=new REMStatement(tokenizer->getRem(inputCode,endPos));
    program->addNode(newLineNode);
    delete endPos;
    return;
}
bool MainWindow::dealLETStatement(string &inputCode,lineNode *newLineNode,int* endPos)
{
    //找到变量
    string varName=tokenizer->getVar(inputCode,endPos);
    if(varName=="1")
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    ++(*endPos);
    //找到等于号
    while('\0'!=inputCode[*endPos])
    {
        if(inputCode[*endPos] == '=')
        {
            break;
        }
        else if(inputCode[*endPos] == ' ')
        {
            ++(*endPos);
        }
        else
        {
            newLineNode->errorNumber = 1;
            program->addNode(newLineNode);
            delete endPos;
            return false;
        }
    }
    ++(*endPos);
    int end = *endPos;
    bool isString = false;
    //判断输入的是表达式还是字符串
    while('\0'!=inputCode[*endPos])
    {
        if(inputCode[*endPos] == '\'' || inputCode[*endPos] == '"')
        {
            isString = true;
            break;
        }
        else if(inputCode[*endPos] == ' ')
        {
            ++(*endPos);
        }
        else
        {
            break;
        }
    }
    if(isString)
    {
        string stringValue = tokenizer->dealString(inputCode,endPos);
        if(stringValue == "'")
        {
            newLineNode->errorNumber = 1;
            program->addNode(newLineNode);
            delete endPos;
            return false;
        }
        else
        {
            newLineNode->statement=new LETStatement(varName, stringValue, nullptr);
            program->addNode(newLineNode);
            delete endPos;
            return true;
        }
    }
    else
    {
        //复位
        *endPos = end;
        //处理表达式
        Expression *letexp = nullptr;
        letexp = tokenizer->dealExpression(inputCode,endPos);
        if(letexp==nullptr)
        {
            /*error("Wrong Expression!");
            delete endPos;
            delete newLineNode;
            return false;*/
            newLineNode->errorNumber = 1;
            program->addNode(newLineNode);
            delete endPos;
            return false;
        }
        else
        {
            newLineNode->statement=new LETStatement(varName, "'", letexp);
            program->addNode(newLineNode);
            delete endPos;
            return true;
        }
    }
}
bool MainWindow::dealINPUTStatement(string &inputCode,lineNode *newLineNode,int* endPos)
{
    string varName=tokenizer->getVar(inputCode,endPos);
    if(varName=="1")
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    if(!tokenizer->judgeCodeEnd(inputCode,endPos))
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    newLineNode->statement=new INPUTStatement(varName);
    program->addNode(newLineNode);
    delete endPos;
    return true;
}
bool MainWindow::dealINPUTSStatement(string &inputCode,lineNode *newLineNode,int* endPos)
{
    string varName=tokenizer->getVar(inputCode,endPos);
    if(varName=="1")
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    if(!tokenizer->judgeCodeEnd(inputCode,endPos))
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    newLineNode->statement=new INPUTSStatement(varName);
    program->addNode(newLineNode);
    delete endPos;
    return true;
}
bool MainWindow::dealPRINTStatement(string &inputCode, lineNode *newLineNode, int *endPos)
{
    Expression *printexp = nullptr;
    printexp = tokenizer->dealExpression(inputCode,endPos);
    if(printexp==nullptr)
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    else
    {
        newLineNode->statement=new PRINTStatement(printexp);
        program->addNode(newLineNode);
        delete endPos;
        return true;
    }
}
bool MainWindow::dealPRINTFStatement(string &inputCode, lineNode *newLineNode, int *endPos)
{
    vector<string> *p1 = new vector<string>;
    vector<printfNode> *p2 = new vector<printfNode>;
    string *p3 = new string;
    bool success = tokenizer->dealPrintf(p1, p2, p3, inputCode, endPos);
    if(!success)
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    else
    {
        if(*p3 == "")
        {
            newLineNode->statement=new PRINTFStatement(nullptr, nullptr, "");
            delete p1;
            delete p2;
            delete p3;
        }
        else
            newLineNode->statement=new PRINTFStatement(p1, p2, *p3);
        program->addNode(newLineNode);
        delete p3;
        delete endPos;
        return true;
    }
}
bool MainWindow::dealGOTOStatement(string &inputCode, lineNode *newLineNode, int *endPos)
{
    int number=tokenizer->getLineNumber(inputCode,endPos);
    if(number==-1)
    {
        /*error("Wrong Target Line Number!");
        delete endPos;
        delete newLineNode;
        return false;*/
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    if(!tokenizer->judgeCodeEnd(inputCode,endPos))
    {
        /*error("Wrong GOTO Statement!");
        delete endPos;
        delete newLineNode;
        return false;*/
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    newLineNode->statement=new GOTOStatement(number);
    program->addNode(newLineNode);
    delete endPos;
    return true;
}
bool MainWindow::dealENDStatement(string &inputCode, lineNode *newLineNode, int *endPos)
{
    if(!tokenizer->judgeCodeEnd(inputCode,endPos))
    {
        /*error("Wrong END Statement!");
        delete endPos;
        delete newLineNode;
        return false;*/
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    newLineNode->statement=new ENDStatement();
    program->addNode(newLineNode);
    delete endPos;
    return true;
}
bool MainWindow::dealIFStatement(string &inputCode, lineNode *newLineNode, int *endPos)
{
    int start = (*endPos), end;
    Expression *exp1 = nullptr, *exp2 = nullptr;
    string code,input1,input2;
    code = inputCode;
    string op="\0";
    if(tokenizer->PosChar(inputCode,'>',endPos))
    {
        op=">";
    }
    else if(tokenizer->PosChar(inputCode,'<',endPos))
    {
        op="<";
    }
    else if(tokenizer->PosChar(inputCode,'=',endPos))
    {
        op="=";
    }
    else
    {
        /*error("Wrong IF Statement!");
        delete endPos;
        delete newLineNode;
        return false;*/
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        return false;
    }
    end = (*endPos) - 1;
    while(inputCode[start]!='\0'&&start<=end)
    {
        if(inputCode[start]==' ')
        {
            ++start;
            continue;
        }
        input1.push_back(inputCode[start]);
        ++start;
    }
    input1.push_back('\0');
    int *tempPos = new int(0);
    exp1=tokenizer->dealExpression(input1,tempPos);
    start= (*endPos) + 1;
    bool flag=false;
    int len = code.length();
    while(start + 3 < len)//找THEN
    {
        if(code[start]=='T'&&code[start + 1]=='H'&&code[start + 2]=='E'&&code[start + 3]=='N')
        {
            if(code[start - 1] != ' ')
            {
                newLineNode->errorNumber = 1;
                program->addNode(newLineNode);
                delete endPos;
                delete tempPos;
                return false;
            }
            end = start - 1;
            flag = true;
            break;
        }
        ++start;
    }
    if(!flag)
    {
        /*error("Wrong IF Statement!");
        delete endPos;
        delete tempPos;
        delete newLineNode;
        return false;*/
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        delete tempPos;
        return false;
    }
    start = (*endPos) + 1;
    *endPos = end + 5;
    if(code[*endPos] != ' ')
    {
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        delete tempPos;
        return false;
    }
    (*endPos)++;
    while(code[start]!='\0'&&start<=end)
    {
        if(code[start]==' ')
        {
            ++start;
            continue;
        }
        input2.push_back(code[start]);
        ++start;
    }
    input2.push_back('\0');
    *tempPos = 0;
    exp2=tokenizer->dealExpression(input2,tempPos);
    int lineNumber=tokenizer->getLineNumber(code,endPos);
    if(lineNumber==-1)
    {
        /*error("Wrong Target Line Number!");
        delete endPos;
        delete tempPos;
        delete newLineNode;
        return false;*/
        newLineNode->errorNumber = 1;
        program->addNode(newLineNode);
        delete endPos;
        delete tempPos;
        return false;
    }
    newLineNode->statement=new IFStatement(exp1,exp2,op,lineNumber);
    program->addNode(newLineNode);
    delete endPos;
    delete tempPos;
    return true;
}
void MainWindow::dealImmediateLET(string &inputCode, int *endPos)
{
    //找到变量
    string varName=tokenizer->getVar(inputCode,endPos);
    if(varName=="1")
    {
        error("Wrong Variable Name!");
        delete endPos;
        return;
    }
    ++(*endPos);
    //找到等于号
    while('\0'!=inputCode[*endPos])
    {
        if(inputCode[*endPos] == '=')
        {
            break;
        }
        else if(inputCode[*endPos] == ' ')
        {
            ++(*endPos);
        }
        else
        {
            error("Wrong LET Statement!");
            delete endPos;
            return;
        }
    }
    ++(*endPos);
    int end = *endPos;
    bool  isString = false;
    //判断输入的是表达式还是字符串
    while('\0'!=inputCode[*endPos])
    {
        if(inputCode[*endPos] == '\'' || inputCode[*endPos] == '"')
        {
            isString = true;
            break;
        }
        else if(inputCode[*endPos] == ' ')
        {
            ++(*endPos);
        }
        else
        {
            break;
        }
    }
    if(isString)
    {
        string stringValue = tokenizer->dealString(inputCode,endPos);
        if(stringValue == "'")
        {
            error("Wrong String Value!");
            delete endPos;
            return;
        }
        else
        {
            if(evalContext->isDefined(varName))
            {
                error("Duplicate Defined Variable!");
                delete endPos;
                return;
            }
            else
            {
                stringTable->setValue(varName, stringValue);
                delete endPos;
                return;
            }
        }
    }
    else
    {
        (*endPos) = end;
        //处理表达式
        Expression *letexp=tokenizer->dealExpression(inputCode,endPos);
        if(letexp==nullptr)
        {
            error("Wrong Expression!");
            delete endPos;
            return;
        }
        if(stringTable->isDefined(varName))
        {
            error("Duplicate Defined Variable!");
            delete endPos;
            return;
        }
        else
        {
            evalContext->setValue(varName,letexp->eval(*evalContext));
            delete endPos;
            return;
        }
    }
}
void MainWindow::dealImmediateINPUT(string &inputCode, int *endPos)
{
    string varName = tokenizer->getVar(inputCode,endPos);
    if(varName=="1")
    {
        error("Wrong Variable Name!");
        delete endPos;
        return;
    }
    if(!tokenizer->judgeCodeEnd(inputCode,endPos))
    {
        error("Wrong INPUT Statement!");
        delete endPos;
        return;
    }
    if(stringTable->isDefined(varName))
    {
        error("Duplicate Defined Variable!");
        delete endPos;
        return;
    }
    bool flag = false;
    int varValue;
    ui->loadButton->setDisabled(true);
    ui->runButton->setDisabled(true);
    ui->debugButton->setDisabled(true);
    while(!flag)
    {
        ui->codeEdit->setText(" ? ");
        ui->codeEdit->setFocus();
        loop.exec();
        varValue = getInputValue(flag);
        ui->codeEdit->clear();
        if(flag)
            break;
        error("Wrong INPUT Value!");
    }
    ui->loadButton->setEnabled(true);
    ui->runButton->setEnabled(true);
    ui->debugButton->setEnabled(true);
    evalContext->setValue(varName, varValue);
    //ui->resultBrowser->clear();
    delete endPos;
    return;
}
void MainWindow::dealImmediateINPUTS(string &inputCode, int *endPos)
{
    string varName = tokenizer->getVar(inputCode,endPos);
    if(varName=="1")
    {
        error("Wrong Variable Name!");
        delete endPos;
        return;
    }
    if(!tokenizer->judgeCodeEnd(inputCode,endPos))
    {
        error("Wrong INPUT Statement!");
        delete endPos;
        return;
    }
    if(evalContext->isDefined(varName))
    {
        error("Duplicate Defined Variable!");
        delete endPos;
        return;
    }
    string stringValue;
    ui->loadButton->setDisabled(true);
    ui->runButton->setDisabled(true);
    ui->debugButton->setDisabled(true);
    while(1)
    {
        ui->codeEdit->setText(" ? ");
        ui->codeEdit->setFocus();
        loop.exec();
        stringValue = getInputStringValue();
        ui->codeEdit->clear();
        if(stringValue[0] != '\'')
            break;
        error("Wrong INPUTS String Value!");
    }
    ui->loadButton->setEnabled(true);
    ui->runButton->setEnabled(true);
    ui->debugButton->setEnabled(true);
    stringTable->setValue(varName, stringValue);
    delete endPos;
    return;
}
void MainWindow::dealImmediatePRINT(string &inputCode, int *endPos)
{
    Expression *printexp = nullptr;
    printexp = tokenizer->dealExpression(inputCode,endPos);
    if(printexp != nullptr)
    {
        if(printexp->judge(*evalContext))
        {
            int result = printexp->eval(*evalContext);
            QString qstr = QString::number(result);
            ui->resultBrowser->append(qstr);
            delete endPos;
            delete printexp;
            return;
        }
        else if(printexp->type() == IDENTIFIER && stringTable->isDefined(printexp->getIdentifierName()))
        {
            string result = stringTable->getValue(printexp->getIdentifierName());
            QString qstr = QString::fromStdString(result);
            ui->resultBrowser->append(qstr);
            delete printexp;
            delete endPos;
            return;
        }
    }
    else
    {
        error("Wrong Expression!");
        delete endPos;
        return;
    }
}
void MainWindow::dealImmediatePRINTF(string &inputCode, int *endPos)
{
    vector<string> *p1 = new vector<string>;
    vector<printfNode> *p2 = new vector<printfNode>;
    string *p3 = new string;
    bool success = tokenizer->dealPrintf(p1, p2, p3, inputCode, endPos);
    if(!success)
    {
        error("Wrong PRINTF Statement!");
        delete endPos;
        delete p1;
        delete p2;
        delete p3;
        return;
    }
    if(*p3 == "")
    {
        ui->resultBrowser->append(QString::fromStdString(""));
        delete endPos;
        delete p1;
        delete p2;
        delete p3;
        return;
    }
    if(p1->size() != 0 && p2->size() == 0)
    {
        ui->resultBrowser->append(QString::fromStdString((*p1)[0]));
        delete endPos;
        delete p1;
        delete p2;
        delete p3;
        return;
    }
    int length2 = p2->size();
    int i = 0;
    string result;
    for(; i < length2; ++i)
    {
        /*if((*p1)[i] != "")
            result += (*p1)[i];*/
        result += (*p1)[i];
        if((*p2)[i].type == 1 || (*p2)[i].type == 0 /*&& (*p2)[i].parameter != "'"*/)
            result += (*p2)[i].parameter;
        else if((*p2)[i].type == 2)
        {
            string varName = (*p2)[i].parameter;
            if(evalContext->isDefined(varName))
                result += std::to_string(evalContext->getValue(varName));
            else if(stringTable->isDefined(varName))
                result += stringTable->getValue(varName);
            else
            {
                error("Undefined Variables!");
                delete endPos;
                delete p1;
                delete p2;
                delete p3;
                return;
            }
        }
    }
    result += (*p1)[i];
    ui->resultBrowser->append(QString::fromStdString(result));
    delete endPos;
    delete p1;
    delete p2;
    delete p3;
    return;
}
//输入新的语句后刷新code展示栏
void MainWindow::renewCodeBrowser()
{
    ui->codeBrowser->clear();
    ui->resultBrowser->clear();
    ui->treeBrowser->clear();
    lineNode *tempNode=program->head->next;
    while(tempNode!=program->tail)
    {
        /*std::string str;
        str=std::to_string(tempNode->lineNumber)+tempNode->statement->toString();
        QString qstr=QString::fromStdString(str);
        ui->codeBrowser->append(qstr);
        tempNode=tempNode->next;*/
        ui->codeBrowser->append(tempNode->code);
        tempNode=tempNode->next;
    }
}
//展示当前变量的值
void MainWindow::renewVarBrowser()
{
    ui->varBrowser->clear();
    QString qstr;
    map<string, string> stringList = stringTable->traverse();
    if(!stringList.empty())
    {
        map<std::string, std::string>::iterator iter1;
        for(iter1 = stringList.begin(); iter1 != stringList.end(); ++iter1)
        {
            QString qstr = QString::fromStdString(iter1->first + ": STR = \"" + iter1->second + "\"");
            ui->varBrowser->append(qstr);
        }
    }
    map<string, int> intList = evalContext->traverse();
    if(!intList.empty())
    {
        map<std::string, int>::iterator iter2;
        for(iter2 = intList.begin(); iter2 != intList.end(); ++iter2)
        {
            qstr = QString::fromStdString(iter2->first + ": INT = " + to_string(iter2->second));
            ui->varBrowser->append(qstr);
        }
    }
}
//clear按钮
void MainWindow::on_clearButton_clicked()
{
    program->clear();
    evalContext->clear();
    stringTable->clear();
    extras1.clear();
    extras2.clear();
    ui->resultBrowser->clear();
    ui->codeBrowser->clear();
    ui->treeBrowser->clear();
    ui->varBrowser->clear();
    ui->codeEdit->clear();
    ui->stepButton->setDisabled(true);
    ui->codeEdit->setEnabled(true);
    ui->loadButton->setEnabled(true);
    ui->runButton->setEnabled(true);
    ui->debugButton->setEnabled(true);
}
//load按钮
void MainWindow::on_loadButton_clicked()
{
    on_clearButton_clicked();
    //qDebug()<<"open file...";
    fileDialog->setWindowTitle(tr("Load A File"));//定义文件对话框标题
    fileDialog->setDirectory(".");//设置默认文件路径
    fileDialog->setNameFilter(tr("text(*.txt *.doc *.docx *.cpp)"));//设置文件过滤器
    /*设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);*/
    fileDialog->setViewMode(QFileDialog::Detail);//设置视图模式


    QString filePath;
    if (fileDialog->exec()==QDialog::Accepted)
    {
        filePath=fileDialog->selectedFiles()[0];
        //ui->codeBrowser->setText(filePath);
        //QFileInfo *fileInfo=new QFileInfo(filePath);
        fileText = new QFile(filePath);
        //QFile fileText(filePath);
        if(!fileText->open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QString qstr = QString::fromStdString("Load File Fail");
            ui->codeBrowser->append(qstr);
            delete fileText;
            return;
        }
        program->clear();
        readStream = new QTextStream(fileText);
        while(!readStream->atEnd())
        {
            QString inputCode=readStream->readLine();
            dealNewLine(inputCode);
        }
        renewCodeBrowser();
        delete fileText;
        delete readStream;
    }
    return;
}
//run按钮
void MainWindow::on_runButton_clicked()
{
    if(debugModel)
    {
        ui->treeBrowser->clear();
        ui->stepButton->setDisabled(true);
        ui->runButton->setDisabled(true);
        lineNode *temp = currentLineNode;
        while(temp != program->tail)
        {
            if(!temp->errorNumber)
            {
                createSyntaxTree(temp);
            }
            else
            {
                string str = std::to_string(temp->lineNumber) + " ERROR";
                QString qstr = QString::fromStdString(str);
                ui->treeBrowser->append(qstr);
            }
            temp = temp->next;
        }
        int nextLine;
        while(currentLineNode != program->tail)
        {
            nextLine = -1;
            if(currentLineNode->errorNumber)
            {
                debugModel = false;
                QMessageBox msgBox;
                string str = "Line " + std::to_string(currentLineNode->lineNumber) + " Wrong Statement";
                msgBox.setText(QString::fromStdString(str));
                msgBox.exec();
                return;
            }
            else
            {
                //createSyntaxTree(currentLineNode);
                switch(currentLineNode->lineCommand)
                {
                case REM:
                {
                    break;
                }
                case LET:
                {
                    Expression *exp = nullptr;
                    currentLineNode->statement->get1Expression(exp);
                    string varName = currentLineNode->statement->getVarName();
                    if(exp != nullptr && exp->judge(*evalContext))
                    {
                        if(stringTable->isDefined(varName))
                        {
                            debugModel = false;
                            string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                            QMessageBox msgBox;
                            msgBox.setText(QString::fromStdString(str));
                            msgBox.exec();
                            return;
                        }
                        else
                            evalContext->setValue(currentLineNode->statement->getVarName(), exp->eval(*evalContext));
                    }
                    else if(exp == nullptr && currentLineNode->statement->getStringValue() != "'")
                    {
                        if(evalContext->isDefined(varName))
                        {
                            debugModel = false;
                            string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                            QMessageBox msgBox;
                            msgBox.setText(QString::fromStdString(str));
                            msgBox.exec();
                            return;
                        }
                        else
                            stringTable->setValue(varName, currentLineNode->statement->getStringValue());
                    }
                    else
                    {
                        debugModel = false;
                        string str = "Line " + std::to_string(currentLineNode->lineNumber) + " Wrong Expression";
                        QMessageBox msgBox;
                        msgBox.setText(QString::fromStdString(str));
                        msgBox.exec();
                        return;
                    }
                    break;
                }
                case INPUT:
                {
                    string varName = currentLineNode->statement->getVarName();
                    if(stringTable->isDefined(varName))
                    {
                        debugModel = false;
                        string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                        QMessageBox msgBox;
                        msgBox.setText(QString::fromStdString(str));
                        msgBox.exec();
                        return;
                    }
                    bool flag = false;
                    int varValue;
                    ui->codeEdit->setEnabled(true);
                    while(!flag)
                    {
                        ui->codeEdit->setText(" ? ");
                        ui->codeEdit->setFocus();
                        loop.exec();
                        varValue = getInputValue(flag);
                        ui->codeEdit->clear();
                        if(flag)
                            break;
                    }
                    ui->codeEdit->setDisabled(true);
                    evalContext->setValue(varName, varValue);
                    break;
                }
                case INPUTS:
                {
                    string varName = currentLineNode->statement->getVarName();
                    if(evalContext->isDefined(varName))
                    {
                        debugModel = false;
                        string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                        QMessageBox msgBox;
                        msgBox.setText(QString::fromStdString(str));
                        msgBox.exec();
                        return;
                    }
                    string stringValue;
                    ui->codeEdit->setEnabled(true);
                    while(1)
                    {
                        ui->codeEdit->setText(" ? ");
                        ui->codeEdit->setFocus();
                        loop.exec();
                        stringValue = getInputStringValue();
                        ui->codeEdit->clear();
                        if(stringValue[0] != '\'')
                            break;
                    }
                    ui->codeEdit->setDisabled(true);
                    stringTable->setValue(varName, stringValue);
                    break;
                }
                case PRINT:
                {
                    Expression *exp=nullptr;
                    currentLineNode->statement->get1Expression(exp);
                    if(exp != nullptr)
                    {
                        if(exp->judge(*evalContext))//输出数字
                        {
                            int result = exp->eval(*evalContext);
                            QString qstr = QString::number(result);
                            ui->resultBrowser->append(qstr);
                            break;
                        }
                        else if(exp->type() == IDENTIFIER && stringTable->isDefined(exp->getIdentifierName()))//输出字符串
                        {
                            string result = stringTable->getValue(exp->getIdentifierName());
                            QString qstr = QString::fromStdString(result);
                            ui->resultBrowser->append(qstr);
                            break;
                        }
                    }
                    debugModel = false;
                    string str="Line "+std::to_string(currentLineNode->lineNumber)+" Wrong Expression";
                    QMessageBox msgBox;
                    msgBox.setText(QString::fromStdString(str));
                    msgBox.exec();
                    return;
                }
                case PRINTF:
                {
                    vector<string> *p1 = nullptr;
                    vector<printfNode> *p2 = nullptr;
                    currentLineNode->statement->get2Vector(p1, p2);
                    if(p1 == nullptr)
                    {
                        ui->resultBrowser->append(QString::fromStdString(""));
                        break;
                    }
                    if(p1->size() != 0 && p2->size() == 0)
                    {
                        ui->resultBrowser->append(QString::fromStdString((*p1)[0]));
                        break;
                    }
                    int length2 = p2->size();
                    int i = 0;
                    string result;
                    for(; i < length2; ++i)
                    {
                        result += (*p1)[i];
                        if((*p2)[i].type == 1 || (*p2)[i].type == 0)
                            result += (*p2)[i].parameter;
                        else if((*p2)[i].type == 2)
                        {
                            string varName = (*p2)[i].parameter;
                            if(evalContext->isDefined(varName))
                                result += std::to_string(evalContext->getValue(varName));
                            else if(stringTable->isDefined(varName))
                                result += stringTable->getValue(varName);
                            else
                            {
                                debugModel = false;
                                QMessageBox msgBox;
                                string str = "Line " + std::to_string(currentLineNode->lineNumber) + "Undefined Variables!";
                                msgBox.setText(QString::fromStdString(str));
                                msgBox.exec();
                                return;
                            }
                        }
                    }
                    result += (*p1)[i];
                    ui->resultBrowser->append(QString::fromStdString(result));
                    break;
                }
                case GOTO:
                {
                    nextLine=currentLineNode->statement->getTargetLineNumber();
                    break;
                }
                case IF:
                {
                    Expression *exp1=nullptr,*exp2=nullptr;
                    currentLineNode->statement->get2Expression(exp1,exp2);
                    if(exp1 != nullptr && exp2 != nullptr && exp1->judge(*evalContext) && exp2->judge(*evalContext))
                    {
                        int result1,result2;
                        string op;
                        op=currentLineNode->statement->getVarName();
                        result1=exp1->eval(*evalContext);
                        result2=exp2->eval(*evalContext);
                        if(op == "=")
                        {
                            if(result1 == result2)
                            {
                                nextLine=currentLineNode->statement->getTargetLineNumber();
                            }
                        }
                        else if(op == "<")
                        {
                            if(result1 < result2)
                            {
                                nextLine=currentLineNode->statement->getTargetLineNumber();
                            }
                        }
                        else if(op == ">")
                        {
                            if(result1 > result2)
                            {
                                nextLine=currentLineNode->statement->getTargetLineNumber();
                            }
                        }
                        else
                        {
                            debugModel = false;
                            string str="Line "+std::to_string(currentLineNode->lineNumber)+" Wrong Symbol";
                            QMessageBox msgBox;
                            msgBox.setText(QString::fromStdString(str));
                            msgBox.exec();
                            return;
                        }
                    }
                    else
                    {
                        debugModel = false;
                        string str="Line "+std::to_string(currentLineNode->lineNumber)+" Wrong Expression";
                        QMessageBox msgBox;
                        msgBox.setText(QString::fromStdString(str));
                        msgBox.exec();
                        return;
                    }
                    break;
                }
                case END:
                {
                    return;
                }
                }
                renewVarBrowser();
                if(nextLine == -1)
                    currentLineNode = currentLineNode->next;
                else
                {
                    bool flag=false;
                    currentLineNode = program->head->next;
                    while(currentLineNode != program->tail)
                    {
                        if(currentLineNode->lineNumber == nextLine)
                        {
                            flag=true;
                            break;
                        }
                        currentLineNode = currentLineNode->next;
                    }
                    if(!flag)
                    {
                        debugModel = false;
                        QMessageBox msgBox;
                        string str = "Line " + std::to_string(currentLineNode->lineNumber) + " Wrong Target Number";
                        msgBox.setText(QString::fromStdString(str));
                        msgBox.exec();
                        return;
                    }
                }
            }
        }
        return;
    }
    else
    {
        //初始化
        ui->treeBrowser->clear();
        ui->resultBrowser->clear();
        extras1.clear();
        //输出表达式树
        lineNode *currentLine = program->head->next;
        while(currentLine != program->tail)
        {
            if(!currentLine->errorNumber)
            {
                createSyntaxTree(currentLine);
            }
            else
            {
                string str = std::to_string(currentLine->lineNumber) + " ERROR";
                QString qstr = QString::fromStdString(str);
                ui->treeBrowser->append(qstr);
                highlightRed(currentLine->lineNumber,extras1);
            }
            currentLine = currentLine->next;
        }
        //运行
        currentLine = program->head->next;
        int nextLine;
        while(currentLine != program->tail)
        {
            if(currentLine->errorNumber)
            {
                string str = "Line " + std::to_string(currentLine->lineNumber) + " Wrong Statement";
                error(str);
                currentLine = nullptr;
                return;
            }
            nextLine = -1;
            switch(currentLine->lineCommand)
            {
            case REM:
            {
                break;
            }
            case LET:
            {
                Expression *exp = nullptr;
                currentLine->statement->get1Expression(exp);
                string varName = currentLine->statement->getVarName();
                if(exp != nullptr && exp->judge(*evalContext))
                {
                    if(stringTable->isDefined(varName))
                    {
                        string str="Line "+std::to_string(currentLine->lineNumber)+" Duplicate Defined Variable";
                        error(str);
                        currentLine = nullptr;
                        return;
                    }
                    else
                        evalContext->setValue(currentLine->statement->getVarName(), exp->eval(*evalContext));
                }
                else if(exp == nullptr && currentLine->statement->getStringValue() != "'")
                {
                    if(evalContext->isDefined(varName))
                    {
                        string str="Line "+std::to_string(currentLine->lineNumber)+" Duplicate Defined Variable";
                        error(str);
                        currentLine = nullptr;
                        return;
                    }
                    else
                        stringTable->setValue(varName, currentLine->statement->getStringValue());
                }
                else
                {
                    string str = "Line " + std::to_string(currentLine->lineNumber) + " Wrong Expression";
                    error(str);
                    currentLine = nullptr;
                    return;
                }
                break;
            }
            case INPUT:
            {
                string varName = currentLine->statement->getVarName();
                if(stringTable->isDefined(varName))
                {
                    string str="Line "+std::to_string(currentLine->lineNumber)+" Duplicate Defined Variable";
                    error(str);
                    currentLine = nullptr;
                    return;
                }
                bool flag = false;
                int varValue;
                ui->loadButton->setDisabled(true);
                ui->runButton->setDisabled(true);
                ui->debugButton->setDisabled(true);
                while(!flag)
                {
                    ui->codeEdit->setText(" ? ");
                    ui->codeEdit->setFocus();
                    loop.exec();
                    varValue = getInputValue(flag);
                    ui->codeEdit->clear();
                    if(flag)
                        break;
                }
                ui->loadButton->setEnabled(true);
                ui->runButton->setEnabled(true);
                ui->debugButton->setEnabled(true);
                evalContext->setValue(varName, varValue);
                break;
            }
            case INPUTS:
            {
                string varName = currentLine->statement->getVarName();
                if(evalContext->isDefined(varName))
                {
                    string str="Line "+std::to_string(currentLine->lineNumber)+" Duplicate Defined Variable";
                    error(str);
                    currentLine = nullptr;
                    return;
                }
                string stringValue;
                ui->loadButton->setDisabled(true);
                ui->runButton->setDisabled(true);
                ui->debugButton->setDisabled(true);
                while(1)
                {
                    ui->codeEdit->setText(" ? ");
                    ui->codeEdit->setFocus();
                    loop.exec();
                    stringValue = getInputStringValue();
                    ui->codeEdit->clear();
                    if(stringValue[0] != '\'')
                        break;
                }
                ui->loadButton->setEnabled(true);
                ui->runButton->setEnabled(true);
                ui->debugButton->setEnabled(true);
                stringTable->setValue(varName, stringValue);
                break;
            }
            case PRINT:
            {
                Expression *exp=nullptr;
                currentLine->statement->get1Expression(exp);
                if(exp != nullptr)
                {
                    if(exp->judge(*evalContext))//输出数字
                    {
                        int result = exp->eval(*evalContext);
                        QString qstr = QString::number(result);
                        ui->resultBrowser->append(qstr);
                        break;
                    }
                    else if(exp->type() == IDENTIFIER && stringTable->isDefined(exp->getIdentifierName()))//输出字符串
                    {
                        string result = stringTable->getValue(exp->getIdentifierName());
                        QString qstr = QString::fromStdString(result);
                        ui->resultBrowser->append(qstr);
                        break;
                    }
                }
                string str="Line "+std::to_string(currentLine->lineNumber)+" Wrong Expression";
                error(str);
                currentLine = nullptr;
                return;
            }
            case PRINTF:
            {
                vector<string> *p1 = nullptr;
                vector<printfNode> *p2 = nullptr;
                //string p3 = currentLine->statement->getStringValue();
                currentLine->statement->get2Vector(p1, p2);
                if(p1 == nullptr)
                {
                    ui->resultBrowser->append(QString::fromStdString(""));
                    break;
                }
                if(p1->size() != 0 && p2->size() == 0)
                {
                    ui->resultBrowser->append(QString::fromStdString((*p1)[0]));
                    break;
                }
                int length2 = p2->size();
                int i = 0;
                string result;
                for(; i < length2; ++i)
                {
                    result += (*p1)[i];
                    if((*p2)[i].type == 1 || (*p2)[i].type == 0)
                        result += (*p2)[i].parameter;
                    else if((*p2)[i].type == 2)
                    {
                        string varName = (*p2)[i].parameter;
                        if(evalContext->isDefined(varName))
                            result += std::to_string(evalContext->getValue(varName));
                        else if(stringTable->isDefined(varName))
                            result += stringTable->getValue(varName);
                        else
                        {
                            error("Line " + std::to_string(currentLine->lineNumber) + "Undefined Variables!");
                            return;
                        }
                    }
                }
                result += (*p1)[i];
                ui->resultBrowser->append(QString::fromStdString(result));
                break;
            }
            case GOTO:
            {
                nextLine=currentLine->statement->getTargetLineNumber();
                break;
            }
            case IF:
            {
                Expression *exp1=nullptr,*exp2=nullptr;
                currentLine->statement->get2Expression(exp1,exp2);
                if(exp1 != nullptr && exp2 != nullptr && exp1->judge(*evalContext) && exp2->judge(*evalContext))
                {
                    int result1,result2;
                    string op;
                    op=currentLine->statement->getVarName();
                    result1=exp1->eval(*evalContext);
                    result2=exp2->eval(*evalContext);
                    if(op == "=")
                    {
                        if(result1 == result2)
                        {
                            nextLine=currentLine->statement->getTargetLineNumber();
                        }
                    }
                    else if(op == "<")
                    {
                        if(result1 < result2)
                        {
                            nextLine=currentLine->statement->getTargetLineNumber();
                        }
                    }
                    else if(op == ">")
                    {
                        if(result1 > result2)
                        {
                            nextLine=currentLine->statement->getTargetLineNumber();
                        }
                    }
                    else
                    {
                        string str="Line "+std::to_string(currentLine->lineNumber)+" Wrong Symbol";
                        error(str);
                        currentLine = nullptr;
                        return;
                    }
                }
                else
                {
                    string str="Line "+std::to_string(currentLine->lineNumber)+" Wrong Expression";
                    error(str);
                    currentLine = nullptr;
                    return;
                }
                break;
            }
            case END:
            {
                return;
            }
            }
            renewVarBrowser();
            //判断nextLine
            if(nextLine == -1)
                currentLine = currentLine->next;
            else
            {
                bool flag=false;
                currentLine = program->head->next;
                while(currentLine != program->tail)
                {
                    if(currentLine->lineNumber == nextLine)
                    {
                        flag=true;
                        break;
                    }
                    currentLine = currentLine->next;
                }
                if(!flag)
                {
                    error("Wrong Target Line Number");
                    return;
                }
            }
        }
        return;
    }
}
//构造表达式树
void MainWindow::createSyntaxTree(lineNode *currentLine)
{
    switch(currentLine->lineCommand)
    {
    case REM:
    {
        string str=std::to_string(currentLine->lineNumber)+" REM";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        qstr = QString::fromStdString("\t"+currentLine->statement->getVarName());
        ui->treeBrowser->append(qstr);
        break;
    }
    case LET:
    {
        string str=std::to_string(currentLine->lineNumber)+" LET =";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        Expression *exp1=nullptr;
        currentLine->statement->get1Expression(exp1);
        if(exp1 == nullptr)
        {
            string str = "\t" + currentLine->statement->getVarName();
            QString qstr = QString::fromStdString(str);
            ui->treeBrowser->append(qstr);
            str = "\t\"" + currentLine->statement->getStringValue() + "\"";
            qstr = QString::fromStdString(str);
            ui->treeBrowser->append(qstr);
        }
        else
            createExpressionTree(exp1,1);
        break;
    }
    case INPUT:
    {
        string str=std::to_string(currentLine->lineNumber)+" INPUT";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        qstr = QString::fromStdString("\t"+currentLine->statement->getVarName());
        ui->treeBrowser->append(qstr);
        break;
    }
    case INPUTS:
    {
        string str=std::to_string(currentLine->lineNumber)+" INPUTS";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        qstr = QString::fromStdString("\t"+currentLine->statement->getVarName());
        ui->treeBrowser->append(qstr);
        break;
    }
    case PRINTF:
    {
        string str=std::to_string(currentLine->lineNumber)+" PRINTF";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        vector<string> *p1 = nullptr;
        vector<printfNode> *p2 = nullptr;
        currentLine->statement->get2Vector(p1, p2);
        string p3 = currentLine->statement->getStringValue();
        ui->treeBrowser->append(QString::fromStdString("\t\"" + p3 + "\""));
        if(p2 != nullptr && p2->size() > 0)
        {
            int len = p2->size();
            for(int i = 0; i < len; ++i)
            {
                if((*p2)[i].type == 0)
                    ui->treeBrowser->append(QString::fromStdString("\t\"" + (*p2)[i].parameter + "\""));
                else
                    ui->treeBrowser->append(QString::fromStdString("\t" + (*p2)[i].parameter));
            }
        }
        break;
    }
    case PRINT:
    {
        string str=std::to_string(currentLine->lineNumber)+" PRINT";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        Expression *exp1=nullptr;
        currentLine->statement->get1Expression(exp1);
        createExpressionTree(exp1,1);
        break;
    }
    case GOTO:
    {
        string str=std::to_string(currentLine->lineNumber)+" GOTO";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        qstr = QString::fromStdString("\t"+std::to_string(currentLine->statement->getTargetLineNumber()));
        ui->treeBrowser->append(qstr);
        break;
    }
    case IF:
    {
        string str=std::to_string(currentLine->lineNumber)+" IF THEN";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);

        Expression *exp1=nullptr,*exp2=nullptr;
        currentLine->statement->get2Expression(exp1,exp2);
        createExpressionTree(exp1,1);

        qstr = QString::fromStdString("\t"+currentLine->statement->getVarName());
        ui->treeBrowser->append(qstr);

        createExpressionTree(exp2,1);
        qstr = QString::fromStdString("\t"+std::to_string(currentLine->statement->getTargetLineNumber()));
        ui->treeBrowser->append(qstr);
        break;
    }
    case END:
    {
        string str=std::to_string(currentLine->lineNumber)+" END";
        QString qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        break;
    }
    }
}
void MainWindow::createExpressionTree(Expression *exp,int indentation)
{
    string str;
    QString qstr;
    switch(exp->type())
    {
    case CONSTANT:
        for(int i=1;i<=indentation;++i)
        {
            str+="\t";
        }
        str += std::to_string(exp->getConstantValue());
        qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        break;
    case IDENTIFIER:
        for(int i=1;i<=indentation;++i)
        {
            str+="\t";
        }
        str += exp->getIdentifierName();
        qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        break;
    case COMPOUND:
        for(int i=1;i<=indentation;++i)
        {
            str+="\t";
        }
        str += exp->getOperator();
        qstr = QString::fromStdString(str);
        ui->treeBrowser->append(qstr);
        createExpressionTree(exp->getLHS(),indentation+1);
        createExpressionTree(exp->getRHS(),indentation+1);
        break;
    }
    return;
}
int MainWindow::getInputValue(bool &flag)
{
    QString text = ui->codeEdit->text();
    std::string input = text.toStdString();
    int *endPosition = new int(3);
    tokenizer->jumpBlank(input,endPosition);
    bool negative = false;
    if(input[*endPosition] == '-')
    {
        negative = true;
        ++(*endPosition);
        tokenizer->jumpBlank(input,endPosition);
    }
    int start = (*endPosition);
    int end = start;
    while('\0' != input[end])
    {
        if(' ' == input[end])
            break;
        if(!tokenizer->isDigit(input[end]))
        {
            flag = false;
            return 0;
        }
        end++;
    }
    if(end == start)
    {
        flag = false;
        return 0;
    }
    int number = 0;
    while(start < end)
    {
        number = number * 10 + input[start] - '0';
        start++;
    }
    ui->codeEdit->clear();
    flag = true;
    if(negative)
        number *= -1;
    return number;
}
string MainWindow::getInputStringValue()
{
    QString text = ui->codeEdit->text();
    std::string input = text.toStdString();
    int *endPosition = new int(3);
    tokenizer->jumpBlank(input,endPosition);
    if(input[*endPosition] != '\'' && input[*endPosition] != '"')
    {
        return "'";
    }
    string result = tokenizer->dealString(input,endPosition);
    ui->codeEdit->clear();
    return result;
}


//高亮显示代码
void MainWindow::highlightRed(int lineNumber, QList<QTextEdit::ExtraSelection> &extras)
{
    bool success = false;
    QTextBrowser *code = ui->codeBrowser;
    QTextCursor cursor(code->document());
    int currentLineNumber = 0;
    lineNode *currentLine = program->head->next;
    QList<QPair<int, QColor>> highlights =
    {
        {92, QColor(100, 255, 100)},
        {131, QColor(255, 100, 100)},
        {180, QColor(255, 100, 100)}
    };

    while(currentLine != program->tail && !success)
    {
        if(currentLine->lineNumber == lineNumber)
        {
            int position = code->document()->findBlockByLineNumber(currentLineNumber).position();
            cursor.setPosition(position, QTextCursor::MoveAnchor);
            QTextEdit::ExtraSelection h;
            h.cursor = cursor;
            h.cursor.movePosition(QTextCursor::StartOfLine);
            h.cursor.movePosition(QTextCursor::EndOfLine);
            h.format.setProperty(QTextFormat::FullWidthSelection, true);
            h.format.setBackground(highlights[1].second);
            extras.append(h);
            success = true;
        }
        ++currentLineNumber;
        currentLine = currentLine->next;
    }
    code->setExtraSelections(extras);
}
void MainWindow::highlightGreen(int lineNumber, QList<QTextEdit::ExtraSelection> &extras)
{
    extras.clear();
    bool success = false;
    QTextBrowser *code = ui->codeBrowser;
    QTextCursor cursor(code->document());
    int currentLineNumber = 0;
    lineNode *currentLine = program->head->next;
    QList<QPair<int, QColor>> highlights =
    {
        {92, QColor(100, 255, 100)},
        {131, QColor(255, 100, 100)},
        {180, QColor(255, 100, 100)}
    };

    while(currentLine != program->tail && !success)
    {
        if(currentLine->lineNumber == lineNumber)
        {
            int position = code->document()->findBlockByLineNumber(currentLineNumber).position();
            cursor.setPosition(position, QTextCursor::MoveAnchor);
            QTextEdit::ExtraSelection h;
            h.cursor = cursor;
            h.cursor.movePosition(QTextCursor::StartOfLine);
            h.cursor.movePosition(QTextCursor::EndOfLine);
            h.format.setProperty(QTextFormat::FullWidthSelection, true);
            h.format.setBackground(highlights[0].second);
            extras.append(h);
            success = true;
        }
        ++currentLineNumber;
        currentLine = currentLine->next;
    }
    code->setExtraSelections(extras);
}
void MainWindow::dealHELP()
{
    ui->treeBrowser->clear();
    QString qstr;
    qstr = QString::fromStdString("Standard Input format:");
    ui->treeBrowser->append(qstr);
    qstr = QString::fromStdString("Line Number + Blank + Statement or Only a Statement");
    ui->treeBrowser->append(qstr);
    qstr = QString::fromStdString("Statements With A Line Number:");
    ui->treeBrowser->append(qstr);
    qstr = QString::fromStdString("REM LET INPUT INPUTS PRINT PRINTF IF GOTO END");
    ui->treeBrowser->append(qstr);
    qstr = QString::fromStdString("Statements Without A Line Number:");
    ui->treeBrowser->append(qstr);
    qstr = QString::fromStdString("RUN LOAD CLEAR HELP QUIT");
    ui->treeBrowser->append(qstr);
    qstr = QString::fromStdString("But You Can Also Use LET INPUT INPUTS PRINT And PRINTF Without A Line Number");
    ui->treeBrowser->append(qstr);
    qstr = QString::fromStdString("Variable Naming Rule Is The Same As C++");
    ui->treeBrowser->append(qstr);
    return;
}

void MainWindow::on_debugButton_clicked()
{
    ui->loadButton->setDisabled(true);
    ui->clearButton->setDisabled(true);
    ui->debugButton->setDisabled(true);
    ui->stepButton->setEnabled(true);
    ui->codeEdit->setDisabled(true);
    ui->treeBrowser->clear();
    ui->resultBrowser->clear();
    ui->varBrowser->clear();
    extras1.clear();
    extras2.clear();
    debugModel = true;

    lineNode *currentLine = program->head->next;
    if(currentLine == program->tail)
    {
        QMessageBox msgBox;
        msgBox.setText("No codes");
        msgBox.exec();
        return;
    }
    //高亮显示错误语句
    while(currentLine != program->tail)
    {
        if(currentLine->errorNumber)
        {
            highlightRed(currentLine->lineNumber, extras1);
        }
        currentLine = currentLine->next;
    }

    currentLineNode = program->head->next;
    while(currentLineNode != program->tail && debugModel)
    {
        highlightGreen(currentLineNode->lineNumber, extras2);

        if(currentLineNode->lineCommand == END)
            break;
        loop2.exec();
    }
    if(debugModel)
    {
        debugModel = false;
        QMessageBox msgBox;
        string str = "The program ends successfully.";
        msgBox.setText(QString::fromStdString(str));
        msgBox.exec();
    }
    extras1.clear();
    extras2.clear();
    ui->codeBrowser->setExtraSelections(extras1);
    ui->codeBrowser->setExtraSelections(extras2);
    ui->loadButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
    ui->debugButton->setEnabled(true);
    ui->runButton->setEnabled(true);
    ui->stepButton->setDisabled(true);
    ui->codeEdit->setEnabled(true);
}

void MainWindow::on_stepButton_clicked()
{
    if(currentLineNode->errorNumber)
    {
        debugModel = false;
        QMessageBox msgBox;
        string str = "Line " + std::to_string(currentLineNode->lineNumber) + " Wrong Statement";
        msgBox.setText(QString::fromStdString(str));
        msgBox.exec();
        return;
    }
    else
    {
        ui->treeBrowser->clear();
        createSyntaxTree(currentLineNode);
        int nextLine = -1;
        switch(currentLineNode->lineCommand)
        {
        case REM:
        {
            break;
        }
        case LET:
        {
            Expression *exp = nullptr;
            currentLineNode->statement->get1Expression(exp);
            string varName = currentLineNode->statement->getVarName();
            if(exp != nullptr && exp->judge(*evalContext))
            {
                if(stringTable->isDefined(varName))
                {
                    string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                    debugModel = false;
                    QMessageBox msgBox;
                    msgBox.setText(QString::fromStdString(str));
                    msgBox.exec();
                    return;
                }
                else
                    evalContext->setValue(currentLineNode->statement->getVarName(), exp->eval(*evalContext));
            }
            else if(exp == nullptr && currentLineNode->statement->getStringValue() != "'")
            {
                if(evalContext->isDefined(varName))
                {
                    string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                    debugModel = false;
                    QMessageBox msgBox;
                    msgBox.setText(QString::fromStdString(str));
                    msgBox.exec();
                    return;
                }
                else
                    stringTable->setValue(varName, currentLineNode->statement->getStringValue());
            }
            else
            {
                string str = "Line " + std::to_string(currentLineNode->lineNumber) + " Wrong Expression";
                debugModel = false;
                QMessageBox msgBox;
                msgBox.setText(QString::fromStdString(str));
                msgBox.exec();
                return;
            }
            break;
        }
        case INPUT:
        {
            string varName = currentLineNode->statement->getVarName();
            if(stringTable->isDefined(varName))
            {
                string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                debugModel = false;
                QMessageBox msgBox;
                msgBox.setText(QString::fromStdString(str));
                msgBox.exec();
                return;
            }
            bool flag = false;
            int varValue;
            ui->stepButton->setDisabled(true);
            ui->runButton->setDisabled(true);
            ui->codeEdit->setEnabled(true);
            while(!flag)
            {
                ui->codeEdit->setText(" ? ");
                ui->codeEdit->setFocus();
                loop.exec();
                varValue = getInputValue(flag);
                ui->codeEdit->clear();
                if(flag)
                    break;
            }
            ui->stepButton->setEnabled(true);
            ui->runButton->setEnabled(true);
            ui->codeEdit->setDisabled(true);
            evalContext->setValue(varName, varValue);
            break;
        }
        case INPUTS:
        {
            string varName = currentLineNode->statement->getVarName();
            if(evalContext->isDefined(varName))
            {
                string str="Line "+std::to_string(currentLineNode->lineNumber)+" Duplicate Defined Variable";
                debugModel = false;
                QMessageBox msgBox;
                msgBox.setText(QString::fromStdString(str));
                msgBox.exec();
                return;
            }
            string stringValue;
            ui->stepButton->setDisabled(true);
            ui->runButton->setDisabled(true);
            ui->codeEdit->setEnabled(true);
            while(1)
            {
                ui->codeEdit->setText(" ? ");
                ui->codeEdit->setFocus();
                loop.exec();
                stringValue = getInputStringValue();
                ui->codeEdit->clear();
                if(stringValue[0] != '\'')
                    break;
            }
            ui->stepButton->setEnabled(true);
            ui->runButton->setEnabled(true);
            ui->codeEdit->setDisabled(true);
            stringTable->setValue(varName, stringValue);
            break;
        }
        case PRINT:
        {
            Expression *exp=nullptr;
            currentLineNode->statement->get1Expression(exp);
            if(exp != nullptr)
            {
                if(exp->judge(*evalContext))//输出数字
                {
                    int result = exp->eval(*evalContext);
                    QString qstr = QString::number(result);
                    ui->resultBrowser->append(qstr);
                    break;
                }
                else if(exp->type() == IDENTIFIER && stringTable->isDefined(exp->getIdentifierName()))//输出字符串
                {
                    string result = stringTable->getValue(exp->getIdentifierName());
                    QString qstr = QString::fromStdString(result);
                    ui->resultBrowser->append(qstr);
                    break;
                }
            }
            string str="Line "+std::to_string(currentLineNode->lineNumber)+" Wrong Expression";
            debugModel = false;
            QMessageBox msgBox;
            msgBox.setText(QString::fromStdString(str));
            msgBox.exec();
            return;
        }
        case PRINTF:
        {
            vector<string> *p1 = nullptr;
            vector<printfNode> *p2 = nullptr;
            currentLineNode->statement->get2Vector(p1, p2);
            if(p1 == nullptr)
            {
                ui->resultBrowser->append(QString::fromStdString(""));
                break;
            }
            if(p1->size() != 0 && p2->size() == 0)
            {
                ui->resultBrowser->append(QString::fromStdString((*p1)[0]));
                break;
            }
            int length2 = p2->size();
            int i = 0;
            string result;
            for(; i < length2; ++i)
            {
                result += (*p1)[i];
                if((*p2)[i].type == 1 || (*p2)[i].type == 0)
                    result += (*p2)[i].parameter;
                else if((*p2)[i].type == 2)
                {
                    string varName = (*p2)[i].parameter;
                    if(evalContext->isDefined(varName))
                        result += std::to_string(evalContext->getValue(varName));
                    else if(stringTable->isDefined(varName))
                        result += stringTable->getValue(varName);
                    else
                    {
                        debugModel = false;
                        QMessageBox msgBox;
                        string str = "Line " + std::to_string(currentLineNode->lineNumber) + "Undefined Variables!";
                        msgBox.setText(QString::fromStdString(str));
                        msgBox.exec();
                        return;
                    }
                }
            }
            result += (*p1)[i];
            ui->resultBrowser->append(QString::fromStdString(result));
            break;
        }
        case GOTO:
        {
            nextLine=currentLineNode->statement->getTargetLineNumber();
            break;
        }
        case IF:
        {
            Expression *exp1=nullptr,*exp2=nullptr;
            currentLineNode->statement->get2Expression(exp1,exp2);
            if(exp1 != nullptr && exp2 != nullptr && exp1->judge(*evalContext) && exp2->judge(*evalContext))
            {
                int result1,result2;
                string op;
                op=currentLineNode->statement->getVarName();
                result1=exp1->eval(*evalContext);
                result2=exp2->eval(*evalContext);
                if(op == "=")
                {
                    if(result1 == result2)
                    {
                        nextLine=currentLineNode->statement->getTargetLineNumber();
                    }
                }
                else if(op == "<")
                {
                    if(result1 < result2)
                    {
                        nextLine=currentLineNode->statement->getTargetLineNumber();
                    }
                }
                else if(op == ">")
                {
                    if(result1 > result2)
                    {
                        nextLine=currentLineNode->statement->getTargetLineNumber();
                    }
                }
                else
                {
                    string str="Line "+std::to_string(currentLineNode->lineNumber)+" Wrong Symbol";
                    debugModel = false;
                    QMessageBox msgBox;
                    msgBox.setText(QString::fromStdString(str));
                    msgBox.exec();
                    return;
                }
            }
            else
            {
                string str="Line "+std::to_string(currentLineNode->lineNumber)+" Wrong Expression";
                debugModel = false;
                QMessageBox msgBox;
                msgBox.setText(QString::fromStdString(str));
                msgBox.exec();
                return;
            }
            break;
        }
        case END:
        {
            return;
        }
        }
        renewVarBrowser();
        if(nextLine == -1)
            currentLineNode = currentLineNode->next;
        else
        {
            bool flag=false;
            currentLineNode = program->head->next;
            while(currentLineNode != program->tail)
            {
                if(currentLineNode->lineNumber == nextLine)
                {
                    flag=true;
                    break;
                }
                currentLineNode = currentLineNode->next;
            }
            if(!flag)
            {
                debugModel = false;
                QMessageBox msgBox;
                string str = "Line " + std::to_string(currentLineNode->lineNumber) + " Wrong Target Number";
                msgBox.setText(QString::fromStdString(str));
                msgBox.exec();
                return;
            }
        }
    }
    return;
}
