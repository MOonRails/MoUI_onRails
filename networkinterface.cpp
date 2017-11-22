#include "networkinterface.h"
//#include "mainwindow.h"


#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>


//! constructor
// ###################################################################
//NetworkInterface::NetworkInterface(QVBoxLayout *layout_base)
NetworkInterface::NetworkInterface(QVBoxLayout *layout_base, QMainWindow * mymainwindow,Ui::MainWindow * myUi)
{
    ui = myUi;
    //myServer = new QTcpServer();

    pushButton_connect = new QPushButton("Connect");
    //pushButton_connect->setFixedWidth(50);
    pushButton_connect->setMaximumWidth(70);




    QObject::connect(pushButton_connect, SIGNAL(clicked()), this , SLOT(on_pushButton_connect_clicked())  );
    lineEdit_ip = new QLineEdit();
    //lineEdit_ip->setFixedWidth(94);
    lineEdit_ip->setMaximumWidth(84);
    lineEdit_ip->setText("127.0.0.1");
    lineEdit_port = new QLineEdit();
    lineEdit_port->setText("5555");
    //lineEdit_port->setFixedWidth(94);
    lineEdit_port->setMaximumWidth(84);

    QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding,QSizePolicy::Minimum);
    QHBoxLayout *layout_Horizontal = new QHBoxLayout();
    layout_Horizontal->addWidget(lineEdit_ip);
    layout_Horizontal->addWidget(lineEdit_port);
    layout_Horizontal->addWidget(pushButton_connect);
    layout_Horizontal->addSpacerItem(spacer);
    layout_base->addLayout(layout_Horizontal);



/*
    layout_base->addWidget(lineEdit_ip);
    layout_base->addWidget(lineEdit_port);
    layout_base->addWidget(pushButton_connect);*/
}


//! destructor
// ###################################################################
NetworkInterface::~NetworkInterface()
{

}






//! connecting to an existing server
// ###################################################################
void NetworkInterface::on_pushButton_connect_clicked(){
    qDebug() << "on_pushButton_connect_clicked\n";



    if(connectButtonStatus == true){
        //qDebug() << "disconnecting button\n";
        disconnecting();
        return;
    }

    clientConnection = new QTcpSocket();
    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);

    /*QSettings settings(QSettings::UserScope, QLatin1String("MOoUI_onRails"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
    settings.endGroup();
    preferences->connectToIP = ipAddress.toStdString();
    preferences->connectToPort = portNumber;*/



    pushButton_connect->setText("Disconnect");
    connectButtonStatus = true;


    lineEdit_ip->setDisabled(true);
    lineEdit_port->setDisabled(true);




    clientConnection->connectToHost(QHostAddress(lineEdit_ip->text()), lineEdit_port->text().toInt());
    qDebug() << "on_pushButton_connect_clicked2\n";

    QObject::connect(   clientConnection,   SIGNAL(connected())  ,    this   ,    SLOT(inComingFromServer())   );

}



//! connecting to an existing server
// ###################################################################
void NetworkInterface::inComingFromServer(){
    qDebug() << "inComingFromServer\n";
    QObject::connect(   clientConnection,   SIGNAL(disconnected())  ,    this   ,    SLOT(disconnected())   );
    QObject::connect(   clientConnection,   &QIODevice::readyRead   ,    this   , readIncoming);
/*

    const char * myChar = "6:1\n";
    clientConnection->write(myChar);
    clientConnection->flush(); // this sends the actual data
*/
   /* int counter = 0;
    while(connectButtonStatus == true){
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );

    }*/
/*
    in.startTransaction();
    QString myReturnString;
    in >> myReturnString;
    qDebug() << myReturnString << "\n";*/

}


//! read incoming data from server
// ###################################################################
void NetworkInterface::readIncoming(){
    qDebug() << "readIncoming: ";



    /*while (clientConnection->bytesAvailable())
        {
            buffer.append(clientConnection->readAll());
            int packetSize = getPacketSize(buffer);
            while(packetSize>0)
            {
                handlePacket(buffer.left(packetSize);
                buffer.remove(0,packetSize);
                packetSize = getPacketSize(buffer);
            }
        }*/

    in.startTransaction();

    while (clientConnection->bytesAvailable()){
        if(connectButtonStatus == false){
            return;
        }
        QString str;
        qint32 a;
        in >> str >> a; // try to read packet atomically

        if (!in.commitTransaction())
            qDebug() << "waiting";
            //return;     // wait for more data
        else {
            qDebug() << "reading " << str;
        }
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }

/*
    in.startTransaction();

        QString data;
        in >> data;

        if (!in.commitTransaction())
        {
            qDebug() << "incomplete: " << data;
            // readyRead will be called again when there is more data
            return;
        }
        qDebug() << "printingNetworkData " << data;*/
    /*QString incomingString;
    in >> incomingString;

    if (!in.commitTransaction())
        qDebug() << "!in.commitTransaction " << incomingString;
        return;*/

    /*if (incomingString == currentFortune) {
        QTimer::singleShot(0, this, &Client::requestNewFortune);
        return;
    }*/



    //qDebug() << "incoming " << incomingString;
    //statusLabel->setText(currentFortune);
    //getFortuneButton->setEnabled(true);

    //in.startTransaction();
    //QString myReturnString;
    //in >> myReturnString;
    //qDebug() <<"test2" << myReturnString << "\n";
}

//! Connected
// ###################################################################
void NetworkInterface::connection(){
    qDebug() << "Connected!";
}


//! disconnect from client and stop listening
// ###################################################################
void NetworkInterface::disconnecting(){
    clientConnection->close();


    lineEdit_ip->setDisabled(false);
    lineEdit_port->setDisabled(false);

    //myConnectButton->setText("Connect");
    //myConnectButton->setIcon(QIcon(":/icons/connect2.png"));
    //myListenButton->setText("Listen");
    //myListenButton->setIcon(QIcon(":/icons/server_connect2.png"));
    pushButton_connect->setText("Connect");
    connectButtonStatus = false;
    qDebug() << "Disconnected!\n";
}


//! connection to client lost
// ###################################################################
void NetworkInterface::disconnected(){
    disconnecting();
    qDebug() << "Connection to client lost!\n";
}



//! when a connection with a client is opened, do this
//! check
// ###################################################################
void NetworkInterface::newConnection(){
    qDebug() << "TransferStack newConnection!\n";
    //clientConnection = myServer->nextPendingConnection();


    QObject::connect(   clientConnection,   SIGNAL(disconnected())  ,    this   ,    SLOT(disconnected())   );
    qDebug() << "Done Sending\n";
}


//! send a string to the server
// ###################################################################
void NetworkInterface::sendString(std::string myString){
    if(connectButtonStatus == true){
        qDebug() << "Sending String: " << myString.c_str();
        myString += "\n";
        const char * myChar = myString.c_str();
        clientConnection->write(myChar);
        clientConnection->flush(); // this sends the actual data

        std::string myMessage = "Sending Command: " + myString;
        ui->statusBar->showMessage(myMessage.c_str());
    } else {
        qDebug() << "No Connection to send Command: " << myString.c_str();
        std::string myMessage = "No Connection to send Command: " + myString;
        ui->statusBar->showMessage(myMessage.c_str());
    }

}


//! send a string to the server
// ###################################################################
void NetworkInterface::findPusblishIp(std::string myNumber){
    //qDebug() << "findPusblishIp: " << myNumber.c_str();
    for (int i = 0; i < myPublishIP_list.size(); ++i) {
        if(myPublishIP_list[i]->number == myNumber){
            //qDebug() << "found " << myPublishIP_list[i]->name.c_str();
            //myPublishIP_list[i]->setValue(21);
        }
    }

}



//! adding a publish ip object to the myPublishIP_list
// ###################################################################
void NetworkInterface::addPublishIP(PublishIP* myPublishIP){
    myPublishIP_list.push_back(myPublishIP);
}

/*
//! adding a publish ip object to the myPublishIP_list
// ###################################################################
void NetworkInterface::addSendIP(SendIP* mySendIP){
    myPublishIP_list.push_back(myPublishIP);
}*/



