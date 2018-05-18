#include "Q_App.hpp"
#include <QFile>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <stdio.h>

namespace{
    void _onStylesheetFileChange(const QString&path){
        qDebug("%s(path=\"%s\")", __FUNCTION__, path.toUtf8().constData());
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug("ERR [%s()]", __FUNCTION__);
        }
        else {
            QTextStream stream(&file);
            qApp->setStyleSheet(stream.readAll());
            file.close();
        }
    }
}


//--------------------------------------------------------------------------------
int Q::App::loadStylesheet(const char*stylesheetFilename, bool watchAndReloadWhenChanged){
    if(watchAndReloadWhenChanged){
        qDebug("watching \"%s\"", stylesheetFilename);
        static QFileSystemWatcher fsWatcher;
        fsWatcher.addPath(stylesheetFilename);
        connect(&fsWatcher, &QFileSystemWatcher::fileChanged, &_onStylesheetFileChange);
    }
    QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    _onStylesheetFileChange(stylesheetFilename);
    return 0;
}