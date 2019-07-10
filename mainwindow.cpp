#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QPainter>
#include <QFile>

#define W  640
#define H  480

#define LEN  (W*H*3 + 54)
#define P_LEN  1380

#define QStringLiteral(str) QString::fromUtf8(str, sizeof(str) - 1)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status = 0;
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect_slot()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()));
    data = new char[LEN];
    save_picture_flag = 0;

    setWindowTitle(QStringLiteral("SYSU视频监控"));
    ui->passoword->setEchoMode(QLineEdit::Password);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete [] data;
    delete socket;
}


void MainWindow::on_save_btn_clicked() {
    qDebug("SaveButton clicked.");
    save_pic_name =  ui->savepath->toPlainText();
    save_picture_flag = 1;
    QString s("save:%1");
    socket->write(s.arg(save_pic_name).toUtf8());
}

void MainWindow::disconnect_slot() {
    ui->connect_btn->setText("Connect");
}

void MainWindow::read_data() {
    int ret;

    ret = socket->read(data+len_rcv, P_LEN);
    if (0 == strncmp("Error:", data+len_rcv, 6)) {
        QMessageBox::warning(this, QStringLiteral("错误"), data+len_rcv);
        status = 0;
        socket->abort();
        ui->connect_btn->setText("connect");
    } else if (0 == strncmp("newImage", data+len_rcv, 8)) {
        len = atoi(data+len_rcv+9);
    } else {
        len_rcv += ret;
        if (len_rcv >= len) {
            update();
            return;
        }
    }
    socket->write("ack");
}

void MainWindow::update() {
    QPixmap pix;
    pix.loadFromData((uchar *)data, len);
    ui->monitor->setPixmap(pix);

    if(save_picture_flag == 1) {
        save_picture_flag = 0;
        QFile rgbfile(save_pic_name + ".bmp");

        if(!rgbfile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QMessageBox msgbox(QMessageBox::Warning, "WARNING", "please enter file name");
            msgbox.exec();
            return;
        }

        rgbfile.write((const char *)data, LEN);
        qDebug() << " save picture finish";
    }

    if (status == 1) {
        socket->write("new_request");
    }

    len_rcv = 0;
}

void MainWindow::on_connect_btn_clicked() {
    qDebug("Connect Button clicked.");
    QString _start = "Connect";
    QString _stop = "Stop";

    if(status == 0) {
        ui->connect_btn->setText(_stop);
        QString host = ui->hostip->text();
        QString username = ui->username->text();
        QString password = ui->passoword->text();

        socket->connectToHost(host, 8080);
        if (!socket->waitForConnected(1000)) {
            ui->connect_btn->setText(_start);
            QMessageBox msgbox(QMessageBox::Critical, "Error", "server connection failed");
            msgbox.exec();
            return;
        }

        len_rcv = 0;
        status = 1;
        QString s("%1&%2");
        socket->write(s.arg(username, password).toUtf8());
    } else if (status == 1) {
        ui->connect_btn->setText("Play");
        socket->write("stop");
        status = 2;
    } else if (status == 2) {
        ui->connect_btn->setText("Stop");
        socket->write("play");
        status = 1;
    }
}
