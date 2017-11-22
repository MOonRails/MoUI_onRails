#include "service_display.h"


//! constructor
// ###################################################################
Service_Display::Service_Display()
{

}



//! constructor
// ###################################################################
Service_Display::Service_Display(QDockWidget *dockWidget, QObject *myWindow, NetworkInterface *myNetworkInterface)
{
    networkInterface=myNetworkInterface;
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



    QLabel* myTitle = new QLabel(" Generic Send:");
    myLineEdit = new QLineEdit;
    myLineEdit->setMaximumWidth(100);
    pushButton_send = new QPushButton("Send");
    pushButton_send->setMaximumWidth(100);
    QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding,QSizePolicy::Minimum);

    QObject::connect(pushButton_send, SIGNAL(clicked()), this , SLOT(on_pushButton_send_clicked())  );

    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    //layout_horizontal_genericSend->addWidget(myLineEdit);
    //layout_horizontal_genericSend->addWidget(pushButton_send);
    //layout_horizontal_genericSend->addSpacerItem(spacer);


    QHBoxLayout *layout_horizontal2 = new QHBoxLayout;
    QVBoxLayout *layout_vertical2 = new QVBoxLayout;
    layout_vertical_Send->addWidget(myTitle);
    layout_vertical_Send->addWidget(myLineEdit);
    layout_vertical_Send->addWidget(pushButton_send);
    //layout_horizontal2->addWidget(myLineEdit);
    //layout_horizontal2->addWidget(pushButton_send);
    //layout_vertical_Send->addLayout(layout_horizontal);
    //layout_vertical_Send->addLayout(layout_vertical2);

    layout_horizontal->addLayout(layout_vertical_Reception);
    layout_horizontal->addWidget(line);
    layout_horizontal->addLayout(layout_vertical_Send);

    //layout_vertical->addLayout(layout_horizontal_genericSend);
    //layout_vertical->addWidget(line2);
    //layout_vertical->addLayout(layout_horizontal);





    multiWidget->setLayout(layout_horizontal);

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
void Service_Display::addDataSend(std::string name, std::string comment, std::string number, std::string supportInReplay, std::string field, std::string type, std::vector<std::string> *myTypeList, std::vector<std::string> *myNamesList)
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("QLineEdit {background-color: rgb(0, 0, 0);}");
    layout_vertical_Send->addWidget(line);
    SendIP* sendip = new SendIP(layout_vertical_Send, name, comment, number, supportInReplay,networkInterface,field,type,myTypeList,myNamesList, false);
    //sendip->openWidget();
    mySendIP_list.push_back(sendip);

    //SendIP* sendip2 = new SendIP(myNetworkInterface, layout_vertical_Send, name, comment, number, supportInReplay,mymainwindow, true);

}



//! add a data display window
// ###################################################################
void Service_Display::addDataReception(std::string name, std::string comment, std::string number, std::string supportInReplay)
{
    //qDebug()<< "addDataReception\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";

    //networkInterface = myNetworkInterface;
    if(dataReceptionFirst == false){
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        //QPalette palette = new QPalette();
        //palette.setColor(QPalette::Base, Qt::green);
        layout_vertical_Reception->addWidget(line);
    } else {
        dataReceptionFirst = false;
    }


    PublishIP* myPublish = new PublishIP(layout_vertical_Reception, name, comment, number, supportInReplay);

    myPublishIP_list.push_back(myPublish);
    networkInterface->addPublishIP(myPublish);

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
int Service_Display::findPublishList(std::string myNumber)
{
    for (int i = 0; i < myPublishIP_list.size(); ++i) {
        if(myPublishIP_list[i]->number == myNumber){
            return i;
        }
    }
    return -1;
}


// ###################################################################
void Service_Display::addSpacer()
{
    QSpacerItem *spacerL = new QSpacerItem(20, 0, QSizePolicy::Minimum,QSizePolicy::Expanding);
    QSpacerItem *spacerR = new QSpacerItem(20, 0, QSizePolicy::Minimum,QSizePolicy::Expanding);
    layout_vertical_Reception->addSpacerItem(spacerL);
    layout_vertical_Send->addSpacerItem(spacerR);
}


//!
// ###################################################################
void Service_Display::on_pushButton_send_clicked()
{
    std::string mySendString = myLineEdit->text().toStdString();
    //qDebug() << "sendipSend " << mySendString.c_str() << " number " << number.c_str();
    networkInterface->sendString(mySendString);
}
