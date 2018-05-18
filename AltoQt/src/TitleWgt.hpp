#pragma once
#include <QFrame>
#include <QString>
class QWidget;

//================================================================================
class TitleWgt: public QFrame{
    Q_OBJECT
public:
    TitleWgt(QString title, QWidget* parent=nullptr);

signals:
    void sigAction(int i=0);
};