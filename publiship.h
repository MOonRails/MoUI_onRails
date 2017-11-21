#ifndef PUBLISHIP_H
#define PUBLISHIP_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>
#include <QtCharts>


class PublishIP : public QWidget
{
    Q_OBJECT
public:
    //PublishIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay);
    PublishIP(QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay);
    ~PublishIP();
    void setValue(int myValue);
    std::string number;
private slots:
    void openCloseGraph();

private:
    QLabel* data;
    QChart *chart;
    QLineSeries *series;
    QChartView * chartView;
    int chartCounter = 0;
    bool open = true;

    //NetworkInterface* networkInterface;

    std::string name,comment,supportInReplay,area,list,service,field = "";


};

#endif // PUBLISHIP_H
