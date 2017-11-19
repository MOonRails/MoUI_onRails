#ifndef SENDIP_H
#define SENDIP_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtWidgets>
#include <QMainWindow>
#include <QtCharts>

class SendIP : public QWidget
{
    Q_OBJECT
public:
    SendIP(QVBoxLayout *layout_base, std::__cxx11::string title);
    ~SendIP();

    QLineEdit* myLinedit;
    QPushButton* myPushButtonSend;

private slots:
    void sendData();
};

#endif // SENDIP_H
