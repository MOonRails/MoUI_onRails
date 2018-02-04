
#include "networkinterface.h"
//#include "mainwindow.h"


#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>


#include <QCoreApplication>
#include <QSerialPort>
#include <QStringList>
#include <QTextStream>

//! constructor
// ###################################################################
//NetworkInterface::NetworkInterface(QVBoxLayout *layout_base)
NetworkInterface::NetworkInterface(QVBoxLayout *layout_base, QMainWindow * mymainwindow,Ui::MainWindow * myUi)
{
    ui = myUi;
    //myServer = new QTcpServer();


    pushButton_connectIP = new QPushButton("Connect IP");
    pushButton_connectIP->setToolTip("Connect to desired IP address");
    pushButton_connectIP->setFixedWidth(85);


    pushButton_connectSerial = new QPushButton("Serial");
    pushButton_connectSerial->setToolTip("Connect to selected serial port, (currently no data recpetion supported)");
    pushButton_connectSerial->setFixedWidth(85);

    QObject::connect(pushButton_connectIP, SIGNAL(clicked()), this , SLOT(on_pushButton_connectIP_clicked())  );
    QObject::connect(pushButton_connectSerial, SIGNAL(clicked()), this , SLOT(on_pushButton_connectSerial_clicked())  );

    lineEdit_ip = new QLineEdit();
    lineEdit_ip->setMaximumWidth(77);
    lineEdit_ip->setText("127.0.0.1");
    lineEdit_port = new QLineEdit();
    lineEdit_port->setText("5555");
    lineEdit_port->setMaximumWidth(77);

    QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding,QSizePolicy::Minimum);
    QHBoxLayout *layout_Horizontal_ip = new QHBoxLayout();
    layout_Horizontal_ip->addWidget(lineEdit_ip);
    layout_Horizontal_ip->addWidget(lineEdit_port);
    layout_Horizontal_ip->addWidget(pushButton_connectIP);
    layout_Horizontal_ip->addSpacerItem(spacer);
    layout_base->addLayout(layout_Horizontal_ip);


    QSpacerItem *spacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding,QSizePolicy::Minimum);
    QHBoxLayout *layout_Horizontal_serial = new QHBoxLayout();
    combobox_serial = new QComboBox();



    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QString s = QObject::tr("Port: ") + info.portName() + "\n"
                + QObject::tr("Location: ") + info.systemLocation() + "\n"
                + QObject::tr("Description: ") + info.description() + "\n"
                + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

        QString label = info.portName() + " : " + info.description();

        combobox_serial->addItem(label);

        //serialPortsList << info.portName();
        serialPortsList.append(info.portName());
    }



    combobox_serial->setFixedWidth(160);


    layout_Horizontal_serial->addWidget(combobox_serial);
    layout_Horizontal_serial->addWidget(pushButton_connectSerial);
    layout_Horizontal_serial->addSpacerItem(spacer2);
    layout_base->addLayout(layout_Horizontal_serial);




}


//! destructor
// ###################################################################
NetworkInterface::~NetworkInterface()
{

}






//! connecting to an existing server
// ###################################################################
void NetworkInterface::on_pushButton_connectIP_clicked(){
    qDebug() << "on_pushButton_connectIP_clicked\n";



    if(connectionStatus == 1){
        //qDebug() << "disconnecting button\n";
        disconnecting();
        pushButton_connectSerial->setEnabled(true);
        pushButton_connectSerial->setFlat(false);
        return;
    }
    pushButton_connectIP->setText("Disconnect");
    connectionStatus = 1;



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
    pushButton_connectSerial->setEnabled(false);
    pushButton_connectSerial->setFlat(true);




    clientConnection->connectToHost(QHostAddress(lineEdit_ip->text()), lineEdit_port->text().toInt());
    qDebug() << "on_pushButton_connectIP_clicked2\n";

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
        if(connectionStatus == 0){
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
    pushButton_connectIP->setText("Connect IP");
    connectionStatus = 0;
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
    if(connectionStatus == 1){ // connected on tcpip
        qDebug() << "Sending String: " << myString.c_str();
        myString += "\n";
        const char * myChar = myString.c_str();
        clientConnection->write(myChar);
        clientConnection->flush(); // this sends the actual data

        std::string myMessage = "Sending Command: " + myString;
        ui->statusBar->showMessage(myMessage.c_str());
    } else if(connectionStatus == 2) { // connected to Serial port
        qDebug() << "Sending String: " << myString.c_str();
        myString += "\n";

        serialPort->write(myString.c_str());
        serialPort->flush();

        std::string myMessage = "Sending Command: " + myString;
        lastCommandSend = myString.c_str();
        lastCommandSend.remove(QRegExp("[\\n\\t\\r]"));

        ui->statusBar->showMessage(myMessage.c_str());
    } else { // no connection - connectionStatus == 0
        qDebug() << "No Connection to send Command: " << myString.c_str();
        std::string myMessage = "No Connection to send Command: " + myString;
        ui->statusBar->showMessage(myMessage.c_str());
    }

}


//! pusblish a received string into one of the graphs in our display
// ###################################################################
void NetworkInterface::pusblishIp_String(std::string myNumber,std::string myText){
    qDebug() << "myNumber: " << myNumber.c_str() << "myText: " << myText.c_str();

    QString myQText = myText.c_str();
    qDebug() << "myNumber: " << myQText.toFloat();
    for (int i = 0; i < myPublishIP_list.size(); ++i) {
        if(myPublishIP_list[i]->number == myNumber){
            myPublishIP_list[i]->setValue(myQText.toFloat());
        }
    }

}



//! adding a publish ip object to the myPublishIP_list
// ###################################################################
void NetworkInterface::addPublishIP(PublishIP* myPublishIP){
    myPublishIP_list.push_back(myPublishIP);
}



//! adding a publish ip object to the myPublishIP_list
// ###################################################################
void NetworkInterface::on_pushButton_connectSerial_clicked(){


    if(connectionStatus == 2){
        pushButton_connectSerial->setText("Serial");
        connectionStatus = 0;
        combobox_serial->setEnabled(true);
        pushButton_connectIP->setEnabled(true);
        pushButton_connectIP->setFlat(false);
        serialPort->close();
        return;
    }

    const auto infos = QSerialPortInfo::availablePorts();


    if(serialPortsList.size() != 0){



        serialPort = new QSerialPort();
        const QString serialPortName = serialPortsList[combobox_serial->currentIndex()];
        serialPort->setPortName(serialPortName);

        const int serialPortBaudRate = 115200;/// info.standardBaudRates();
        serialPort->setBaudRate(serialPortBaudRate);

        serialPort->setParity(QSerialPort::NoParity);

        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        serialPort->setStopBits(QSerialPort::OneStop);
        if (!serialPort->open(QIODevice::ReadWrite)) {
            qDebug() << QObject::tr("Failed to open port %1, error: %2")
                              .arg(serialPortName).arg(serialPort->error()) << endl;
            ui->statusBar->showMessage("Connection not possible");
            return;

        } else {

            pushButton_connectSerial->setText("Disconnect");
            connectionStatus = 2;
            ui->statusBar->showMessage("Connected");

            combobox_serial->setEnabled(false);
            pushButton_connectIP->setEnabled(false);
            pushButton_connectIP->setFlat(true);

            qApp->processEvents();
        }

        /*QThread::sleep(2); // give the system some time to boot up

        QString test2("2:1\n");
        qDebug() << "out: " << test2;


        serialPort->write(test2.toUtf8());
        serialPort->flush();


        QByteArray input;

        serialPort->waitForBytesWritten(100);
        serialPort->waitForReadyRead(100);
        if(serialPort->bytesAvailable()>=18)
            input = serialPort->read(18);
        qDebug()<<input;*/


        QObject::connect(   serialPort,   &QIODevice::readyRead   ,    this   , &NetworkInterface::readIncomingFromSerial );



        return;
    }





    qDebug() << "no serial port found";







}






void NetworkInterface::openSerialPort()
{

}


void NetworkInterface::closeSerialPort()
{

}


void NetworkInterface::writeData(const QByteArray &data)
{
    serialPort->write(data);
}


void NetworkInterface::readData()
{

}



//! data incoming from concected serial port
// ###################################################################
void NetworkInterface::readIncomingFromSerial(){

    QByteArray readData = serialPort->readAll();

    if (serialPort->error() == QSerialPort::ReadError) {
        qDebug() << QObject::tr("Failed to read from port %1, error: %2")
                    .arg("serialPortName").arg(serialPort->errorString());
        return;
    } else if (serialPort->error() == QSerialPort::TimeoutError && readData.isEmpty()) {
        qDebug() << QObject::tr("No data was currently available"
                                " for reading from port %1")
                    .arg("serialPortName");
        return;
    }

    QString DataAsString(readData);
    QString DataAsStringSub = DataAsString;
    //qDebug() << "first 3 chras " << DataAsStringSub.mid(0,3);

    if(DataAsStringSub.mid(0,3) == "STR"){

        //qDebug() << "STR " << DataAsString;
        DataAsString.remove(QRegExp("[\\n\\t\\r]"));
        QStringList pieces = DataAsString.split( "STR::" );

        qDebug() << "STR " << pieces[pieces.size()-2];



        if (pieces[pieces.size()-2] == lastCommandSend){
            std::string myMessage = "Sending Command: " + lastCommandSend.toStdString() + " - confirmed";
            lastCommandSend = "";
            ui->statusBar->showMessage(myMessage.c_str());
        }
        return;
    } else if(DataAsStringSub.mid(0,3) == "TR:") {
        qDebug() << "TR: " << DataAsString;
        return;
    }


    DataAsString.remove(QRegExp("[\\n\\t\\r]"));
    //qDebug() << "DataAsString removed " << DataAsString;


    QStringList pieces = DataAsString.split( ":" );
    //qDebug() << "piece 0 " << pieces[0];
    //qDebug() << "piece 1 " << pieces[1];
    pusblishIp_String(pieces[0].toStdString(),pieces[1].toStdString()   );


}

