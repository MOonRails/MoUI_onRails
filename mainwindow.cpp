#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    treeWidget = new QTreeWidget();

    QTreeWidgetItem *myItem = new QTreeWidgetItem(0);
    myItem->setText(0, tr("Cities"));

    QTreeWidgetItem *myItem2 = new QTreeWidgetItem(0);
    myItem2->setText(0, tr("Cities2"));

    QTreeWidgetItem *myItem3 = new QTreeWidgetItem(0);
    myItem3->setText(0, tr("Cities3"));

    QTreeWidgetItem *telecommand = new QTreeWidgetItem(0);
    telecommand->setText(0, tr("Telecommand"));


    QTreeWidgetItem *telemetry = new QTreeWidgetItem(0);
    telemetry->setText(0, tr("Telemetry"));


    treeWidget->addTopLevelItem(myItem);
    treeWidget->addTopLevelItem(myItem2);
    treeWidget->addTopLevelItem(myItem3);
    myItem3->addChild(telemetry);
    myItem3->addChild(telecommand);




    ui->horizontalLayout->addWidget(treeWidget);

    QObject::connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(on_treeWidget_itemDoubleClicked(QTreeWidgetItem*,  int)));




    //Converter* converter = new Converter();

    QDockWidget * dockWidget = new QDockWidget(tr("MoOnRails"), this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
    dockWidget->setFloating(false);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    QWidget* multiWidget = new QWidget();
    QVBoxLayout *mainVerticalLayout = new QVBoxLayout;

    QLabel *title = new QLabel("MoOnRails");

    mainVerticalLayout->addWidget(title);

    multiWidget->setLayout(mainVerticalLayout);
    dockWidget->setWidget(multiWidget);

}






MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "clicked: " << column << " name: " << item->text(column);

    QString myTitle = item->text(column);

    QDockWidget * dockWidget = new QDockWidget(myTitle, this);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
    dockWidget->setFloating(true);
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

}
