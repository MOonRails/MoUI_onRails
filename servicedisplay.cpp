#include "servicedisplay.h"
#include "publiship.h"
#include "sendip.h"


#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QtCharts>


//! constructor
// ###################################################################
ServiceDisplay::ServiceDisplay()
{

}


//! constructor
// ###################################################################
ServiceDisplay::ServiceDisplay(QDockWidget *dockWidget, QObject *myWindow)
{
    window = myWindow;
    //Ui::MainWindow * myUi;
    //myUi->statusBar->showMessage(tr("Ready2"));
    //statusBar()->showMessage(tr("Ready2"));
    QWidget* multiWidget = new QWidget();
    QHBoxLayout *layout_horizontal = new QHBoxLayout;
    layout_vertical_Send = new QVBoxLayout;
    layout_vertical_Reception = new QVBoxLayout;

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);


    layout_horizontal->addLayout(layout_vertical_Reception);
    layout_horizontal->addWidget(line);
    layout_horizontal->addLayout(layout_vertical_Send);
    multiWidget->setLayout(layout_horizontal);

    dockWidget->setWidget(multiWidget);
}


//! destructor
// ###################################################################
ServiceDisplay::~ServiceDisplay()
{
}


//! add a data sending window
// ###################################################################
void ServiceDisplay::addDataSend(NetworkInterface* myNetworkInterface,std::string name, std::string comment, std::string number, std::string supportInReplay,QMainWindow * mymainwindow)
{
    SendIP* sendip = new SendIP(myNetworkInterface, layout_vertical_Send, name, comment, number, supportInReplay,mymainwindow, false);
    //sendip->openWidget();
    //sendipList.push_back(sendip);

    //SendIP* sendip2 = new SendIP(myNetworkInterface, layout_vertical_Send, name, comment, number, supportInReplay,mymainwindow, true);

}

//! add a data display window
// ###################################################################
void ServiceDisplay::addDataReception(NetworkInterface* myNetworkInterface, std::string name, std::string comment, std::string number, std::string supportInReplay)
{
    //qDebug()<< "addDataReception\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";

    networkInterface = myNetworkInterface;
    PublishIP* myPublish = new PublishIP(myNetworkInterface, layout_vertical_Reception, name, comment, number, supportInReplay);

    publishipList.push_back(myPublish);

    myPublish->setValue(1);
    myPublish->setValue(2);
    myPublish->setValue(11);
    myPublish->setValue(3);
    myPublish->setValue(4);
    myPublish->setValue(15);
    myPublish->setValue(7);
    myPublish->setValue(8);
}


//!
// ###################################################################
void ServiceDisplay::findPublishList(std::string myNumber)
{
    for (int i = 0; i < publishipList.size(); ++i) {
        qDebug() <<  publishipList[i]->number.c_str();//"publish number " <<
    }
}
