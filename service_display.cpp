#include "service_display.h"


//! constructor
// ###################################################################
Service_Display::Service_Display()
{

}



//! constructor
// ###################################################################
Service_Display::Service_Display(QDockWidget *dockWidget, QObject *myWindow)
{
    //window = myWindow;
    //Ui::MainWindow * myUi;
    //myUi->statusBar->showMessage(tr("Ready2"));
    //statusBar()->showMessage(tr("Ready2"));
    QWidget* multiWidget = new QWidget();
    QHBoxLayout *layout_horizontal = new QHBoxLayout;
    QVBoxLayout *layout_vertical = new QVBoxLayout;
    layout_vertical_Send = new QVBoxLayout;
    layout_vertical_Reception = new QVBoxLayout;

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    QHBoxLayout *layout_horizontal_genericSend = new QHBoxLayout;

    myLineEdit = new QLineEdit;
    myLineEdit->setMaximumWidth(100);
    pushButton_send = new QPushButton("Send");
    pushButton_send->setMaximumWidth(100);

    layout_horizontal_genericSend->addWidget(myLineEdit);
    layout_horizontal_genericSend->addWidget(pushButton_send);


    layout_horizontal->addLayout(layout_vertical_Reception);
    layout_horizontal->addWidget(line);
    layout_horizontal->addLayout(layout_vertical_Send);

    layout_vertical->addLayout(layout_horizontal_genericSend);
    layout_vertical->addLayout(layout_horizontal);
    multiWidget->setLayout(layout_vertical);

    dockWidget->setWidget(multiWidget);
}


//! destructor
// ###################################################################
Service_Display::~Service_Display()
{
}



//! add a data sending window
// ###################################################################
//void ServiceDisplay::addDataSend(std::string name, std::string comment, std::string number, std::string supportInReplay,QMainWindow * mymainwindow)
void Service_Display::addDataSend(std::string name, std::string comment, std::string number, std::string supportInReplay)
{
    SendIP* sendip = new SendIP(layout_vertical_Send, name, comment, number, supportInReplay, false);
    //sendip->openWidget();
    //sendipList.push_back(sendip);

    //SendIP* sendip2 = new SendIP(myNetworkInterface, layout_vertical_Send, name, comment, number, supportInReplay,mymainwindow, true);

}



//! add a data display window
// ###################################################################
void Service_Display::addDataReception(std::string name, std::string comment, std::string number, std::string supportInReplay)
{
    //qDebug()<< "addDataReception\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";

    //networkInterface = myNetworkInterface;
    PublishIP* myPublish = new PublishIP(layout_vertical_Reception, name, comment, number, supportInReplay);

    //publishipList.push_back(myPublish);

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
void Service_Display::findPublishList(std::string myNumber)
{
    //for (int i = 0; i < publishipList.size(); ++i) {
    //    qDebug() <<  publishipList[i]->number.c_str();//"publish number " <<
    //}
}
