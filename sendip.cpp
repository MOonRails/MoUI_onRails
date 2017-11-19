#include "sendip.h"


//! constructor
// ###################################################################
SendIP::SendIP(QVBoxLayout *layout_base,  std::string title)
{

    QLabel* myTitle = new QLabel(title.c_str());
    myLinedit = new QLineEdit();
    myPushButtonSend = new QPushButton("Send");


    QHBoxLayout *layout_horizontal = new QHBoxLayout;
    QVBoxLayout *layout_vertical = new QVBoxLayout;

    QObject::connect(myPushButtonSend, SIGNAL(clicked()), this , SLOT(sendData())  );


    layout_vertical->addWidget(myTitle);
    layout_horizontal->addWidget(myLinedit);
    layout_horizontal->addWidget(myPushButtonSend);
    layout_vertical->addLayout(layout_horizontal);


    layout_base->addLayout(layout_vertical);



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
    qDebug()<< "sendData\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
    qDebug() << myLinedit->text();

}
