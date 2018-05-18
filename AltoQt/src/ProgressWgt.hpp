#pragma once
#include <QFrame>
#include <QLabel>
#include <QString>
class QWidget;

//================================================================================
class ProgressWgt: public QFrame{
    Q_OBJECT
private:
    struct X{
        QLabel* percentLbl;
        QLabel* filenameLbl;
    };
    size_t  _len = 0;
    X       _arr[10];
public:
    ProgressWgt(QWidget* parent=nullptr);

    void setCount(size_t count);
    void operator()(size_t index, QString text);
    void operator()(size_t index, size_t size, size_t len, double percent);
    void enable(size_t index, bool b=true);
    void enable(bool b=true);

};