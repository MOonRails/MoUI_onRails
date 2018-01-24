#include "sendip.h"

#include "mainwindow.h"

//! constructor
// ###################################################################
//SendIP::SendIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay, QMainWindow * mymainwindow, bool dock)
SendIP::SendIP( QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber,
                std::string mySupportInReplay,
                NetworkInterface* myNetworkInterface,
                std::string myField,
                std::string myType,
                std::vector<std::string>*myTypeList,
                std::vector<std::string>*myNamesList, bool dock)
{
    //window = mymainwindow;
    qDebug() << "SendIP number " << myNumber.c_str() << " myType " << myType.c_str();
    //networkInterface = myNetworkInterface;
    name = myName;
    comment = myComment;
    number = myNumber;
    supportInReplay = mySupportInReplay;
    field = myField;
    type = myType;

    myName = number + std::string(". ") + myName + std::string(": ");

    myTitle = new QLabel(myName.c_str());





    networkInterface=myNetworkInterface;
    myPushButtonSend = new QPushButton("Send");
    myPushButtonSend->setToolTip(myComment.c_str());


    layout_horizontal = new QHBoxLayout;
    layout_vertical = new QVBoxLayout;

    if(type == "Boolean"){
        myPushButtonTrue = new QPushButton("TRUE");
        myPushButtonTrue->setDown(true);
        myPushButtonFalse = new QPushButton("FALSE");
        QObject::connect(myPushButtonTrue, SIGNAL(clicked()), this , SLOT(on_myPushButtonTrue_clicked())  );
        QObject::connect(myPushButtonFalse, SIGNAL(clicked()), this , SLOT(on_myPushButtonFalse_clicked())  );
        QObject::connect(myPushButtonSend, SIGNAL(clicked()), this , SLOT(sendDataBool())  );

        layout_vertical->addWidget(myTitle);
        layout_horizontal->addWidget(myPushButtonTrue);
        layout_horizontal->addWidget(myPushButtonFalse);
        layout_horizontal->addWidget(myPushButtonSend);
        layout_vertical->addLayout(layout_horizontal);
    } else if(type == "UOctet") {
        // add horizontal slider here
        defaultButtons();
    } else if(myTypeList->size() != 0) {
        layout_vertical->addWidget(myTitle);
        for (int i = 0; i < myTypeList->size(); ++i) {
            QLineEdit* myNewQlineEdit = new QLineEdit();
            std::string tooltip;
            tooltip += myNamesList->operator[](i);
            tooltip += " : ";
            tooltip += myTypeList->operator[](i);
            myNewQlineEdit->setToolTip(tooltip.c_str());
            myNewQlineEdit_list.push_back(myNewQlineEdit);
            layout_horizontal->addWidget(myNewQlineEdit);
        }
        layout_horizontal->addWidget(myPushButtonSend);
        QObject::connect(myPushButtonSend, SIGNAL(clicked()), this , SLOT(sendDataStructure())  );
        layout_vertical->addLayout(layout_horizontal);
    }  else {
        defaultButtons();
    }

    if(!dock){
        layout_base->addLayout(layout_vertical);
    } else {
        QWidget* multiWidget = new QWidget();
        QDockWidget * dockWidget = new QDockWidget(name.c_str(), window);


        multiWidget->setLayout(layout_vertical);
        dockWidget->setWidget(multiWidget);
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


//! adds the default buttons to the interface
// ###################################################################
void SendIP::defaultButtons()
{
    myLinedit = new QLineEdit();
    layout_vertical->addWidget(myTitle);
    layout_horizontal->addWidget(myLinedit);
    layout_horizontal->addWidget(myPushButtonSend);
    layout_vertical->addLayout(layout_horizontal);
    QObject::connect(myPushButtonSend, SIGNAL(clicked()), this , SLOT(sendData())  );
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

//! send data button pushed
// ###################################################################
void SendIP::sendDataBool()
{
    ///qDebug()<< "sendData\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
    ///
    std::string mySendString;
    if(buttonTrue == true){
        mySendString = number;
        mySendString += ":1";
    } else {
        mySendString = number;
        mySendString += ":0";

    }

    qDebug() << "sendipSend " << mySendString.c_str() << " value " << mySendString.c_str();
    networkInterface->sendString(mySendString);


    if(buttonTrue == true){
        myPushButtonTrue->setDown(true);
        myPushButtonFalse->setDown(false);
    } else {
        myPushButtonTrue->setDown(false);
        myPushButtonFalse->setDown(true);
    }
}


//! send data button pushed
// ###################################################################
void SendIP::sendDataStructure()
{
    std::string mySendString = number + std::string(":");
    bool first = true;
    for (int i = 0; i < myNewQlineEdit_list.size(); ++i) {
        if(first == false){
            mySendString += ",";
        } else {
            first = false;
        }
        mySendString += myNewQlineEdit_list[i]->text().toStdString();
    }
    networkInterface->sendString(mySendString);

}


//! pushed the true button
// ###################################################################
void SendIP::on_myPushButtonTrue_clicked()
{
    if(buttonTrue == true){
        return;
    } else {
        buttonTrue = true;
        buttonFalse = false;
        myPushButtonTrue->setDown(true);
        myPushButtonFalse->setDown(false);
    }
}



//! pushed the false button
// ###################################################################
void SendIP::on_myPushButtonFalse_clicked()
{
    if(buttonTrue == false){
        return;
    } else {
        buttonTrue = false;
        buttonFalse = true;
        myPushButtonTrue->setDown(false);
        myPushButtonFalse->setDown(true);
    }
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

