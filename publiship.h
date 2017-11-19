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
    PublishIP(QVBoxLayout *layout_base, std::__cxx11::string title);
    ~PublishIP();
    void setValue(int myValue);
private slots:
    void openCloseGraph();

private:
    QLabel* data;
    QChart *chart;
    QLineSeries *series;
    QChartView * chartView;
    int chartCounter = 0;
    bool open = true;
};

#endif // PUBLISHIP_H
