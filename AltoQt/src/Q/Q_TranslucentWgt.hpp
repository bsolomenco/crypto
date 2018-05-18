#if 0 //Translucent Widget
    //make it transparent using window attribute
    setAttribute(Qt::WA_TranslucentBackground, true);
    //make it transparent using stylesheet... but doesn't work!?
    setStyleSheet("Q--FramelessWgt{background:transparent;}");
#endif
#pragma once 
#include <QString>
#include <QWidget>

namespace Q{
    class TranslucentWgt: public QWidget{
        Q_OBJECT
    public:
        TranslucentWgt(
            QString         title=""                    , 
            QWidget*        parent=nullptr              , 
            Qt::WindowFlags flg=Qt::FramelessWindowHint ,   //neccessary to hide the title bar
            bool            transparent=true
        );
    };
}