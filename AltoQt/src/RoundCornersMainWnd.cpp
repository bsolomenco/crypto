#include "RoundCornersMainWnd.hpp"
#include "Q_MoveResizeFrm.hpp"
#include "TitleWgt.hpp"
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QShortcut>
#include <QVBoxLayout>

namespace{
    int margins[] = {2, 40, 40, 40};
}

//--------------------------------------------------------------------------------
RoundCornersMainWnd::RoundCornersMainWnd()
    : Q::TranslucentWgt("RoundCornersMainWnd.WindowTitle", true, nullptr)
{
    Q::MoveResizeFrm* borderFrm = new Q::MoveResizeFrm(margins, this);
    borderFrm->setObjectName("borderFrm");

    TitleWgt* titleWgt = new TitleWgt("custom title widget", borderFrm);
    titleWgt->setObjectName("titleWgt");

    QMainWindow* mainWgt = new QMainWindow();
    mainWgt->setCentralWidget(new QLabel("main widget", mainWgt));
    mainWgt->setObjectName("mainWgt");
    mainWgt->addDockWidget(Qt::LeftDockWidgetArea, new QDockWidget("dw0"));
    mainWgt->addDockWidget(Qt::RightDockWidgetArea, new QDockWidget("dw1"));

    QVBoxLayout* contentLay = new QVBoxLayout(borderFrm);
    contentLay->setContentsMargins(0, 0, 0, 0);
    contentLay->setSpacing(0);
    contentLay->addWidget(titleWgt);
    contentLay->addWidget(mainWgt);

    QVBoxLayout* borderLay = new QVBoxLayout(this);
    borderLay->setContentsMargins(0, 0, 0, 0);
    borderLay->setSpacing(0);
    borderLay->addWidget(borderFrm);

    QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")), this, nullptr, nullptr, Qt::ApplicationShortcut);
    connect(shortcut, &QShortcut::activated, this, &RoundCornersMainWnd::onKey);
}

//--------------------------------------------------------------------------------
void RoundCornersMainWnd::onKey(){
    qDebug("%s()", __func__);
}