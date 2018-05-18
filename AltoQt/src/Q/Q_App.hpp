#pragma once
#include <QApplication>
#include <QString>

namespace Q{
    class App
        : public QApplication
    {
        Q_OBJECT
    private:
    public:
        App(int argc, char**argv) : QApplication(argc, argv) {}

        int loadStylesheet(const char*stylesheetFilename, bool watchAndReloadWhenChanged=false);
    };
}