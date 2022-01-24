#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QFileDialog>
#include<QTimer>
#include<QElapsedTimer>
#include<QEventLoop>
#include<QFile>
#include<QDebug>
#include<QMessageBox>
#include<QTextEdit>
#include<QTextBlock>
#include"tokenizer.h"
#include"program.h"
//using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;


public:
    QList<QString> resultOutput;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Tokenizer *tokenizer;
    Program *program;
    EvaluationContext *evalContext;
    StringTable *stringTable;
    QFileDialog *fileDialog;
    QTextStream *readStream;
    QFile *fileText;
    //QApplication* app;
    QEventLoop loop;
    QEventLoop loop2;
    QList<QTextEdit::ExtraSelection> extras1;
    QList<QTextEdit::ExtraSelection> extras2;
    bool debugModel = false;
    lineNode *currentLineNode = nullptr;
    void error(const std::string errorReport);//输出错误信息
    void dealNewLine(const QString text);
    void renewCodeBrowser();//更新代码展示框
    void renewVarBrowser();//更新当前变量展示框
    int getInputValue(bool &flag);
    string getInputStringValue();
    void test(const QString text);
    void createSyntaxTree(lineNode *currentLine);
    void createExpressionTree(Expression *exp,int indentation);
    //处理各种指令
    void dealREMStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealLETStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealINPUTStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealINPUTSStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealPRINTStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealPRINTFStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealGOTOStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealIFStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    bool dealENDStatement(string &inputCode,lineNode *newLineNode,int* endPos);
    //处理立即执行的五条语句
    void dealImmediateLET(string &inputCode,int* endPos);
    void dealImmediateINPUT(string &inputCode,int* endPos);
    void dealImmediateINPUTS(string &inputCode,int* endPos);
    void dealImmediatePRINT(string &inputCode,int* endPos);
    void dealImmediatePRINTF(string &inputCode,int* endPos);
    void highlightRed(int lineNumber,QList<QTextEdit::ExtraSelection> &extras);
    void highlightGreen(int lineNumber,QList<QTextEdit::ExtraSelection> &extras);
    void dealHELP();







private slots:
    void on_codeEdit_returnPressed();

    void on_loadButton_clicked();
    void on_clearButton_clicked();
    void on_runButton_clicked();
    void on_debugButton_clicked();
    void on_stepButton_clicked();
};
#endif // MAINWINDOW_H
