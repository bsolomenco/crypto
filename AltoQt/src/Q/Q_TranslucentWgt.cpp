#include "Q_TranslucentWgt.hpp"

//--------------------------------------------------------------------------------
Q::TranslucentWgt::TranslucentWgt(QString title, QWidget*parent, Qt::WindowFlags flg, bool transparent)
    //: QWidget(parent)
    : QWidget(parent, flg)
{
    setWindowTitle(title);
    setAttribute(Qt::WA_TranslucentBackground, transparent);
}