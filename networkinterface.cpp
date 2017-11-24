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



    clientConnection = new QTcpSocket();
    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);

    QSettings settings(QSettings::UserScope, QLatin1String("MOoUI_onRails"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
    settings.endGroup();
    //preferences->connectToIP = lineEdit_ip->text().toStdString();
    //preferences->connectToPort = lineEdit_port->text().toInt();
    QNetworkConfigurationManager manager;
    manager.capabilities();

    QNetworkConfiguration config = manager.configurationFromIdentifier(id);
    if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
        config = manager.defaultConfiguration();
    }
    networkSession = new QNetworkSession(config, this);
    QObject::connect(networkSession, &QNetworkSession::opened,this, &NetworkInterface::sessionOpened);
    QObject::connect(clientConnection, &QIODevice::readyRead,this, &NetworkInterface::listeningToIncomingData);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);

    QObject::connect(clientConnection, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),this, &NetworkInterface::displayConnectionError );

    networkSession->open();





    lineEdit_ip->setDisabled(true);
    lineEdit_port->setDisabled(true);




    clientConnection->connectToHost(QHostAddress(lineEdit_ip->text()), lineEdit_port->text().toInt());
    qDebug() << "on_pushButton_connect_clicked2\n";

    QObject::connect(   clientConnection,   &QAbstractSocket::connected  ,    this   ,    &NetworkInterface::inComingFromServer   );

}

//! displayConnectionError
// ###################################################################
void NetworkInterface::displayConnectionError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found. Please check the host name and port settings.";
        QMessageBox::information(this, tr("Gepetto Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure the server is running";
        QMessageBox::information(this, tr("Gepetto Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, and"
                                    "check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        qDebug() << "The following error occurred: %1.";
        QMessageBox::information(this, tr("Gepetto Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(clientConnection->errorString()));
    }

    //getFortuneButton->setEnabled(true);
}


//! open session   - used by listening part
// ###################################################################
void NetworkInterface::sessionOpened()
{
    qDebug() << "sessionOpened\n";


    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();



}





//! listening to incoming data
// ###################################################################
void NetworkInterface::listeningToIncomingData()
{
    qDebug() << "\nlisteningToIncomingData: receiving data\n";
    in.startTransaction();





    quint8 nextByte;
    std::string retrievedString = "";
    std::string binaryString = "";

    int startingCol = 0;
    if(leftoverIncomingData_column != -1){
        //qDebug() << "using leftover data " << leftoverIncomingData.length();
        binaryString = leftoverIncomingData;
        startingCol = leftoverIncomingData_column;
        leftoverIncomingData = "";
    }


    // loop until all data is reveived
    while ( !in.atEnd() ) {
        QString myQString2;
        in >> nextByte;
        //qDebug() << "nextByte" << nextByte << "\n";
        QString myQString;
        myQString.append( tr("%1").arg(nextByte,2,16).replace(" ", "0") );
        retrievedString += myQString.toStdString();


    }


    // check if the retrieved data is empty
    if(retrievedString.length() == 0){
        qDebug() << "ERROR: empty frame\n";
        return;
    }





    QByteArray text = QByteArray::fromHex(retrievedString.c_str());
    text = text.trimmed();
    qDebug() << "retrievedString " << text;
    QString Qtext = text;
    if(Qtext == ""){
        return;
    }
    QStringList pieces = Qtext.split( ":" );


    if(pieces[0] == "STR"){
        return;
    }

    pusblishIp_String(pieces[0].toStdString(),pieces[1].toStdString()   );







}



//! connecting to an existing server
// ###################################################################
void NetworkInterface::inComingFromServer(){
    qDebug() << "inComingFromServer\n";
    QObject::connect(   clientConnection,   &QAbstractSocket::disconnected ,    this   ,    &NetworkInterface::disconnected   );
    QObject::connect(   clientConnection,   &QIODevice::readyRead   ,    this   , &NetworkInterface::readIncoming );


}


//! read incoming data from server
// ###################################################################
void NetworkInterface::readIncoming(){
    qDebug() << "readIncoming: ";





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
void NetworkInterface::pusblishIp_String(std::string myNumber,std::string myText){
    qDebug() << "myNumber: " << myNumber.c_str() << "myText: " << myText.c_str();

    QString myQText = myText.c_str();
    qDebug() << "myNumber: " << myQText.toFloat();
    for (int i = 0; i < myPublishIP_list.size(); ++i) {
        if(myPublishIP_list[i]->number == myNumber){
            //qDebug() << "found " << myPublishIP_list[i]->name.c_str();
            //myPublishIP_list[i]->setValue(21);
            myPublishIP_list[i]->setValue(myQText.toFloat());
        }
    }

}



//! adding a publish ip object to the myPublishIP_list
// ###################################################################
void NetworkInterface::addPublishIP(PublishIP* myPublishIP){
    myPublishIP_list.push_back(myPublishIP);
}




