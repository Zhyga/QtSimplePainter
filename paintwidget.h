#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QPixmap>
#include <QPoint>
#include <QMouseEvent>
#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsDropShadowEffect>
#include <QLabel>

class PaintWidget : public QWidget
{

    Q_OBJECT

public:
    PaintWidget(QWidget* parent = nullptr);
    virtual ~PaintWidget();
    enum ShapeTypes{
        SQUARE,CIRCLE,TRIANGLE,LINE,MOVE
    };
    QPixmap getPixMap() const { return *pixMap;}
    void setSType(ShapeTypes type) { this->sType = type; }
private:
    QPixmap* pixMap;
    QPoint begin;
    QPoint destination;
    ShapeTypes sType;
    QList<QLine> lines;
    QPoint oldDest;
public slots:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void drawTriangle(QRect rect,QPainter* painter);
    void dragableLabel(QRect rect,ShapeTypes sType);
};


#endif // PAINTWIDGET_H
