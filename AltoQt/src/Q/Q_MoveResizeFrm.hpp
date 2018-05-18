#pragma once
#include <QFrame>
#include <QPoint>
class QMouseEvent;

namespace Q{
    class MoveResizeFrm: public QFrame{
        Q_OBJECT
    public:
        MoveResizeFrm(int margins[4], QWidget* parent=0);

    protected:
        QPoint  _movePos;
        QPoint  _reszPos;
        int     _margins[4];  //NORTH, EAST, SOUTH, WEST
        int     _row;         //
        int     _col;         //

        void mouseMoveEvent (QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
    };
}