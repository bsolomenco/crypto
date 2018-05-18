#include "Q_MoveResizeFrm.hpp"
#include <QMouseEvent>

namespace{
    //--------------------------------------------------
    Qt::CursorShape cursorShape[3][3] = {
        {Qt::SizeFDiagCursor, Qt::SizeVerCursor, Qt::SizeBDiagCursor},
        {Qt::SizeHorCursor  , Qt::SizeAllCursor, Qt::SizeHorCursor  },
        {Qt::SizeBDiagCursor, Qt::SizeVerCursor, Qt::SizeFDiagCursor},
    };

    //--------------------------------------------------
    enum class CardinalDir{N, E, S, W, C, NE, SE, SW, NW};
    CardinalDir resizeDir[3][3] = {//resize directions
        {CardinalDir::NW, CardinalDir::N, CardinalDir::NE},
        {CardinalDir::W , CardinalDir::C, CardinalDir::E },
        {CardinalDir::SW, CardinalDir::S, CardinalDir::SE},
    };
}

//==================================================
Q::MoveResizeFrm::MoveResizeFrm(int margins[4], QWidget* parent)
    : QFrame(parent)
    , _movePos()
    , _reszPos()
    , _margins{margins[0], margins[1], margins[2], margins[3]}
    , _row(0)
    , _col(0)
{
    setMouseTracking(true);
}

//--------------------------------------------------------------------------------
void Q::MoveResizeFrm::mouseMoveEvent(QMouseEvent* event){
    //qDebug("%s(x=%d y=%d)", __FUNCTION__, event->x(), event->y());
    int x(event->x());
    int y(event->y());
    int col(x<_margins[3] ? 0 : (x<width()-_margins[1] ? 1 : 2));
    int row(y<_margins[0] ? 0 : (y<height()-_margins[2] ? 1 : 2));
    setCursor(cursorShape[row][col]);
    if (event->buttons() & Qt::LeftButton){
        //QWidget& pw = *parentWidget();
        QWidget& pw = *(parentWidget() ? parentWidget() : this);
        switch(resizeDir[_row][_col]){
            case CardinalDir::C:
                pw.move(event->globalPos() - _movePos);
                break;
            case CardinalDir::N:
                pw.resize(pw.width(), pw.height()-(event->globalPos()-_reszPos).y());
                pw.move(pw.x(), (event->globalPos() - _movePos).y());
                break;
            case CardinalDir::E:
                pw.resize(pw.width()+(event->globalPos()-_reszPos).x(), pw.height());
                break;
            case CardinalDir::S:
                pw.resize(pw.width(), pw.height()+(event->globalPos()-_reszPos).y());
                break;
            case CardinalDir::W:
                pw.resize(pw.width()-(event->globalPos()-_reszPos).x(), pw.height());
                pw.move((event->globalPos() - _movePos).x(), pw.y());
                break;
            case CardinalDir::NE:
                pw.resize(pw.width()+(event->globalPos()-_reszPos).x(), pw.height()-(event->globalPos()-_reszPos).y());
                pw.move(pw.x(), (event->globalPos() - _movePos).y());
                break;
            case CardinalDir::SE:
                pw.resize(pw.width()+(event->globalPos()-_reszPos).x(), pw.height()+(event->globalPos()-_reszPos).y());
                break;
            case CardinalDir::SW:
                pw.resize(pw.width()-(event->globalPos()-_reszPos).x(), pw.height()+(event->globalPos()-_reszPos).y());
                pw.move((event->globalPos() - _movePos).x(), pw.y());
                break;
            case CardinalDir::NW:
                pw.resize(pw.width()-(event->globalPos()-_reszPos).x(), pw.height()-(event->globalPos()-_reszPos).y());
                pw.move(event->globalPos() - _movePos);
                break;
        }
        _reszPos = event->globalPos();
        _movePos = event->globalPos() - pw.frameGeometry().topLeft();
    }
    //event->accept();
}

//--------------------------------------------------------------------------------
void Q::MoveResizeFrm::mousePressEvent(QMouseEvent* event){
    //qDebug("%s(x=%d y=%d)", __FUNCTION__, event->x(), event->y());
    if(event->button() == Qt::LeftButton){
        int x(event->x());
        int y(event->y());
        _col = (x<_margins[int(CardinalDir::W)] ? 0 : (x< width()-_margins[int(CardinalDir::E)] ? 1 : 2));
        _row = (y<_margins[int(CardinalDir::N)] ? 0 : (y<height()-_margins[int(CardinalDir::S)] ? 1 : 2));
        setCursor(cursorShape[_row][_col]);
      
        _reszPos = event->globalPos();
        QWidget& pw = *(parentWidget() ? parentWidget() : this);
        _movePos = event->globalPos() - pw.frameGeometry().topLeft();
        //event->accept();
    }
}