#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlSimpleReader>


#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>



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



    ui->verticalLayout->addWidget(treeWidget);

    QObject::connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(on_treeWidget_itemDoubleClicked(QTreeWidgetItem*,  int)));




    //Converter* converter = new Converter();

    QDockWidget * dockWidget = new QDockWidget(tr("MoOnRails"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
    dockWidget->setFloating(false);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    QWidget* multiWidget = new QWidget();
    QVBoxLayout *mainVerticalLayout = new QVBoxLayout;

    QLabel *title = new QLabel("MoOnRails");

    mainVerticalLayout->addWidget(title);

    multiWidget->setLayout(mainVerticalLayout);
    dockWidget->setWidget(multiWidget);

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

void MainWindow::on_pushButton_connect_clicked()
{
    qDebug("function: on_pushButton_connect_clicked\n");



}

//! launch of a service window window
// ########################################################
void MainWindow::addService(std::__cxx11::string serviceName){
  QDockWidget * qDockWidget_service = new QDockWidget(serviceName.c_str(), this);
  qDockWidget_service->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
  qDockWidget_service->setFloating(true);
  qDockWidget_service->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  ServiceDisplay* myNewWindow = new ServiceDisplay(qDockWidget_service, this);
  myServiceDisplays.push_back(myNewWindow);

  this->addDockWidget(Qt::BottomDockWidgetArea, qDockWidget_service);
  }
