#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QPainter>

#define W  640
#define H  480

#define LEN  (W*H*3 + 54)
#define P_LEN 1380

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect_slot()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()));
    data = new char[LEN];
}

MainWindow::~MainWindow()
{
    delete ui;
    delete [] data;
    delete socket;
}

void MainWindow::disconnect_slot() {
    ui->connect_btn->setText("Connect");
}

void MainWindow::read_data() {
    int ret;

    ret = socket->read(data+len_rcv, P_LEN);

    if (0 == strncmp("newImage", data+len_rcv, 8)) {
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

    qDebug() << "load pix";

    ui->monitor->setPixmap(pix);

    qDebug() << "setPixmap";

    socket->write("new_request");
    len_rcv = 0;
}

void MainWindow::on_connect_btn_clicked() {
    qDebug("Connect Button clicked.");

    static unsigned char count = 0;
    QString _start = "Connect";
    QString _stop = "Stop";

    if(count == 0)
    {
        ui->connect_btn->setText(_stop);
        QString host = ui->hostip->text();
        socket->connectToHost(host, 8080);
        if (!socket->waitForConnected(1000)) {
            ui->connect_btn->setText(_start);
            QMessageBox::critical(this, "error", "server connection failed");
            return;
        }

        len_rcv = 0;
        socket->write("new_request");
    } else {
        ui->connect_btn->setText(_start);
        count = 0;
    }
}
