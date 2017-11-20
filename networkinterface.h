#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H
//#include <servicedisplay.h>

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>
#include <QtCharts>

#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include "ui_mainwindow.h"

class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;




class NetworkInterface : public QWidget
{
    Q_OBJECT
public:
    NetworkInterface(QVBoxLayout *layout_base, QMainWindow * mymainwindow, Ui::MainWindow * myUi);
    ~NetworkInterface();

    void sendString(std::string myString);
    void findPusblishIp(std::string myNumber);
    //std::vector<ServiceDisplay*> myServices;

private:

    QPushButton* pushButton_connect;
    QLineEdit* lineEdit_ip;
    QLineEdit* lineEdit_port;



    QTcpSocket *mySocket;
    QTcpServer *myServer;
    QTcpSocket *clientConnection;
    QNetworkSession *networkSession;

    bool connectButtonStatus = false;

    QMainWindow* mainwindow;
    Ui::MainWindow * ui;

private slots:
    void on_pushButton_connect_clicked();
    void newConnection();
    void disconnecting();//
    void disconnected();//
    void inComingFromServer();//
    void connection();//

};

#endif // NETWORKINTERFACE_H
