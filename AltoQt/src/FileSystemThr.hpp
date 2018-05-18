#pragma once
#include <QThread>
#include <filesystem>
class QObject;
namespace fs = std::experimental::filesystem;

//================================================================================
class FileSystemThr
    : public QThread
{
    Q_OBJECT
private:
    enum Msg{
        ENCODECODE=0x400+1, 
    };
    size_t _threadId = 0;
    QString _src;
    QString _dst;
    fs::recursive_directory_iterator _dirIter;
    fs::recursive_directory_iterator _dirIterEnd;
    size_t _tidArr[10] = {0};
    size_t _tidLen = 0;
public:
    FileSystemThr(QObject*parent=nullptr);

    size_t threadId(){return _threadId;}

    int postMsg(unsigned msg, unsigned p0=0, unsigned p1=0);

    void run() override;

public slots:
    void post(QString src, QString dst, int op);

signals:
    //void sigAction(int i=0);
};