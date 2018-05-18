#include "MainWnd.hpp"
#include "Q_App.hpp"
#include "Q_RoundCornersMainWnd.hpp"
#include "TitleWgt.hpp"
#include <QDockWidget>
#include <QFileSystemWatcher>
#include <QLabel>

char stylesheet[] = {
    "QMainWindow{"
    //"background                 : transparent;"
    "background                 : #FFFF00;"
    "color                      : #C0C0C0;"

    //"border                     : 10px solid red;"
    "border-top-left-radius     : 0px;"
    "border-top-right-radius    : 0px;"
    "border-bottom-right-radius : 10px;"
    "border-bottom-left-radius  : 10px;"
    "}"

    //dock windows
    "_Q--MoveResizeFrm{"
    "background                 : transparent;"
    "border                     : 1px solid #4040FF;"
    "border-width               : 0px 6px 6px 6px;"
    "border-top-left-radius     : 0px;"
    "border-top-right-radius    : 0px;"
    "border-bottom-right-radius : 8px;"
    "border-bottom-left-radius  : 8px;"
    "}"
    "Dock--TitleWgt{"
    "background                 : #4040FF;"
    "border                     : 0px solid red;"
    "border-top-left-radius     : 8px;"
    "border-top-right-radius    : 8px;"
    "border-bottom-right-radius : 0px;"
    "border-bottom-left-radius  : 0px;"
    "color                      : #FF0000;"//it is not inherited by the label inside title!?
    "}"
    //main widget inside dock windows
    "Dock--MainWgt{"
    "background                 : transparent;"
    //"background                 : #000000;"
    "border                     : 1px solid #8080FF;"
    "border-width               : 0px 6px 6px 6px;"
    "border-top-left-radius     : 0px;"
    "border-top-right-radius    : 0px;"
    "border-bottom-right-radius : 8px;"
    "border-bottom-left-radius  : 8px;"
    "color                      : #C0C0C0;"
    "}"
    "Dock--MainWgt QLabel{"
    "background                 : qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #000000, stop:0.5 #202020, stop: 1 #000000);"
    "color                      : #FF0000;"
    "min-height                 : 100px;"
    "}"
};


//--------------------------------------------------------------------------------
int main(int argc, char**argv){
    Q::App app(argc, argv);
    app.loadStylesheet("res\\AltoQt.qss", true);

    MainWnd mw;
    mw.resize(800, 400);
    mw.show();

    return app.exec();
}