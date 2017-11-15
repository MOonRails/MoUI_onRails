#ifndef SERVICEDISPLAY_H
#define SERVICEDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>

class ServiceDisplay : public QWidget
{
    Q_OBJECT
public:
    ServiceDisplay();
    ServiceDisplay(QDockWidget *dockWidget, QObject *myWindow);
    ~ServiceDisplay();
    void addDataSend();
    void addDataReception();

private:
    QObject *mainWindow; // contains the QObject from the main window
};

#endif // SERVICEDISPLAY_H
