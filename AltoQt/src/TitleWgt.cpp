#include "TitleWgt.hpp"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabBar>
#include <QToolButton>
#include <QWidget>

//--------------------------------------------------------------------------------
TitleWgt::TitleWgt(QString title, QWidget* parent)
    : QFrame(parent)
{
    QHBoxLayout*    _layout = new QHBoxLayout(this);
    QLabel*         _titleLbl = new QLabel(title, this);
    QTabBar*        _tabBar = new QTabBar(this);
    QToolButton*    _closeBtn = new QToolButton(this);

    _titleLbl->setObjectName("titleLbl");

    _tabBar->addTab("Tab1");
    _tabBar->addTab("Tab2");
    _tabBar->addTab("Tab3");

    _closeBtn->setObjectName("closeBtn");
    _closeBtn->setText("X");
    _closeBtn->setAutoRaise(true);
    _closeBtn->setToolTip("close");
    connect(_closeBtn, &QToolButton::released, qApp, [this](){emit sigAction(0);});

    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);
    _layout->addWidget(_titleLbl);
    _layout->addStretch(1);
    _layout->addWidget(_tabBar, 1);
    _layout->addStretch(1);
    _layout->addWidget(_closeBtn);
}