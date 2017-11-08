#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

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

private slots:

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    //void on_treeWidget_itemDoubleClicked(int column);
};

#endif // MAINWINDOW_H
