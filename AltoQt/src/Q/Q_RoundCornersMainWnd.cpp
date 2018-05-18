#include "Q_RoundCornersMainWnd.hpp"
#include <QDockWidget>

namespace{
    int margins[] = {2, 40, 40, 40};
}

//--------------------------------------------------------------------------------
Q::RoundCornersMainWnd::RoundCornersMainWnd(QString title, Qt::WindowFlags flg)
    : Q::TranslucentWgt(title, nullptr, flg, true)
    , _borderLay(this)
    , _borderFrm(margins, this)
    , _contentLay(&_borderFrm)
    , _mainWgt(nullptr)
{
    _borderLay.setContentsMargins(0, 0, 0, 0);
    _borderLay.setSpacing(0);
    _borderLay.addWidget(&_borderFrm);

    _borderFrm.setObjectName("Q_RoundCornersMainWnd_borderFrm");

    _contentLay.setContentsMargins(0, 0, 0, 0);
    _contentLay.setSpacing(0);
    _contentLay.insertWidget(1, &_mainWgt);//index=0 is reserved for title widget

    _mainWgt.setObjectName("mainWgt");
}

//--------------------------------------------------------------------------------
void Q::RoundCornersMainWnd::setTitleWidget(QWidget* titleWidget){
    //titleWidget->setParent(&_borderFrm);
    //titleWidget->show();
    _contentLay.insertWidget(0, titleWidget);
}

//--------------------------------------------------------------------------------
void Q::RoundCornersMainWnd::setCentralWidget(QWidget* centralWidget){
    centralWidget->setParent(&_mainWgt);
    _mainWgt.setCentralWidget(centralWidget);
}

//--------------------------------------------------------------------------------
void Q::RoundCornersMainWnd::addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockwidget, Qt::Orientation orientation){
    _mainWgt.addDockWidget(area, dockwidget, orientation);
}