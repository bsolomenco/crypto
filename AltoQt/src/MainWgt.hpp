#pragma once
#include "ProgressWgt.hpp"
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class MainWgt
    : public QWidget
{
    Q_OBJECT
private:
    QLineEdit   _srcLed;
    QPushButton _brouseSrcBtn;

    QLineEdit   _dstLed;
    QPushButton _brouseDstBtn;

    QPushButton _encBtn;
    QPushButton _decBtn;

    //ProgressWgt _progressWgt;
public:
    MainWgt(QWidget*parent=nullptr);

private slots:
    void selectFile();

signals:
    void sigX(QString src, QString dst, int op);
};