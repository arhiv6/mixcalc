#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>

class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum DiagramType { FreeFreq, SpurFreq, InputFreq };
    explicit DiagramItem(DiagramType type, qreal fMin, qreal fMax, int m = 0, int n = 0, QGraphicsItem *parent = 0);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private slots:
    void inverseBrush();

private:
    DiagramType _type;
    QPolygonF myPolygon;
};

#endif // DIAGRAMITEM_H
