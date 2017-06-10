//
// Created by m on 10.06.17.
//

#include <QtGui/QPainter>
#include "QGraphicsCursorItem.h"

void QGraphicsCursorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setCompositionMode(QPainter::CompositionMode::CompositionMode_Difference);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

QGraphicsCursorItem::QGraphicsCursorItem(const QRectF &rect, QGraphicsItem *parent)
        : QGraphicsEllipseItem(rect, parent) {
    this->setPen(QPen(Qt::white));
}
