#pragma once
#include "Q_MoveResizeFrm.hpp"
#include "Q_TranslucentWgt.hpp"
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
class QDockWidget;
class QWidget;

namespace Q{
    class RoundCornersMainWnd: public Q::TranslucentWgt{
        Q_OBJECT
    protected:
        QVBoxLayout         _borderLay;
        Q::MoveResizeFrm    _borderFrm;
        QVBoxLayout         _contentLay;
        QMainWindow         _mainWgt;
    public:
        RoundCornersMainWnd(QString title, Qt::WindowFlags flg=Qt::FramelessWindowHint);

        void setTitleWidget(QWidget* titleWidget);
        void setCentralWidget(QWidget* centralWidget);
        void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockwidget, Qt::Orientation orientation=Qt::Horizontal);
    };
}