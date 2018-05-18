#include "MainWnd.hpp"
#include "Base64.hpp"
#include "Common.hpp"
#include "Win_Thr.h"
#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QThread>
#include <windows.h>

//--------------------------------------------------------------------------------
MainWnd::MainWnd()
    : RoundCornersMainWnd("AltoQt", Qt::FramelessWindowHint)
    , _titleWgt("AltoQt", this)
    , _logWgt()
    , _mainWgt()
    , _progressWgt(this)
    , _thr(this)
{
    Common::mainWnd = this;
    Common::mainThrId = QThread::currentThreadId();

    qDebug("TRC %s() winId=%p", __FUNCTION__, winId());
    setAttribute(Qt::WA_NativeWindow, true);//force native window to process native events

    auto _logDockWgt = new QDockWidget("Log");
    //_logDockWgt->setTitleBarWidget(new QLabel("Log"));
    _logDockWgt->setWidget(&_logWgt);

    setTitleWidget(&_titleWgt);
    //setCentralWidget(new QLabel("main widget"));
    setCentralWidget(&_mainWgt);

    auto _progressDockWgt = new QDockWidget("Progress");
    //_logDockWgt->setTitleBarWidget(new QLabel("Log"));
    _progressDockWgt->setWidget(&_progressWgt);
    _progressWgt.setCount(QThread::idealThreadCount());
    _progressWgt.enable(false);


    addDockWidget(Qt::BottomDockWidgetArea, _progressDockWgt);
    addDockWidget(Qt::BottomDockWidgetArea, _logDockWgt);

    _titleWgt.setObjectName("titleWgt");

    connect(&_titleWgt, &TitleWgt::sigAction, this, &MainWnd::onAction);
    connect(&_thr, &QThread::started , this, [this](){qDebug("thread started"); _logWgt("thread %d started", _thr.threadId());});
    connect(&_thr, &QThread::finished, this, [this](){_logWgt("thread %d finished", _thr.threadId());});
    connect(&_mainWgt, &MainWgt::sigX, this, &MainWnd::slotX);
}

//--------------------------------------------------------------------------------
void MainWnd::onAction(int id){
    _logWgt("%s(id=%d)", __FUNCTION__, id);
    switch(id){
        case 0://quit
            _logWgt("DBG closing...");
            _threadManager.stop(5000);
            QApplication::closeAllWindows();
            break;
        case 1:{//test
            if(_thr.isRunning()){
                int err = !_thr.postMsg(WM_DESTROY, 0, 0);
                if(err){
                    _logWgt("ERR .postMsg(WM_DESTROY)");
                }
            }else{
                _thr.start();
            }
            break;
        }
        default:
            _logWgt("ERR unknown action ID: %d", id);
    }
}

//--------------------------------------------------------------------------------
void MainWnd::slotX(QString src, QString dst, int op){
    static const char* opStr[] = {"DECRYPT", "ENCRYPT"};
    _logWgt("%s src=\"%s\" dst=\"%s\"", opStr[op], src.toUtf8().constData(), dst.toUtf8().constData());
    _progressWgt.enable(true);

    _t0 = std::chrono::system_clock::now();

    _srcPath    = src.toUtf8().constData();
    _dstDirPath = dst.toUtf8().constData();
    _relPath = "";
    _dirIter    = fs::recursive_directory_iterator(src.toUtf8().constData());
    _dirDepth   = 0;
    _b64Func = (op ? encodeStr : decodeStr);
#if 1
    _threadManager.start(QThread::idealThreadCount(), op);
#else
    _threadManager.start(1, op);
#endif
}

//--------------------------------------------------
bool MainWnd::event(QEvent* evt){
    switch(evt->type()){
        case QEvent::WinIdChange:{
            Common::mainWndId = (void*)winId();
#if 0//debug
            qDebug("QEvent::WinIdChange: %p", Common::mainWndId);
            int ok = PostMessage((HWND)Common::mainWndId, WM_USER, 0, 0);
            if(!ok){
                qDebug("ERR PostMessage()");
            }
#endif
            break;
        }
    }
    return RoundCornersMainWnd::event(evt);
}

//--------------------------------------------------------------------------------
bool MainWnd::nativeEvent(const QByteArray& /*eventType*/, void* message, long* /*result*/){
    MSG* msg = (MSG*)message;
    WorkInf& inf = *(WorkInf*)(msg->wParam);
    switch(msg->message){
        case Common::THR_STARTED:{
            _logWgt("DBG thread started: %08X(%d)", inf.tid, inf.tix);
        }
        case Common::THR_READY:{//a worker thread is available; lParam=tid, wParam=Inf*
            //qDebug("WM_USER wPar=%X lPar=%X [%s()]", msg->wParam, msg->lParam, __FUNCTION__);
            if(_dirIter == _dirIterEnd){
                auto t1 = std::chrono::system_clock::now();
                size_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(t1-_t0).count();
                int hrs = milliseconds/(1000*60*60)%24;
                int min = milliseconds/(1000*60)%60;
                int sec = (milliseconds-min*1000*60)/1000%100;
                _logWgt("TIM %02d:%02d:%02d.%zu", hrs, min, sec, milliseconds);
                //_threadManager.stop(3000);
                _threadManager.quit(inf.tix);
                _logWgt("DONE [%s()]", __FUNCTION__);
                return true;
            }

            //qDebug("%s", _dirIter->path().filename().string().data());
#if 1
            std::string dstFilename = _dirIter->path().filename().string();
            dstFilename = (*_b64Func)(dstFilename.data(), inf.cipher, inf.buf, sizeof(inf.buf));
#else
            std::string dstFilename = (_dirIter->path().filename().string().data());
#endif
            //for(; _dirDepth>_dirIter.depth(); --_dirDepth){//back from a dir(s)
            for(; _dirDepth>_dirIter.depth(); --_dirDepth){//back from a dir(s)
                _relPath = _relPath.parent_path();
            }
            _dirDepth = _dirIter.depth();
            if(fs::is_directory(_dirIter->path())){
                //qDebug("    DIR  %s", _dirIter->path().filename().string().data());
                //_logWgt("%08X DIR  %s", msg->wParam, _dirIter->path().filename().string().data());
                ++_dirDepth;
                _relPath /= dstFilename;
                inf.dstPath = _dstDirPath / _relPath;
                fs::create_directories(inf.dstPath);
                _threadManager.ping(inf.tix);
            }else{
                //qDebug("    FILE %s", _dirIter->path().filename().string().data());
                inf.set(_dirIter->path(), _dstDirPath/_relPath/dstFilename, fs::file_size(_dirIter->path()));
                _progressWgt(inf.tix, inf.srcPath.string().data());
                _threadManager.encrypt(inf.tix);
            }
            ++_dirIter;
            return true;
        }
        case Common::THR_STOPPED:
            _progressWgt.enable(inf.tix, false);
            _logWgt("DBG thread stopped: %08X(%zu)", inf.tid, inf.tix);
            break;
        case Common::THR_PROGRESS:
            //qDebug("THR_PROGRESS tix=%d %zu / %zu %lf", inf.tix, inf.len, inf.fileSize, inf.percent);
            _progressWgt(inf.tix, inf.fileSize, inf.len, inf.percent);
            break;
    }
    return false;//let Qt process it further
}