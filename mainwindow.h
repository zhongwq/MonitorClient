#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_connect_btn_clicked();
    void on_save_btn_clicked();
    void disconnect_slot();
    void read_data();
    void update();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    int status;
    QString save_pic_name;
    bool save_picture_flag;
    char *data;
    int  len; //图像大小
    int  len_rcv; //接收大小
};

#endif // MAINWINDOW_H
