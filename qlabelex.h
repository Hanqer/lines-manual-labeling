#ifndef QLABELEX_H
#define QLABELEX_H

#include <QLabel>

class QLabelEx : public QLabel
{
    Q_OBJECT
public:
    explicit QLabelEx(QWidget *parent = nullptr);
    void setOffset();
    int xoffset, yoffset;
    QPoint pointPos;

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
signals:
    void clicked(QMouseEvent *ev);
    void pressed(QMouseEvent *ev);
    void doubled(QMouseEvent *ev);
    void moved(QMouseEvent *ev);

public slots:
};

#endif // QLABELEX_H
