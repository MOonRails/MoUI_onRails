#ifndef SERVICE_DISPLAY_H
#define SERVICE_DISPLAY_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QtCharts>

#include "publiship.h"
#include "sendip.h"
#include "networkinterface.h"

class Service_Display : public QWidget
{
    Q_OBJECT
public:
    Service_Display();
    Service_Display(QDockWidget *myDockWidget, QObject *myWindow, NetworkInterface* myNetworkInterface);
    ~Service_Display();

    void addDataSend(std::string name, std::string comment, std::string number, std::string supportInReplay, std::string field, std::string type, std::vector<std::string> *myTypeList, std::vector<std::string> *myNamesList);
    void addDataReception(std::string name, std::string comment, std::string number, std::string supportInReplay);
    int findPublishList(std::string myNumber);
    void addSpacer();
private:
    //QObject *mainWindow; // contains the QObject from the main window
    QVBoxLayout *layout_vertical_Send;
    QVBoxLayout *layout_vertical_Reception;

    NetworkInterface* networkInterface;

    //std::vector<QPushButton*> mySendButtons;
    //std::vector<QLabel*> myReceptionLabel;
    //std::vector<QChart*> myReceptionChart;
    std::vector<SendIP*> mySendIP_list;
    std::vector<PublishIP*> myPublishIP_list;

    QLineEdit * myLineEdit;
    QPushButton * pushButton_send;
    bool dataReceptionFirst = true;

    QDockWidget *dockWidget;
    //QObject *window;
private slots:
    void on_pushButton_send_clicked();

};
#endif // SERVICE_DISPLAY_H
