#include "publiship.h"


//! constructor
// ###################################################################
//PublishIP::PublishIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay)
PublishIP::PublishIP(QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay)

{
    //qDebug()<< "addDataReception\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";
    //networkInterface = myNetworkInterface;


    name = myName;
    comment = myComment;
    number = myNumber;
    supportInReplay = mySupportInReplay;
    myName = number + std::string(". ") + myName + std::string(": ");
    QLabel* myTitle = new QLabel(myName.c_str());
    data = new QLabel("#VALUE");
    chart = new QChart();


    QHBoxLayout *layout_horizontal = new QHBoxLayout;
    QVBoxLayout *layout_vertical = new QVBoxLayout;
    QPushButton *pushButtonOpenClose = new QPushButton;
    pushButtonOpenClose->setMaximumWidth(50);
    QObject::connect(pushButtonOpenClose, SIGNAL(clicked()), this , SLOT(openCloseGraph())  );

    QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding,QSizePolicy::Minimum);

    // initialise graph
    //myReceptionChart.push_back(chart);
    chart->legend()->show();
    chart->createDefaultAxes();
    //chart->setTitle(myName.c_str());
    chart->createDefaultAxes();





    series = new QLineSeries();
    series->setName(myName.c_str());
    chart->addSeries(series);
    chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    axisX = new QValueAxis;
    axisX->setRange(10, 20);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%.2f");
    chartView->chart()->setAxisX(axisX, series);

    contentArea = new QScrollArea();
    contentArea->setStyleSheet("QScrollArea {  border: none; }");
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentArea->setContentsMargins(0,0,0,0);
    contentArea->setMaximumHeight(0);

    contentArea->setWidget(chartView);

    layout_horizontal->addWidget(myTitle);
    layout_horizontal->addWidget(data);
    layout_horizontal->addSpacerItem(spacer);
    layout_horizontal->addWidget(pushButtonOpenClose);
    layout_vertical->addLayout(layout_horizontal);
    layout_vertical->addWidget(contentArea);

    layout_base->addLayout(layout_vertical);



}



//! destructor
// ###################################################################
PublishIP::~PublishIP()
{

}


//! set new value
//! \param input value to set
// ###################################################################
void PublishIP::setValue(int myValue)
{
    if(myValue < min){
        min = myValue;
    }
    if(myValue > max){
        max = myValue;
    }

    chart->removeSeries(series);

    if(chartCounter > 100){
        series->remove(0);
    }

    series->append(QPointF(chartCounter,myValue));
    data->setText(QString::number(myValue));

    //chart->axisX()->setRange(0,series->count());
    //chart->axisY()->setRange(*minp,*maxp);
    //chart->update();
    chart->addSeries(series);
    /*axisX->setRange(min, max);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%.2f");
    chartView->chart()->setAxisX(axisX, series);*/

    chartCounter++;
}


//! set new value
// ###################################################################
void PublishIP::openCloseGraph()
{


    QPropertyAnimation * toggleAnimation = new QPropertyAnimation(contentArea, "maximumHeight");
    toggleAnimation->setDuration(300);


    if(open == true){


        toggleAnimation->setStartValue(0);
        toggleAnimation->setEndValue(400);

        open = false;
    } else {

        toggleAnimation->setStartValue(contentArea->height());
        toggleAnimation->setEndValue(0);
        open = true;
    }
    toggleAnimation->start();
}

