#pragma once
#include "ProgressWgt.hpp"
#include "Q_RoundCornersMainWnd.hpp"
#include "Q_LogWgt.hpp"
#include "FileSystemThr.hpp"
#include "MainWgt.hpp"
#include "TitleWgt.hpp"
#include "ThreadManager.hpp"
#include <QMainWindow>
#include <chrono>
class Worker;
class QThread;
namespace fs = std::experimental::filesystem;

class MainWnd
    : public Q::RoundCornersMainWnd
{
    Q_OBJECT
private:
    TitleWgt        _titleWgt;
    Q::LogWgt       _logWgt;
    MainWgt         _mainWgt;
    ProgressWgt     _progressWgt;
    FileSystemThr   _thr;


    struct Work{
        QThread t;
        Worker* w;

        Work(Worker* w=nullptr)
            :w(w)
        {};
    };

    size_t  _workLen = 0;
    size_t  _workDim = 0;
    Work    _workArr[10] = {};


    fs::recursive_directory_iterator    _dirIter;
    fs::recursive_directory_iterator    _dirIterEnd;
    int                                 _dirDepth = 0;
    fs::path                            _relPath;
    fs::path                            _srcPath;
    fs::path                            _dstDirPath;


    ThreadManager _threadManager;
    const char* (*_b64Func)(const char* str, Cipher& cipher, char* buf, size_t dim);

    std::chrono::system_clock::time_point _t0;

public:
    MainWnd();
    bool event(QEvent* evt);
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);

public slots:
    void onAction(int id);
    void slotX(QString src, QString dst, int op);
};