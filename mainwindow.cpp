#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlSimpleReader>


#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>

#include <iostream>
#include <fstream>
#include "servicedisplay.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    treeWidget = new QTreeWidget();
    treeWidget->setMaximumWidth(200);

    pushButton_file = new QPushButton("Open");
    pushButton_file->setFixedWidth(75);
    pushButton_connect = new QPushButton("Connect");
    pushButton_connect->setFixedWidth(75);
    QObject::connect(pushButton_file, SIGNAL(clicked()), this , SLOT(on_pushButton_file_clicked())  );
    QObject::connect(pushButton_connect, SIGNAL(clicked()), this , SLOT(on_pushButton_connect_clicked())  );
    lineEdit_ip = new QLineEdit();
    lineEdit_ip->setFixedWidth(75);
    lineEdit_ip->setText("127.0.0.1");
    lineEdit_port = new QLineEdit();
    lineEdit_port->setText("3000");
    lineEdit_port->setFixedWidth(75);






    QObject::connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(on_treeWidget_itemDoubleClicked(QTreeWidgetItem*,  int)));


    QHBoxLayout *buttonsHorizontallayout = new QHBoxLayout;
    //QVBoxLayout *mainVerticalLayout = new QVBoxLayout;

    /*buttonsHorizontallayout->addWidget(pushButton_file);
    buttonsHorizontallayout->addWidget(lineEdit_ip);
    buttonsHorizontallayout->addWidget(lineEdit_port);
    buttonsHorizontallayout->addWidget(pushButton_connect);
    ui->verticalLayout->addLayout(buttonsHorizontallayout);*/

    ui->verticalLayout->addWidget(pushButton_file);
    ui->verticalLayout->addWidget(lineEdit_ip);
    ui->verticalLayout->addWidget(lineEdit_port);
    ui->verticalLayout->addWidget(pushButton_connect);
    ui->verticalLayout->addWidget(treeWidget);





    /*QDockWidget * dockWidget = new QDockWidget(tr("MoOnRails"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
    dockWidget->setFloating(false);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    QWidget* multiWidget = new QWidget();
    QVBoxLayout *dockVerticalLayout = new QVBoxLayout;
    QLabel *title = new QLabel("MoOnRails");
    dockVerticalLayout->addWidget(title);
    multiWidget->setLayout(dockVerticalLayout);
    dockWidget->setWidget(multiWidget);*/


}






MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "clicked: " << column << " name: " << item->text(column);

    QString myTitle = item->text(column);

    QDockWidget * dockWidget = new QDockWidget(myTitle, this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
    dockWidget->setFloating(true);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

}



void MainWindow::loadFile(std::string filename){
    qDebug("function: loadFile\n");
    QFile* xmlFile = new QFile(filename.c_str());
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,"Load XML File Problem",
                              "Couldn't open xmlfile.xml to load settings for download",
                              QMessageBox::Ok);
        return;
    }
    QXmlStreamReader* xmlReader = new QXmlStreamReader(xmlFile);

    bool areaFound = false;
    std::string areaName = "";

    QTreeWidgetItem *myCurrentTreeWidgetItem;

    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        //qDebug() << "while " << xmlReader->name();
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader->name() == "service"){
                areaFound = true;
                //areaName = xmlReader->readElementText()
                //qDebug() << "1: " << xmlReader->readElementText();
                qDebug() << "1: " << xmlReader->name();
                //qDebug() << "1: " << xmlReader->qualifiedName();
                //qDebug() << "1: " << xmlReader->text();
                //qDebug() << "1: " << xmlReader->namespaceUri();
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "attr " << attribute_value;
                        areaName = attribute_value.toStdString();
                        addService(areaName);
                        // do something
                        myCurrentTreeWidgetItem = new QTreeWidgetItem(0);
                        myCurrentTreeWidgetItem->setText(0,attribute_value);
                        treeWidget->addTopLevelItem(myCurrentTreeWidgetItem);


                    }
                }
            } else if(xmlReader->name() == "pubsubIP"){

                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "name " << attribute_value;

                        QTreeWidgetItem *subTreeWidgetItem = new QTreeWidgetItem(0);
                        subTreeWidgetItem->setText(0,attribute_value);
                        myCurrentTreeWidgetItem->addChild(subTreeWidgetItem);

                        myServiceDisplays.front()->addDataReception(attribute_value.toStdString());
                    }
                    if (attr.name().toString() == QLatin1String("comment")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "comment " << attribute_value;
                    }
                    if (attr.name().toString() == QLatin1String("number")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "number " << attribute_value;
                    }
                    if (attr.name().toString() == QLatin1String("supportInReplay")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "supportInReplay " << attribute_value;
                    }
                }

            } else if(xmlReader->name() == "sendIP"){
                //addDataReception
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "name " << attribute_value;
                        QTreeWidgetItem *subTreeWidgetItem = new QTreeWidgetItem(0);
                        subTreeWidgetItem->setText(0,attribute_value);
                        myCurrentTreeWidgetItem->addChild(subTreeWidgetItem);
                        myServiceDisplays.front()->addDataSend(attribute_value.toStdString());
                    }
                    if (attr.name().toString() == QLatin1String("comment")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "comment " << attribute_value;
                    }
                    if (attr.name().toString() == QLatin1String("number")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "number " << attribute_value;
                    }
                    if (attr.name().toString() == QLatin1String("supportInReplay")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "supportInReplay " << attribute_value;
                    }
                }
            }


        } else if (token == QXmlStreamReader::EndElement){
            if(xmlReader->name() == "service"){
                qDebug() << "end: " << xmlReader->name();
                areaFound = false;
                areaName = "";
            }


        }

    }

    if(xmlReader->hasError()) {
            QMessageBox::critical(this,
            "xmlFile.xml Parse Error",xmlReader->errorString(),
            QMessageBox::Ok);
            return;
    }



}

void MainWindow::on_pushButton_file_clicked()
{
    qDebug("function: on_pushButton_newFDF_clicked\n");
    // open dialog to select file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Format Definition File"), QString(),tr("All Files (*. *.xml )"));

    if(fileName == ""){
        qDebug() << "No File Selected";
        return;
    }

    loadFile(fileName.toStdString());

}


//! launch of a service window window
// ########################################################
void MainWindow::addService(std::__cxx11::string serviceName){
  QDockWidget * qDockWidget_service = new QDockWidget(serviceName.c_str(), this);
  qDockWidget_service->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
  qDockWidget_service->setFloating(false);
  qDockWidget_service->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  ServiceDisplay* myNewWindow = new ServiceDisplay(qDockWidget_service, this);
  myServiceDisplays.push_back(myNewWindow);

  this->addDockWidget(Qt::RightDockWidgetArea, qDockWidget_service);
  //ui->centralWidget->setMinimumWidth(700);
  //ui->MainWindow->setMinimumWidth(700);
  }




//! connecting to an existing server
// ###################################################################
void MainWindow::on_pushButton_connect_clicked(){
    qDebug() << "on_pushButton_connect_clicked\n";

/*
    if(connectButtonStatus == false){
        disconnecting();
        return;
    }


    myListenButton->setDisabled(true);
    myConnectButton->setText("Disconnect");
    myConnectButton->setIcon(QIcon(":/icons/disconnect2.png"));
    myStatusLabel->setText("<font color='orange'>Connecting</font>");
    connectButtonStatus = false;
    myIP_Edit->setDisabled(true);
    myPortEdit->setDisabled(true);
*/
    clientConnection = new QTcpSocket();

    in = new QDataStream();
    tcpSocket = new QTcpSocket(this);
    in->setDevice(tcpSocket);
    in->setVersion(QDataStream::Qt_4_0);


    clientConnection->connectToHost(QHostAddress(lineEdit_ip->text()), lineEdit_port->text().toInt());

    QObject::connect(   clientConnection,   SIGNAL(connected())  ,    this   ,    SLOT(inComingFromServer())   );


}



void MainWindow::inComingFromServer(){
    qDebug() << "inComingFromServer\n";
/*

    std::string myLabelString = "";
    myLabelString += "<font color='green'>";
    myLabelString += clientConnection->peerAddress().toString().toStdString();
    myLabelString += "</font>";
    myStatusLabel->setText(myLabelString.c_str());

    QTimer *timer = new QTimer;
    timer->setSingleShot(true);*/


    QObject::connect(   clientConnection,   SIGNAL(disconnected())  ,    this   ,    SLOT(disconnected())   );


    in->startTransaction();



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
    while ( !in->atEnd() ) {
        *in >> nextByte;
        //qDebug() << "nextByte" << nextByte << "\n";
        QString myQString;
        myQString.append( tr("%1").arg(nextByte,2,16).replace(" ", "0") );
        //qDebug() << "myQString" << tr("%1").arg(nextByte,2,16) << "\n";
        retrievedString += myQString.toStdString();
    }


    // check if the retrieved data is empty
    if(retrievedString.length() == 0){
        qDebug() << "ERROR: empty frame\n";
        return;
    }


    // print hex data to consol
    //if(preferences->debugDataReceptionHex == true){
        qDebug() << "myReveived hex:\n";
        qDebug() << retrievedString.c_str() << "\n";
    //}
}



void MainWindow::disconnected(){
    qDebug() << "disconnected\n";

}

