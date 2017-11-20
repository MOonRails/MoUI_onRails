#include "sendip.h"
#include "networkinterface.h"
#include "mainwindow.h"

//! constructor
// ###################################################################
SendIP::SendIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay, QMainWindow * mymainwindow, bool dock)
{
    window = mymainwindow;
    qDebug() << "SendIP number " << myNumber.c_str() << " supportInReplay " << mySupportInReplay.c_str();
    networkInterface = myNetworkInterface;
    name = myName;
    comment = myComment;
    number = myNumber;
    supportInReplay = mySupportInReplay;
    QLabel* myTitle = new QLabel(name.c_str());
    myLinedit = new QLineEdit();
    QPushButton* myPushButtonSend = new QPushButton("Send");
    QObject::connect(myPushButtonSend, SIGNAL(clicked()), this , SLOT(sendData())  );




    if(!dock){
        QHBoxLayout *layout_horizontal = new QHBoxLayout;
        QVBoxLayout *layout_vertical = new QVBoxLayout;
        layout_vertical->addWidget(myTitle);
        layout_horizontal->addWidget(myLinedit);
        layout_horizontal->addWidget(myPushButtonSend);
        layout_vertical->addLayout(layout_horizontal);

        layout_base->addLayout(layout_vertical);
    } else {
        QDockWidget * dockWidget = new QDockWidget(name.c_str(), window);


        QHBoxLayout *layout_horizontal2 = new QHBoxLayout;
        QVBoxLayout *layout_vertical2 = new QVBoxLayout;

        //layout_vertical->addWidget(myTitle);
        layout_horizontal2->addWidget(myLinedit);
        layout_horizontal2->addWidget(myPushButtonSend);
        layout_vertical2->addLayout(layout_horizontal2);
        dockWidget->setLayout(layout_horizontal2);
        //dockWidget->setWidget(myLinedit);
        //dockWidget->setWidget(myPushButtonSend);

        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
        dockWidget->setFloating(true);
        window->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    }




}


//! destructor
// ###################################################################
SendIP::~SendIP()
{

}



//! send data button pushed
// ###################################################################
void SendIP::sendData()
{
    ///qDebug()<< "sendData\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
    ///
    std::string mySendString = number + std::string(":") + myLinedit->text().toStdString();
    //qDebug() << "sendipSend " << mySendString.c_str() << " number " << number.c_str();
    networkInterface->sendString(mySendString);

}


//! open the items as docking widget
// ###################################################################
void SendIP::openWidget(){
    /*
    QDockWidget * dockWidget = new QDockWidget(name.c_str(), window);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
    dockWidget->setFloating(true);
    //QWidget *myWidget;

    window->addDockWidget(Qt::RightDockWidgetArea, dockWidget);


    //QWidget* multiWidget = new QWidget();
    //QVBoxLayout *dockVerticalLayout = new QVBoxLayout;
    //QLabel *title = new QLabel("MoOnRails");
    //dockVerticalLayout->addWidget(title);
    //multiWidget->setLayout(dockVerticalLayout);
    //dockWidget->setWidget(multiWidget);
    //dockWidget->setLayout(layout_vertical);


    QLabel* myTitle = new QLabel(name.c_str());
    QLineEdit* myLinedit = new QLineEdit();
    QPushButton* myPushButtonSend = new QPushButton("Send");
    QObject::connect(myPushButtonSend, SIGNAL(clicked()), this , SLOT(sendData())  );
    QHBoxLayout *layout_horizontal = new QHBoxLayout;
    QVBoxLayout *layout_vertical = new QVBoxLayout;

    layout_vertical->addWidget(myTitle);
    layout_horizontal->addWidget(myLinedit);
    layout_horizontal->addWidget(myPushButtonSend);
    layout_vertical->addLayout(layout_horizontal);

    dockWidget->setLayout(layout_vertical);*/
}

