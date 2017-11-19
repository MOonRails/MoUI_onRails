#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTcpSocket>
#include "servicedisplay.h"
#include <QDataStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTreeWidget *treeWidget;
    QDockWidget *startDockWidget;
    void loadFile(std::__cxx11::string filename);
    void addService(std::__cxx11::string serviceName);

    //std::vector<QPushButton> mySendButtons;
    std::vector<ServiceDisplay*> myServiceDisplays;

    QPushButton* pushButton_file;
    QPushButton* pushButton_connect;
    QLineEdit* lineEdit_ip;
    QLineEdit* lineEdit_port;

    QTcpSocket *clientConnection;
    QDataStream *in;
    QTcpSocket* tcpSocket;
    int leftoverIncomingData_column = -1;
    std::string leftoverIncomingData = "";

private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void inComingFromServer();
    void on_pushButton_file_clicked();
    void on_pushButton_connect_clicked();
    void disconnected();
};

#endif // MAINWINDOW_H
