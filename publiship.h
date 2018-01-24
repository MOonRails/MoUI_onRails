#ifndef PUBLISHIP_H
#define PUBLISHIP_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>
#include <QtCharts>
#include <QMessageBox>


class PublishIP : public QWidget
{
    Q_OBJECT
public:
    //PublishIP(NetworkInterface* myNetworkInterface, QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay);
    PublishIP(QVBoxLayout *layout_base,std::string myName, std::string myComment, std::string myNumber, std::string mySupportInReplay);
    ~PublishIP();
    void setValue(int myValue);
    std::string number,name;
private slots:
    void openCloseGraph();

private:
    QLabel* data;
    QChart *chart;
    QLineSeries *series;
    QChartView * chartView;
    int chartCounter = 0;
    bool open = true;
    int min = 0;
    int max = 1000;
    QValueAxis *axisY;
    QValueAxis *axisX;
    //NetworkInterface* networkInterface;

    std::string comment,supportInReplay,area,list,service,field = "";

    QScrollArea* contentArea;

    //NetworkInterface* networkInterface;


};

#endif // PUBLISHIP_H
