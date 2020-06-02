#ifndef COORDSYSTEM_H
#define COORDSYSTEM_H

#include "gvector.h"

#include <QPointF>



class CoordSystem
{
public:
    enum Orientation
    {
        CounterClockwise,
        Clockwise
    };

    // getters
    QPointF const & Origin() const { return m_origin; }
    GVector const & XAxisOrt() const { return m_xAxisOrt; }
    GVector YAxisOrt() const;
    Orientation GetOrientation() const { return m_orientation; }

    // static QPointF GetMappedPoint( QPointF const & point, CoordSystem const & system );

    CoordSystem( QPointF const & origin, GVector const & xAxisOrt,
                 Orientation o = CounterClockwise );
private:
    QPointF m_origin;
    GVector m_xAxisOrt;
    Orientation m_orientation;
};

#endif // COORDSYSTEM_H
