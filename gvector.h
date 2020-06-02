#ifndef GVECTOR_H
#define GVECTOR_H

#include "qglobal.h"
#include "mathnamespace.h"

#include <QPointF>

class GraphicsPoint;
class CoordSystem;



class GVector
{
public:
    // getters
    qreal X() const { return m_x; }
    qreal Y() const { return m_y; }
    qreal Norm() const;
    GVector GetNormalized() const;
    QPointF ToPoint() const;
    qreal GetAngle( CoordSystem const & system ) const;

    // operators
    GVector const operator +( GVector const & other ) const;
    GVector const operator *( qreal k ) const;

    // questions
    bool IsNull() const;

    GVector( qreal x, qreal y );
    GVector( QPointF const & beg, QPointF const & end );
    GVector( GraphicsPoint * beg, GraphicsPoint * end );
private:
    qreal m_x;
    qreal m_y;
};

#endif // GVECTOR_H
