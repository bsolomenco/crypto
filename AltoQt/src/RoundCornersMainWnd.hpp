#pragma once
#include "Q_TranslucentWgt.hpp"

//================================================================================
class RoundCornersMainWnd: public Q::TranslucentWgt{
    Q_OBJECT
public:
    RoundCornersMainWnd();

public slots:
    void onKey();
};