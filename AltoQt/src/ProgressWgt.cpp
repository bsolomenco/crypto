#include "ProgressWgt.hpp"
#include <QGridLayout>
#include <QLineEdit>
#include <QProgressBar>
#include <QToolButton>
#include <QWidget>

//--------------------------------------------------------------------------------
ProgressWgt::ProgressWgt(QWidget* parent)
    : QFrame(parent)
{
    QGridLayout* gl = new QGridLayout(this);
    for(size_t i=0; i<sizeof(_arr)/sizeof(_arr[0]); ++i){
        _arr[i].percentLbl = new QLabel(QString::asprintf("%6.02lf%%", 0.0), this);
        gl->addWidget(_arr[i].percentLbl, (int)i, 0);
        _arr[i].filenameLbl = new QLabel(QString::asprintf("%zu", i), this);
        gl->addWidget(_arr[i].filenameLbl, (int)i, 1);
    }
    gl->setColumnStretch(1, 1);
#if 0
    {//test
        QToolButton* btn = new QToolButton(this);
        btn->setText("test button");
        gl->addWidget(btn, 10, 0);

        QLineEdit* led = new QLineEdit(this);
        led->setText("test line edit");
        gl->addWidget(led, 11, 0);
    }
#endif
}

//--------------------------------------------------------------------------------
void ProgressWgt::setCount(size_t count){
    _len = count;
    for(size_t i=0; i<_len; ++i){
        _arr[i].percentLbl->setText("00.00%");
        _arr[i].filenameLbl->setText("");
    }
    for(size_t i=_len; i<sizeof(_arr)/sizeof(_arr[0]); ++i){
        _arr[i].percentLbl->hide();
        _arr[i].filenameLbl->hide();
    }
}

//--------------------------------------------------------------------------------
void ProgressWgt::operator()(size_t index, QString text){
    _arr[index].percentLbl->setText("00.00%");
    _arr[index].filenameLbl->setText(text);
}

//--------------------------------------------------------------------------------
void ProgressWgt::operator()(size_t index, size_t size, size_t len, double percent){
    _arr[index].percentLbl->setText(QString::asprintf("%06.02lf%% %zu / %zu", percent, len, size));
}

//--------------------------------------------------------------------------------
void ProgressWgt::enable(size_t index, bool b){
    _arr[index].percentLbl->setEnabled(b);
    _arr[index].filenameLbl->setEnabled(b);
}

//--------------------------------------------------------------------------------
void ProgressWgt::enable(bool b){
    for(size_t i=sizeof(_arr)/sizeof(_arr[0]); i-->0; ){
        _arr[i].percentLbl->setEnabled(b);
        _arr[i].filenameLbl->setEnabled(b);
    }
}

