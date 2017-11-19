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
void ServiceDisplay::addDataSend(std::string title)
{
    SendIP* sendip = new SendIP(layout_vertical_Send, title);

}

//! add a data display window
// ###################################################################
void ServiceDisplay::addDataReception(std::string title)
{
    //qDebug()<< "addDataReception\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";


    PublishIP* myPublish = new PublishIP(layout_vertical_Reception, title);


    myPublish->setValue(1);
    myPublish->setValue(2);
    myPublish->setValue(11);
    myPublish->setValue(3);
    myPublish->setValue(4);
    myPublish->setValue(15);
    myPublish->setValue(7);
    myPublish->setValue(8);
}

