QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AltoQt
TEMPLATE = vcapp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=                          \
        src                             \
        src/Botan64                     \
        src/Q                           \
        src/S                           \

HEADERS +=                              \
    src/Botan64/botan_all.h             \
    src/Botan64/botan_all_internal.h    \
    src/Q/Q_App.hpp                     \   
    src/Q/Q_LogWgt.hpp                  \   
    src/Q/Q_MoveResizeFrm.hpp           \
    src/Q/Q_RoundCornersMainWnd.hpp     \
    src/Q/Q_TranslucentWgt.hpp          \
    src/S/Win_Thr.h                     \
    src/Base64.hpp                      \
    src/Cipher.hpp                      \
    src/Common.hpp                      \
    src/File.hpp                        \
    src/FileSystemThr.hpp               \
    src/MainWgt.hpp                     \
    src/MainWnd.hpp                     \
    src/ProgressWgt.hpp                 \
    src/TitleWgt.hpp                    \
    src/ThreadManager.hpp               \

SOURCES +=                              \
    src/Botan64/botan_all.cpp           \
    src/Q/Q_App.cpp                     \
    src/Q/Q_LogWgt.cpp                  \
    src/Q/Q_MoveResizeFrm.cpp           \
    src/Q/Q_RoundCornersMainWnd.cpp     \
    src/Q/Q_TranslucentWgt.cpp          \
    src/S/Win_Thr.cpp                   \
    src/Base64.cpp                      \
    src/Cipher.cpp                      \
    src/Common.cpp                      \
    src/FileSystemThr.cpp               \
    src/main.cpp                        \
    src/MainWgt.cpp                     \
    src/MainWnd.cpp                     \
    src/ProgressWgt.cpp                 \
    src/TitleWgt.cpp                    \
    src/ThreadManager.cpp               \
