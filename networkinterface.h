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
#include "publiship.h"


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
    void addPublishIP(PublishIP* myPublishIP);
private:

    QPushButton* pushButton_connect;
    QLineEdit* lineEdit_ip;
    QLineEdit* lineEdit_port;


    std::vector<PublishIP*> myPublishIP_list;

    //QTcpServer *myServer;
    QTcpSocket *clientConnection;
    QNetworkSession *networkSession;
    QDataStream in;

    bool connectButtonStatus = false;

    QMainWindow* mainwindow;
    Ui::MainWindow * ui;

private slots:
    void on_pushButton_connect_clicked();
    void newConnection();
    void disconnecting();
    void disconnected();
    void inComingFromServer();
    void connection();
    void readIncoming();

};

#endif // NETWORKINTERFACE_H
