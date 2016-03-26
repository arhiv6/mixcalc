#include "myrect.h"

#include <QtMath>
#include <QPainter>

DiagramItem::DiagramItem(DiagramType type, qreal fMin, qreal fMax, int m, int n, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    QPen pen;
    float y1, y2;

    switch (type)
    {
        case FreeFreq:
            y1 = 0;
            y2 = sqrt(pow(n, 2) + pow(m, 2))+1;
            setBrush(QBrush(QColor(0x33, 0xaa, 0xFF, 128)));
            setToolTip(QString("<b>Free<br>F:%1-%2</b>").arg(fMin).arg(fMax));
            pen.setStyle(Qt::NoPen);
            setZValue(0);
            break;
        case SpurFreq:
            y1 = sqrt(pow(n, 2) + pow(m, 2));
            y2 = y1 + 1;
            setBrush(QBrush(QColor(0x33, 0x33, 0xFF, 128)));
            setToolTip(QString("<b>M:%1 N:%2<br>F:%3-%4</b>").arg(m).arg(n).arg(fMin).arg(fMax));
            break;
        case InputFreq:
            y1 = 0;
            y2 = 1;
            setBrush(QBrush(QColor(0xFF, 0x33, 0xFF)));
            setToolTip(QString("<b>Input<br>F:%1-%2</b>").arg(fMin).arg(fMax));
            break;
        default:
            return;
    }

    myPolygon << QPointF(fMin, y1) << QPointF(fMax, y1) << QPointF(fMax, y2) << QPointF(fMin, y2);
    setPolygon(myPolygon);
    pen.setCosmetic(true);
    pen.setCapStyle(Qt::FlatCap);
    setPen(pen);
    setAcceptHoverEvents(true);
     _type = type;
}

void DiagramItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    inverseBrush();
    QGraphicsPolygonItem::hoverEnterEvent(event);
}
void DiagramItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    inverseBrush();
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

void DiagramItem::inverseBrush()
{
    if (_type != FreeFreq)
    {
        QColor oldColor = brush().color();
        setBrush(QColor((~oldColor.red()) & 0xFF, (~oldColor.green()) & 0xFF, (~oldColor.blue()) & 0xFF, oldColor.alpha()));
    }
}
