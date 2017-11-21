#ifndef SENDIP_H
#define SENDIP_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>
#include <QtCharts>
#include "networkinterface.h"


class SendIP : public QWidget
{
    Q_OBJECT
public:
    SendIP(QVBoxLayout *layout_base, std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay, NetworkInterface* myNetworkInterface, bool dock);
    //SendIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay,QMainWindow * mymainwindow, bool dock);
    ~SendIP();

    QLineEdit* myLinedit;
    //QPushButton* myPushButtonSend;
    void openWidget();
    std::string number;



private slots:
    void sendData();


private:
    //NetworkInterface* networkInterface;
    //QHBoxLayout *layout_horizontal;
    //QVBoxLayout *layout_vertical;
    QMainWindow *window;
    std::string name,comment,supportInReplay,area,list,service,field = "";


    NetworkInterface* networkInterface;
};

#endif // SENDIP_H
