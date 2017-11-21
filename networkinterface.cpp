#include "networkinterface.h"
//#include "mainwindow.h"


#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>


//! constructor
// ###################################################################
//NetworkInterface::NetworkInterface(QVBoxLayout *layout_base, QMainWindow * mymainwindow,Ui::MainWindow * myUi)
NetworkInterface::NetworkInterface(QVBoxLayout *layout_base)
{
    //ui = myUi;
    myServer = new QTcpServer();

    pushButton_connect = new QPushButton("Connect");
    pushButton_connect->setFixedWidth(75);

    clientConnection = new QTcpSocket();

    QObject::connect(pushButton_connect, SIGNAL(clicked()), this , SLOT(on_pushButton_connect_clicked())  );
    lineEdit_ip = new QLineEdit();
    lineEdit_ip->setFixedWidth(75);
    lineEdit_ip->setText("127.0.0.1");
    lineEdit_port = new QLineEdit();
    lineEdit_port->setText("5555");
    lineEdit_port->setFixedWidth(75);



    layout_base->addWidget(lineEdit_ip);
    layout_base->addWidget(lineEdit_port);
    layout_base->addWidget(pushButton_connect);
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

    pushButton_connect->setText("Disconnect");
    connectButtonStatus = true;

    //myListenButton->setDisabled(true);
    //myConnectButton->setText("Disconnect");
    //myConnectButton->setIcon(QIcon(":/icons/disconnect2.png"));


    //myStatusLabel->setText("<font color='orange'>Connecting</font>");
    //connectButtonStatus = false;
    //myIP_Edit->setDisabled(true);
    //myPortEdit->setDisabled(true);




    clientConnection->connectToHost(QHostAddress(lineEdit_ip->text()), lineEdit_port->text().toInt());
    qDebug() << "on_pushButton_connect_clicked2\n";

    QObject::connect(   clientConnection,   SIGNAL(connected())  ,    this   ,    SLOT(inComingFromServer())   );


}



//! connecting to an existing server
// ###################################################################
void NetworkInterface::inComingFromServer(){
    qDebug() << "inComingFromServer\n";
    QObject::connect(   clientConnection,   SIGNAL(disconnected())  ,    this   ,    SLOT(disconnected())   );
/*
    int counter = 0;
    while(broadcastData == false){
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        if(nextItem == true){
            nextItem = false;
            qDebug() << "next item\n";
            break;
        }
    }*/

    const char * myChar = "6:1\n";
    clientConnection->write(myChar);
    clientConnection->flush(); // this sends the actual data



    in.startTransaction();

    QString myReturnString;
    in >> myReturnString;
    qDebug() << myReturnString << "\n";

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
    myServer->close();
    //myConnectButton->setDisabled(false);
    //myListenButton->setDisabled(false);

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
    clientConnection = myServer->nextPendingConnection();


    QObject::connect(   clientConnection,   SIGNAL(disconnected())  ,    this   ,    SLOT(disconnected())   );
    qDebug() << "Done Sending\n";
}


//! send a string to the server
// ###################################################################
void NetworkInterface::sendString(std::string myString){
    qDebug() << "Sending String: " << myString.c_str();

    //ui->statusBar->showMessage( myString.c_str());
}


//! send a string to the server
// ###################################################################
void NetworkInterface::findPusblishIp(std::string myNumber){
    qDebug() << "findPusblishIp: " << myNumber.c_str();
    //for (int i = 0; i < myServices.size(); ++i) {
    //    i->findPublishList(myNumber);
    //}

}
