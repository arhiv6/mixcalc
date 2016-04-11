/*
 * Copyright 2016 Vyacheslav Verkhovin
 *
 * This file is part of MixCalc.
 *
 * MixCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RxCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RxCalc. If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
