#include "Q_LogWgt.hpp"
#include <stdarg.h> 
#include <ctime>

//--------------------------------------------------------------------------------
Q::LogWgt::LogWgt(QWidget*parent)
    :QTextEdit(parent)
{
    //setMinimumHeight(23);
    setReadOnly(true);
    setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
    setFont(QFont("Courier"));
}

//--------------------------------------------------------------------------------
void Q::LogWgt::operator()(const char* format, ...){
    static char buf[64] = {0};
    static QString s;
    std::time_t t = std::time(nullptr);
    //s += QString::asprintf("%T ", std::localtime(&t));
    std::strftime(buf, sizeof(buf), "%T ", std::localtime(&t));
    s = buf;
    va_list args;
    va_start(args, format);
    s += QString::vasprintf(format, args);
    append(s);
    va_end(args);
}