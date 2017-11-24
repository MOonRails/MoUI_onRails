#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include "servicedisplay.h"
//#include "networkinterface.h"

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



//! constructor
// ###################################################################
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);





    statusBar()->showMessage(tr("Ready"));

    treeWidget = new QTreeWidget();
    treeWidget->setMaximumWidth(250);


    pushButton_file = new QPushButton("Open");
    pushButton_file->setFixedWidth(250);
    QObject::connect(pushButton_file, SIGNAL(clicked()), this , SLOT(on_pushButton_file_clicked())  );

    ui->verticalLayout->addWidget(pushButton_file);


    networkInterface = new NetworkInterface(ui->verticalLayout, this, ui);


    QObject::connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(on_treeWidget_itemDoubleClicked(QTreeWidgetItem*,  int)));

    ui->verticalLayout->addWidget(treeWidget);


    QApplication::setStyle(QStyleFactory::create("Dark"));
    QPalette palette;


    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::Highlight, QColor(142,45,197));
    palette.setColor(QPalette::ButtonText, QColor(0,0,0));
    palette.setColor(QPalette::Base, QColor(200,200,200));
    palette.setColor(QPalette::WindowText, QColor(255,255,255));
    qApp->setPalette(palette);

    treeWidget->setHeaderLabel("Load File to start");
}






MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "clicked: " << column << " name: " << item->text(column);

    QString myTitle = item->text(column);
    qDebug() << "clicked: " << column << " parent: " << item->parent();
    QTreeWidgetItem *myCurrentTreeWidgetItem = item->parent();
    qDebug() << "clicked: " << column << " parent2: " << myCurrentTreeWidgetItem->text(0);

/*
    QDockWidget * dockWidget = new QDockWidget(myTitle, this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
    dockWidget->setFloating(true);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);*/

}


//! load and parse file from disk
// ###################################################################
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
    QTreeWidgetItem *myCurrentTreeWidgetItemPusblish;
    QTreeWidgetItem *myCurrentTreeWidgetItemSend;

    std::string name,comment,number,supportInReplay = "";
    std::string area,list,typeName,service = "";
    std::string field= "";

    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        //qDebug() << "while " << xmlReader->name();
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader->name() == "service"){
                areaFound = true;
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        qDebug() << "attr " << attribute_value;
                        areaName = attribute_value.toStdString();
                        addService(areaName);
                        // do something
                        myCurrentTreeWidgetItem = new QTreeWidgetItem(0);
                        myCurrentTreeWidgetItemPusblish = new QTreeWidgetItem(0);
                        myCurrentTreeWidgetItemSend = new QTreeWidgetItem(0);
                        myCurrentTreeWidgetItem->setText(0,attribute_value);
                        myCurrentTreeWidgetItemPusblish->setText(0,"Receive");
                        myCurrentTreeWidgetItemSend->setText(0,"Send");
                        treeWidget->addTopLevelItem(myCurrentTreeWidgetItem);
                        myCurrentTreeWidgetItem->addChild(myCurrentTreeWidgetItemPusblish);
                        myCurrentTreeWidgetItem->addChild(myCurrentTreeWidgetItemSend);


                    }
                }
            } else if(xmlReader->name() == "pubsubIP"){
                //std::string name,comment,number,supportInReplay = "";
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "name " << attribute_value;
                        name = attribute_value.toStdString();

                    }
                    if (attr.name().toString() == QLatin1String("comment")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "comment " << attribute_value;
                        comment = attribute_value.toStdString();
                    }
                    if (attr.name().toString() == QLatin1String("number")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "number " << attribute_value;
                        number = attribute_value.toStdString();
                    }
                    if (attr.name().toString() == QLatin1String("supportInReplay")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "supportInReplay " << attribute_value;
                        supportInReplay = attribute_value.toStdString();
                    }


                }
                /*QTreeWidgetItem *subTreeWidgetItem = new QTreeWidgetItem(0);
                subTreeWidgetItem->setText(0,name.c_str());
                myCurrentTreeWidgetItem->addChild(subTreeWidgetItem);
                //myServiceDisplays.front()->addDataReception(networkInterface,name, comment,number,supportInReplay);
                myServiceDisplays.front()->addDataReception(name, comment,number,supportInReplay);*/
            } else if(xmlReader->name() == "sendIP"){
                //std::string name,comment,number,supportInReplay = "";
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "name " << attribute_value;
                        name = attribute_value.toStdString();
                    }
                    if (attr.name().toString() == QLatin1String("comment")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "comment " << attribute_value;
                        comment = attribute_value.toStdString();
                    }
                    if (attr.name().toString() == QLatin1String("number")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "number " << attribute_value;
                        number = attribute_value.toStdString();
                    }
                    if (attr.name().toString() == QLatin1String("supportInReplay")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "supportInReplay " << attribute_value;
                        supportInReplay = attribute_value.toStdString();
                    }
                }
                /*QTreeWidgetItem *subTreeWidgetItem = new QTreeWidgetItem(0);
                subTreeWidgetItem->setText(0,name.c_str());
                myCurrentTreeWidgetItem->addChild(subTreeWidgetItem);
                qDebug() << "number " << number.c_str() << " supportInReplay " << supportInReplay.c_str();
                //myServiceDisplays.front()->addDataSend(networkInterface,name, comment,number,supportInReplay,this);
                myServiceDisplays.front()->addDataSend(name, comment,number,supportInReplay);*/



            } else if(xmlReader->name() == "type"){
                //std::string area,list,name,service = "";
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("area")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "area " << attribute_value;
                        area = attribute_value.toStdString();

                    }
                    if (attr.name().toString() == QLatin1String("list")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "list " << attribute_value;
                        list = attribute_value.toStdString();
                    }
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "name " << attribute_value;
                        typeName = attribute_value.toStdString();
                    }
                    if (attr.name().toString() == QLatin1String("service")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "service " << attribute_value;
                        service = attribute_value.toStdString();
                    }

                }
            } else if(xmlReader->name() == "field"){
                //std::string field= "";
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        //qDebug() << "field " << attribute_value;
                        field = attribute_value.toStdString();
                    }
                }
            }


        } else if (token == QXmlStreamReader::EndElement){
            if(xmlReader->name() == "service"){
                //qDebug() << "end: " << xmlReader->name();
                areaFound = false;
                areaName = "";
                std::string name,comment,number,supportInReplay,area,list,typeName,service,field = "";
            } else if(xmlReader->name() == "pubsubIP"){
                QTreeWidgetItem *subTreeWidgetItem = new QTreeWidgetItem(0);
                subTreeWidgetItem->setText(0,name.c_str());
                myCurrentTreeWidgetItemPusblish->addChild(subTreeWidgetItem);
                myServiceDisplays.back()->addDataReception(name, comment,number,supportInReplay);
                std::string name,comment,number,supportInReplay,area,list,typeName,service,field = "";

            } else if(xmlReader->name() == "sendIP"){
                QTreeWidgetItem *subTreeWidgetItem = new QTreeWidgetItem(0);
                subTreeWidgetItem->setText(0,name.c_str());
                myCurrentTreeWidgetItemSend->addChild(subTreeWidgetItem);

                std::vector<std::string>*myTypeList = new std::vector<std::string>;
                std::vector<std::string>*myNamesList = new std::vector<std::string>;
                if(typeName != "Boolean" && typeName != "Float" && typeName != "UOctet" ){
                    if(findDataType(filename,typeName,myTypeList,myNamesList)){
                        //qDebug() << "###############FoundValue " << typeName.c_str();
                        //qDebug() << "myTypeList " << myTypeList->size();
                    } //else {
                        //qDebug() << "###############not found " << typeName.c_str();
                    //}

                }
                //std::vector<std::string>*myTypeList2;
                //std::vector<std::string>*myNamesList2;
                myServiceDisplays.back()->addDataSend(name, comment,number,supportInReplay,field,typeName,myTypeList,myNamesList);
                std::string name,comment,number,supportInReplay,area,list,typeName,service,field = "";
            }

        }

    }

    if(xmlReader->hasError()) {
            QMessageBox::critical(this,
            "xmlFile.xml Parse Error",xmlReader->errorString(),
            QMessageBox::Ok);
            return;
    }

    // end of parsing actions
    myServiceDisplays.front()->addSpacer();
    treeWidget->expandAll();
    //QTreeWidgetItem* header = treeWidget->headerItem();
    //header->setText(0, "My Text");
    treeWidget->setHeaderLabel(filename.c_str());

}


//! opens the file menu and parses the selected xml file
// ########################################################
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
void MainWindow::addService(std::string serviceName){
  QDockWidget * qDockWidget_service = new QDockWidget(serviceName.c_str(), this);
  qDockWidget_service->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
  qDockWidget_service->setFloating(false);
  qDockWidget_service->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  Service_Display* myNewWindow = new Service_Display(qDockWidget_service, this, networkInterface);
  //networkInterface->myServices.push_back(myNewWindow);
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
/*    clientConnection = new QTcpSocket();

    in = new QDataStream();
    tcpSocket = new QTcpSocket(this);
    in->setDevice(tcpSocket);
    in->setVersion(QDataStream::Qt_4_0);


    clientConnection->connectToHost(QHostAddress(lineEdit_ip->text()), lineEdit_port->text().toInt());

    QObject::connect(   clientConnection,   SIGNAL(connected())  ,    this   ,    SLOT(inComingFromServer())   );

*/
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






//! load and parse file from disk
// ###################################################################
bool MainWindow::findDataType(std::string filename, std::string dataType, std::vector<std::string> *myTypeList, std::vector<std::string> *myNamesList){
    qDebug() << "findDataType: " << dataType.c_str() ;
    QFile* xmlFile = new QFile(filename.c_str());
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,"Load XML File Problem",
                              "Couldn't open xmlfile.xml to load settings for download",
                              QMessageBox::Ok);
        return false;
    }
    QXmlStreamReader* xmlReader = new QXmlStreamReader(xmlFile);

    bool myCompositFound = false;


    //Parse the XML until we reach end of it
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader->name() == "composite"){
                foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                    if (attr.name().toString() == QLatin1String("name")) {
                        QString attribute_value = attr.value().toString();
                        if (attribute_value == dataType.c_str()) {
                            //qDebug() << "found datatype";
                            myCompositFound = true;
                        } else {
                            myCompositFound = false;
                        }
                    }

                }
            } else if(xmlReader->name() == "field"){
                if(myCompositFound == true){
                    foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                        if (attr.name().toString() == QLatin1String("name")) {
                            QString attribute_value = attr.value().toString();
                            myNamesList->push_back(attribute_value.toStdString());
                        }
                    }
                }
            } else if(xmlReader->name() == "type"){
                if(myCompositFound == true){
                    foreach(const QXmlStreamAttribute &attr, xmlReader->attributes()) {
                        if (attr.name().toString() == QLatin1String("name")) {
                            QString attribute_value = attr.value().toString();
                            myTypeList->push_back(attribute_value.toStdString());
                        }
                    }
                }
            }


        } else if (token == QXmlStreamReader::EndElement){
            if(xmlReader->name() == "dataTypes"){
                if(myCompositFound == true){
                    return true;
                }
            }
        }

    }

    if(xmlReader->hasError()) {
            QMessageBox::critical(this,
            "xmlFile.xml Parse Error",xmlReader->errorString(),
            QMessageBox::Ok);
            return false;
    }

    return false;
    // end of parsing actions


}
