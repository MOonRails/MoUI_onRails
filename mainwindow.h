#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTcpSocket>

#include <QFileSystemWatcher>
#include <QDataStream>
#include <QMessageBox>
#include <QDateTime>

#include "service_display.h"
#include "networkinterface.h"
#include <vector>

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
    //QDockWidget *startDockWidget;
    void loadFile(std::string filename);
    void addService(std::string serviceName);


    std::vector<Service_Display*>myServiceDisplays;

    QPushButton* pushButton_file;


    QDateTime lastRead;


    int leftoverIncomingData_column = -1;
    std::string leftoverIncomingData = "";

    QFileSystemWatcher* watcher;



    NetworkInterface* networkInterface;

    bool findDataType(std::string filename, std::string dataType,std::vector<std::string> *myTypeList,std::vector<std::string> *myNamesList);

private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_file_clicked();

    void on_fileChanged(QString myFile);
};

#endif // MAINWINDOW_H
