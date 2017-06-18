#ifndef DRAWCANVAS_H
#define DRAWCANVAS_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWidget>
#include <graphics_items/QGraphicsCursorItem.h>
#include <core/Wave.h>

class DrawCanvas : public QGraphicsView {
Q_OBJECT

private:
    QGraphicsScene _scene;
    QGraphicsPixmapItem *pixmapItem = nullptr;
    QGraphicsCursorItem *penSizeItem = nullptr;
    QGraphicsItemGroup* skeletonNode;

    enum class DrawingState {
        NONE, DRAWING, PEN_RESIZING
    };

    struct {
        DrawingState state = DrawingState::NONE;
        QPointF lastPosition = QPoint();
        QPointF pressPosition = QPoint();
        QImage pixmapImage;
        qreal initSize;
        QList<QGraphicsLineItem *> lines;
        QPen pen;
    } drawingState;

    void _initScene();
    void _flushPixmap();

public:
    explicit DrawCanvas(QWidget *parent = nullptr);
    QGraphicsScene * getScene();
    void setSkeleton(Skeleton *skeleton);
    QImage image();


signals:


public slots:

    void setImageSize(int width, int height);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    //void paintEvent(QPaintEvent *event);
};

#endif // DRAWCANVAS_H
