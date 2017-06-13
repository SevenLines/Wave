#include "drawcanvas.h"
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

void DrawCanvas::_flushPixmap() {
    if (!this->drawingState.pixmapImage.isNull()) {
        pixmapItem->setPixmap(QPixmap::fromImage(this->drawingState.pixmapImage));
        for (auto item : this->drawingState.lines) {
            delete item;
        }
        this->drawingState.lines.clear();
    }
}

DrawCanvas::DrawCanvas(QWidget *parent) : QGraphicsView(parent) {
    this->_initScene();
    this->setImageSize(800, 600);
}

void DrawCanvas::setImageSize(int width, int height) {
    QImage newImage(width, height, QImage::Format_RGB888);
    newImage.fill(Qt::white);

    if (!this->pixmapItem->pixmap().isNull()) {
        auto image = this->pixmapItem->pixmap().toImage();
        auto painter = new QPainter(&newImage);
        painter->drawImage(QPoint(0, 0), image);
    }

    scene.setSceneRect(0, 0, width, height);

    this->pixmapItem->setPixmap(QPixmap::fromImage(newImage));
}

void DrawCanvas::mousePressEvent(QMouseEvent *event) {
    this->drawingState.pressPosition = this->mapToScene(event->pos());
    if (event->modifiers() & Qt::KeyboardModifier::ShiftModifier) {
        this->drawingState.state = DrawingState::PEN_RESIZING;
        auto offset = QPointF(this->penSizeItem->rect().width() / 2, this->penSizeItem->rect().height() / 2);
        this->penSizeItem->setPos(this->drawingState.pressPosition - offset);
    } else {
        this->drawingState.state = DrawingState::DRAWING;
        this->drawingState.pixmapImage = pixmapItem->pixmap().toImage();
    }
    this->drawingState.lastPosition = this->mapToScene(event->pos());

}

void DrawCanvas::mouseReleaseEvent(QMouseEvent *event) {
    this->drawingState.state = DrawingState::NONE;
    this->drawingState.lastPosition = this->mapToScene(event->pos());

    this->_flushPixmap();
}

void DrawCanvas::mouseMoveEvent(QMouseEvent *event) {
    QPainter painter;
    QPointF currentPosition = this->mapToScene(event->pos());
    QPen pen(this->drawingState.pen);
    switch (this->drawingState.state) {
        case DrawingState::DRAWING:
            painter.begin(&this->drawingState.pixmapImage);

            if (event->buttons() & Qt::MouseButton::RightButton) {
                pen.setBrush(QBrush(Qt::white));
            }

            painter.setPen(pen);
            painter.drawLine(this->drawingState.lastPosition, currentPosition);
            this->drawingState.lines.append(
                    this->scene.addLine(
                            QLineF(this->drawingState.lastPosition, currentPosition),
                            pen
                    )
            );
            if (this->drawingState.lines.size() >= 30) {
                this->_flushPixmap();
            }
            painter.end();
            break;
        case DrawingState::PEN_RESIZING:
            if (event->modifiers() & Qt::KeyboardModifier::ShiftModifier) {
                qreal radius = QLineF(this->drawingState.pressPosition, currentPosition).length();
                this->penSizeItem->setRect(QRectF(0, 0, 2 * radius, 2 * radius));
                auto offset = QPointF(radius, radius);
                this->penSizeItem->setPos(this->drawingState.pressPosition - offset);
                this->drawingState.pen.setWidth((int) (2 * radius));
            }
            break;
        case DrawingState::NONE:
            break;
    }

    this->drawingState.lastPosition = this->mapToScene(event->pos());

    auto offset = QPointF(this->penSizeItem->rect().width() / 2, this->penSizeItem->rect().height() / 2);
    this->penSizeItem->setPos(this->drawingState.lastPosition - offset);
    this->penSizeItem->setZValue(1000);
}

void DrawCanvas::_initScene() {
    this->setScene(&this->scene);
    this->scene.setBackgroundBrush(QBrush(QColor(Qt::black)));

    this->pixmapItem = new QGraphicsPixmapItem(0);
    this->scene.addItem(this->pixmapItem);
    this->drawingState.pen = QPen(QBrush(Qt::black), 10,
                                  Qt::PenStyle::SolidLine,
                                  Qt::PenCapStyle::RoundCap,
                                  Qt::PenJoinStyle::RoundJoin);

    this->penSizeItem = new QGraphicsCursorItem(QRectF(0, 0, 10, 10));
    this->scene.addItem(this->penSizeItem);
//    this->penSizeItem = this->scene.addEllipse();
}

QImage DrawCanvas::image() {
    return pixmapItem->pixmap().toImage();
}

QGraphicsScene* DrawCanvas::getScene() {
    return &this->scene;
}

