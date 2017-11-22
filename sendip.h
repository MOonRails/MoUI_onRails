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
    SendIP(QVBoxLayout *layout_base, std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay, NetworkInterface* myNetworkInterface, std::string myField, std::string myType, std::vector<std::string> *myTypeList, std::vector<std::string> *myNamesList, bool dock);
    //SendIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay,QMainWindow * mymainwindow, bool dock);
    ~SendIP();

    QLineEdit* myLinedit;
    //QPushButton* myPushButtonSend;
    void openWidget();
    std::string number;



private slots:
    void sendData();
    void sendDataBool();
    void sendDataStructure();
    void on_myPushButtonFalse_clicked();
    void on_myPushButtonTrue_clicked();


private:
    bool buttonTrue = true;
    bool buttonFalse = false;

    void defaultButtons();

    //NetworkInterface* networkInterface;
    //QHBoxLayout *layout_horizontal;
    //QVBoxLayout *layout_vertical;
    QMainWindow *window;
    std::string name,comment,supportInReplay,area,list,service,field,type = "";

    QPushButton* myPushButtonTrue;
    QPushButton* myPushButtonFalse;


    NetworkInterface* networkInterface;
    QLabel* myTitle;
    QHBoxLayout *layout_horizontal;
    QVBoxLayout *layout_vertical;
    QPushButton* myPushButtonSend;
    std::vector<QLineEdit*>myNewQlineEdit_list;
};

#endif // SENDIP_H
