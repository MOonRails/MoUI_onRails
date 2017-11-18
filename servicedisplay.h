#ifndef SERVICEDISPLAY_H
#define SERVICEDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>
#include <QtCharts>

class ServiceDisplay : public QWidget
{
    Q_OBJECT
public:
    ServiceDisplay();
    ServiceDisplay(QDockWidget *dockWidget, QObject *myWindow);
    ~ServiceDisplay();
    void addDataSend(std::string title);
    void addDataReception(std::string title);

private:
    QObject *mainWindow; // contains the QObject from the main window
    QVBoxLayout *layout_vertical_Send;
    QVBoxLayout *layout_vertical_Reception;

    std::vector<QPushButton*> mySendButtons;

    std::vector<QLabel*> myReceptionLabel;

    std::vector<QChart*> myReceptionChart;
};

#endif // SERVICEDISPLAY_H
