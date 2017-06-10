//
// Created by m on 10.06.17.
//

#ifndef TESTPROJECT_QGRAPHICSCURSORITEM_H
#define TESTPROJECT_QGRAPHICSCURSORITEM_H


#include <QtWidgets/QGraphicsEllipseItem>

class QGraphicsCursorItem : public QGraphicsEllipseItem {
public:
    explicit QGraphicsCursorItem(const QRectF &rect, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};


#endif //TESTPROJECT_QGRAPHICSCURSORITEM_H
