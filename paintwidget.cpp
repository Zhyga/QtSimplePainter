#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent)
{
    resize(parent->size());
    pixMap = new QPixmap(size());
    pixMap->fill(Qt::white);
    setAcceptDrops(true);

}

PaintWidget::~PaintWidget()
{
    delete pixMap;
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter* painter = new QPainter(this);
    painter->drawPixmap(QPoint(),*pixMap);
    if(!begin.isNull() && !destination.isNull()){
        QRect rect = QRect(begin,destination);
        switch (sType) {
        case CIRCLE :
            painter->drawEllipse(rect.normalized());
            break;
        case SQUARE :
            painter->drawRect(rect.normalized());
            break;
        case TRIANGLE :
        {
            drawTriangle(rect,painter);
            break;
        }
        case LINE :
            painter->drawLine(begin,destination);
            break;
        default:
            break;
        }
    }
    delete painter;
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(sType == LINE){
            QLabel *child = static_cast<QLabel*>(childAt(event->position().toPoint()));
            if (!child)
                return;
            begin = child->geometry().center();
        }
        else{
        begin = event->pos();
        destination = begin;
        update();
        }
    }
    if(sType == MOVE){

        QLabel *child = static_cast<QLabel*>(childAt(event->position().toPoint()));
        if (!child)
            return;
        oldDest = child->geometry().center();
        QPixmap pixmap = child->pixmap(Qt::ReturnByValue);
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << pixmap << QPoint(event->position().toPoint() - child->pos());
    //! [1]

    //! [2]
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/x-dnditemdata", itemData);
    //! [2]

    //! [3]
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(event->position().toPoint() - child->pos());
    //! [3]

        QPixmap tempPixmap = pixmap;
        QPainter painter;
        painter.begin(&tempPixmap);
        painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
        painter.end();

        child->setPixmap(tempPixmap);
        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
            child->close();
        } else {
            child->show();
            child->setPixmap(pixmap);
        }
    }

}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->MouseMove){
        destination = event->pos();
        update();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QRect rect = QRect(begin,destination);
        QPainter* painter = new QPainter(pixMap);
        switch (sType) {
        case CIRCLE :{
            dragableLabel(rect,CIRCLE);
            break;
        }
        case SQUARE :{
            dragableLabel(rect,SQUARE);
            break;
        }
        case TRIANGLE :{
            dragableLabel(rect,TRIANGLE);
            break;
        }
        case LINE :{
            QLabel *child = static_cast<QLabel*>(childAt(event->position().toPoint()));
            if (!child)
                break;
            destination = child->geometry().center();
            lines.append(QLine(begin,destination));
            painter->drawLine(begin,destination);
            break;
        }
        default:
            break;
        }
        update();
        begin = QPoint();
        destination = QPoint();
        delete painter;
    }
}

void PaintWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void PaintWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void PaintWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        QLabel *newIcon = new QLabel(this);
        newIcon->setPixmap(pixmap);
        newIcon->move(event->position().toPoint() - offset);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
            QLabel *child = static_cast<QLabel*>(childAt(event->position().toPoint()));
            if (child){
                destination = child->geometry().center();
                QPainter painter(pixMap);
                QPen currentPen = painter.pen();
                foreach(QLine line,lines){
                    if(line.p2() == oldDest){
                        painter.setPen(Qt::white);
                        painter.drawLine(line);
                        painter.setPen(currentPen);
                        painter.drawLine(line.p1(),destination);
                        lines.append(QLine(line.p1(),destination));

                        lines.remove(lines.indexOf(line));
                    }
                    else{
                        if(line.p1() == oldDest){
                            painter.setPen(Qt::white);
                            painter.drawLine(line);
                            painter.setPen(currentPen);
                            painter.drawLine(line.p2(),destination);
                            lines.append(QLine(line.p2(),destination));

                            lines.remove(lines.indexOf(line));
                        }
                    }
                }
                update();
            }
            oldDest = destination;
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void PaintWidget::drawTriangle(QRect rect,QPainter* painter){
    QPoint firstPoint =  QPoint(rect.topLeft().x(),rect.topLeft().y());
    QPoint secondPoint =  QPoint(rect.topLeft().x() + (rect.bottomRight().x()-rect.topLeft().x())/2,rect.bottomRight().y());
    QPoint thirdPoint =  QPoint(rect.bottomRight().x(),rect.topLeft().y());
    painter->drawLine(firstPoint,secondPoint);
    painter->drawLine(firstPoint,thirdPoint);
    painter->drawLine(secondPoint,thirdPoint);
}

void PaintWidget::dragableLabel(QRect rect,ShapeTypes sType)
{
    QLabel* label = new QLabel(this);
    QPixmap pixmap = QPixmap(rect.normalized().size() + QSize(1,1));
    pixmap.fill(Qt::transparent);
    label->resize(pixmap.size());
    label->move(rect.normalized().topLeft());
    QPainter painter =  QPainter(&pixmap);
    QRect triangleRect = rect;
    rect = rect.normalized();
    switch (sType) {
    case CIRCLE :
        painter.drawEllipse(0,0,rect.size().width(),rect.size().height());
        break;
    case SQUARE :
        painter.drawRect(0,0,rect.size().width(),rect.size().height());
        break;
    case TRIANGLE :
    {
        triangleRect.moveCenter(QPoint(rect.size().width()/2,rect.size().height()/2));
        drawTriangle(triangleRect,&painter);
        break;
    }
    default:
        break;
    }
    label->setPixmap(pixmap);
    label->setAttribute(Qt::WA_DeleteOnClose);
    label->show();
}


