#include "qlabelex.h"
#include <QMouseEvent>
QLabelEx::QLabelEx(QWidget *parent) : QLabel(parent)
{
}

void QLabelEx::mouseReleaseEvent(QMouseEvent *ev)
{
    pointPos = ev->globalPos();
    emit clicked(ev);
}

void QLabelEx::mousePressEvent(QMouseEvent *ev)
{
    emit pressed(ev);
}

void QLabelEx::mouseDoubleClickEvent(QMouseEvent *ev)
{
    emit doubled(ev);
}

void QLabelEx::mouseMoveEvent(QMouseEvent *ev)
{
    emit moved(ev);
}

void QLabelEx::setOffset()
{
    xoffset = (contentsRect().width() - pixmap()->rect().width()) / 2;
    yoffset = (contentsRect().height() - pixmap()->rect().height()) / 2;
}
