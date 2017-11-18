#include "servicedisplay.h"

#include <QtWidgets>
#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QtCharts>


//! constructor
// ###################################################################
ServiceDisplay::ServiceDisplay()
{

}


//! constructor
// ###################################################################
ServiceDisplay::ServiceDisplay(QDockWidget *dockWidget, QObject *myWindow)
{
    QWidget* multiWidget = new QWidget();
    QHBoxLayout *layout_horizontal = new QHBoxLayout;
    layout_vertical_Send = new QVBoxLayout;
    layout_vertical_Reception = new QVBoxLayout;

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);


    //layout_vertical->addWidget(myDisplayTable);
    layout_horizontal->addLayout(layout_vertical_Reception);
    layout_horizontal->addWidget(line);
    layout_horizontal->addLayout(layout_vertical_Send);
    multiWidget->setLayout(layout_horizontal);

    dockWidget->setWidget(multiWidget);
}


//! destructor
// ###################################################################
ServiceDisplay::~ServiceDisplay()
{
}


//! add a data sending window
// ###################################################################
void ServiceDisplay::addDataSend(std::string title)
{
    qDebug()<< "addDataSend\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";
    //QVBoxLayout *layout_addDataReception;
    QLabel* myTitle = new QLabel(title.c_str());
    QLineEdit* myLinedit = new QLineEdit();
    QPushButton* myPushButtonSend = new QPushButton("Send");
    QHBoxLayout *layout_horizontalSendData = new QHBoxLayout;
    layout_horizontalSendData->addWidget(myLinedit);
    layout_horizontalSendData->addWidget(myPushButtonSend);
    layout_vertical_Send->addWidget(myTitle);
    layout_vertical_Send->addLayout(layout_horizontalSendData);
}

//! add a data display window
// ###################################################################
void ServiceDisplay::addDataReception(std::string title)
{
    qDebug()<< "addDataReception\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";
    //QVBoxLayout *layout_addDataReception;
    QLabel* myTitle = new QLabel(title.c_str());
    //QToolBox* myToolBox = new QToolBox();

    QLabel* data = new QLabel("data");
    //myReceptionLabel.push_back(data);
    /*myToolBox->insertItem(0,data, "test");

    myToolBox->insertItem(0,data, "test");
    myToolBox->insertItem(0,data, "test");

    myToolBox->insertItem(0,data, "test");*/


    // initialise graph
    QChart *chart = new QChart();
    myReceptionChart.push_back(chart);
    chart->legend()->show();
    chart->createDefaultAxes();
    chart->setTitle(title.c_str());

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    QLineSeries *series = new QLineSeries();
    series->setName(title.c_str());
    // iterate over current row
    for (int i = 0; i < 10; ++i) {
        series->append(i,i );
    }

    chart->addSeries(series);

    layout_vertical_Reception->addWidget(myTitle);
    layout_vertical_Reception->addWidget(data);
    layout_vertical_Reception->addWidget(chartView);
    //layout_vertical_Reception->addWidget(myToolBox);
}

