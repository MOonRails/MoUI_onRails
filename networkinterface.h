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
#include <QByteArray>


#include <iostream>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QElapsedTimer>
#include <QThread>
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
    void pusblishIp_String(std::string myNumber,std::string myText);

    int connectionStatus = 0;
    // 0 = No connection
    // 1 = connected to IP
    // 2 = connected to Serial Port


    QSerialPort *serialPort;
    void addPublishIP(PublishIP* myPublishIP);
private:

    QList<QString> serialPortsList;

    QPushButton* pushButton_connectIP;
    QPushButton* pushButton_connectSerial;
    QComboBox* combobox_serial;
    QLineEdit* lineEdit_ip;
    QLineEdit* lineEdit_port;
    QByteArray buffer;
    QString buffer2;

    std::vector<PublishIP*> myPublishIP_list;

    //QTcpServer *myServer;
    QTcpSocket *clientConnection;
    QNetworkSession *networkSession;
    QDataStream in;

    bool receptionExclusion = false;


    bool connectButtonStatus = false; // replaced by connectionStatus

    QMainWindow* mainwindow;
    Ui::MainWindow * ui;

    int leftoverIncomingData_column = -1;
    std::string leftoverIncomingData = "";

    QString lastCommandSend = "";



private slots:


    void on_pushButton_connectIP_clicked();
    void on_pushButton_connectSerial_clicked();
    void newConnection();
    void disconnecting();
    void disconnected();
    void inComingFromServer();
    void connection();
    void readIncoming();

    void readIncomingFromSerial();


    void displayConnectionError(QAbstractSocket::SocketError socketError);
    void sessionOpened();
    //void myServerNewConnection();
    void listeningToIncomingData();


    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
};

#endif // NETWORKINTERFACE_H
