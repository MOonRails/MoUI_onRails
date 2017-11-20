#include "publiship.h"
#include "networkinterface.h"

//! constructor
// ###################################################################
PublishIP::PublishIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay)
{
    //qDebug()<< "addDataReception\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";
    networkInterface = myNetworkInterface;


    name = myName;
    comment = myComment;
    myNumber = myNumber;
    mySupportInReplay = mySupportInReplay;
    myName += ": ";
    QLabel* myTitle = new QLabel(myName.c_str());
    data = new QLabel("#VALUE");
    chart = new QChart();


    QHBoxLayout *layout_horizontal = new QHBoxLayout;
    QVBoxLayout *layout_vertical = new QVBoxLayout;
    QPushButton *pushButtonOpenClose = new QPushButton;
    pushButtonOpenClose->setMaximumWidth(50);
    QObject::connect(pushButtonOpenClose, SIGNAL(clicked()), this , SLOT(openCloseGraph())  );

    // initialise graph
    //myReceptionChart.push_back(chart);
    chart->legend()->show();
    chart->createDefaultAxes();
    chart->setTitle(myName.c_str());




    series = new QLineSeries();
    series->setName(myName.c_str());
    chart->addSeries(series);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);





    layout_horizontal->addWidget(myTitle);
    layout_horizontal->addWidget(data);
    layout_horizontal->addWidget(pushButtonOpenClose);
    layout_vertical->addLayout(layout_horizontal);
    layout_vertical->addWidget(chartView);

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
    chart->removeSeries(series);
    series->append(QPointF(chartCounter,myValue));
    data->setText(QString::number(myValue));

    //chart->axisX()->setRange(0,series->count());
    //chart->axisY()->setRange(*minp,*maxp);
    //chart->update();
    chart->addSeries(series);

    chartCounter++;
}


//! set new value
// ###################################################################
void PublishIP::openCloseGraph()
{
    if(open == true){
        chart->setMaximumHeight(0);

        //chart->setFixedHeight(0);
        chart->setMaximumWidth(0);
        open = false;
    } else {
        chart ->setMaximumHeight(16777215);
        chart ->setMaximumWidth(16777215);
        open = true;
    }
    chart->setVisible(open);
}

