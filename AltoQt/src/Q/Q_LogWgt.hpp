#pragma once
#include <QTextEdit>
class QWidget;

namespace Q{
    class LogWgt: public QTextEdit{
        Q_OBJECT
    public:
        LogWgt(QWidget*parent=nullptr);

        void operator()(const char* format, ...);
    };
}