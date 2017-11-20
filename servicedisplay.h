#ifndef SERVICEDISPLAY_H
#define SERVICEDISPLAY_H


#include "publiship.h"
#include "sendip.h"
#include "servicedisplay.h"

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>
#include <QtCharts>

class ServiceDisplay : public QWidget
{
    Q_OBJECT
public:
    ServiceDisplay();
    ServiceDisplay(QDockWidget *dockWidget, QObject *myWindow);
    ~ServiceDisplay();
    void addDataSend(NetworkInterface* myNetworkInterface,std::string name, std::string comment, std::string number, std::string supportInReplay,QMainWindow * mymainwindow);
    void addDataReception(NetworkInterface* myNetworkInterface,std::string name, std::string comment, std::string number, std::string supportInReplay);
    void setNetworkInterface();
    void findPublishList(std::string myNumber);

private:
    QObject *mainWindow; // contains the QObject from the main window
    QVBoxLayout *layout_vertical_Send;
    QVBoxLayout *layout_vertical_Reception;

    std::vector<QPushButton*> mySendButtons;

    std::vector<QLabel*> myReceptionLabel;

    std::vector<QChart*> myReceptionChart;


    //std::vector<SendIP*> sendipList;
    std::vector<PublishIP*> publishipList;

    NetworkInterface* networkInterface;

    QObject *window;

};

#endif // SERVICEDISPLAY_H
