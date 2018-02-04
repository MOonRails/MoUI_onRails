#include "mainwindow.h"
#include "ui_mainwindow.h"



//#include <QXmlSimpleReader>
#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>

#include <iostream>
#include <fstream>
#include <QCloseEvent>


//! constructor
// ###################################################################
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    qApp->setApplicationName("MoUI_onRails");


    lastRead = QDateTime::currentDateTime();

    statusBar()->showMessage(tr("Ready"));

    treeWidget = new QTreeWidget();
    treeWidget->setMaximumWidth(250);


    pushButton_file = new QPushButton("Open Config File");
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




    for (int i = 0; i < myServiceDisplays.size(); ++i) {
        delete myServiceDisplays[i];
        treeWidget->clear();
    }


    watcher = new QFileSystemWatcher(this);
    watcher->addPath(filename.c_str());
    QObject::connect(watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(on_fileChanged(const QString &)));



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
    myServiceDisplays.back()->addSpacer();
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



void MainWindow::on_fileChanged(QString myFile){

    // sometimes the operating system will trigger this method twice.
    // therefore we measure the time difference since the last triggering
    QDateTime lastWriteTime = QDateTime::currentDateTime();
    int offset = lastWriteTime.msecsTo(lastRead);
    lastRead = lastWriteTime;
    if (offset < -100)
    {

        // else discard the (duplicated) OnChanged event
        qDebug() << "on_fileChanged\n";


        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setText("<FONT COLOR='#000000'>The configuration file you have loaded has been modified.</FONT>");
        msgBox->setInformativeText("<FONT COLOR='#000000'>Do you want to reload the file?</FONT>");
        msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox->setIcon(QMessageBox::Warning);
        int ret = msgBox->exec();

        qDebug() << ret;
        if(ret == QMessageBox::Yes){
            qDebug() << "reloading";
            qDebug() << myFile;
            loadFile(myFile.toStdString());
        }


    }

}


void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Are you sure?",
                                                                "Are you sure?",
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        if(networkInterface->connectionStatus == 2){
            networkInterface->serialPort->close();
        }

        event->accept();
    }
}
