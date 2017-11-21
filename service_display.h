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

class Service_Display : public QWidget
{
    Q_OBJECT
public:
    Service_Display();
    Service_Display(QDockWidget *dockWidget, QObject *myWindow);
    ~Service_Display();

    void addDataSend(std::string name, std::string comment, std::string number, std::string supportInReplay);
    void addDataReception(std::string name, std::string comment, std::string number, std::string supportInReplay);
    void findPublishList(std::string myNumber);

private:
    //QObject *mainWindow; // contains the QObject from the main window
    QVBoxLayout *layout_vertical_Send;
    QVBoxLayout *layout_vertical_Reception;



    //std::vector<QPushButton*> mySendButtons;
    //std::vector<QLabel*> myReceptionLabel;
    //std::vector<QChart*> myReceptionChart;

    QLineEdit * myLineEdit;
    QPushButton * pushButton_send;


    //QObject *window;

};
#endif // SERVICE_DISPLAY_H
