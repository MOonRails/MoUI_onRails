#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "servicedisplay.h"

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

private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    //void on_treeWidget_itemDoubleClicked(int column);
    void on_pushButton_file_clicked();
    void on_pushButton_connect_clicked();
};

#endif // MAINWINDOW_H
