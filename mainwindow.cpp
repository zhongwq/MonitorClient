#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTcpSocket>
#include <QSqlError>

#define QStringLiteral(str) QString::fromUtf8(str, sizeof(str) - 1)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDesktopWidget *desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    setWindowTitle(QStringLiteral("Login"));
    ui->lineEditPass->setEchoMode(QLineEdit::Password);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoginButton_clicked()
{
    QString username = ui->lineEditNum->text();
    QString password = ui->lineEditPass->text();
    if(username=="" || password=="") {
        QMessageBox::information(this, QStringLiteral("警告！"),QStringLiteral("请按照格式正确填写用户名，密码！！"));
    }




}

void MainWindow::on_ExitButton_clicked() {
    this->close();
}
