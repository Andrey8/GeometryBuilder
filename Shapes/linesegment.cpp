#include "linesegment.h"
#include "GraphicsShapes/graphicspoint.h"

#include <QLineF>



GeomShapes::LineSegment::LineSegment( QPointF const p1, QPointF const p2 )
    : m_p1( p1 ),
      m_p2( p2 )
{}

GeomShapes::LineSegment::LineSegment( GraphicsPoint * p1, GraphicsPoint * p2 )
    : m_p1( p1->GetPosition() ),
      m_p2( p2->GetPosition() )
{}



QLineF GeomShapes::LineSegment::GetQLineF() const
{
    return QLineF( m_p1, m_p2 );
}

QPointF GeomShapes::LineSegment::GetMidpoint() const
{
    return QLineF( m_p1, m_p2 ).center();
}

bool GeomShapes::LineSegment::Intersects( GeomShapes::LineSegment const & other ) const
{
    QLineF const l1 = GetQLineF();
    QLineF const l2 = other.GetQLineF();

    //QPointF * p = new QPointF;
    if ( l1.intersect( l2, nullptr ) == QLineF::BoundedIntersection )
    {
        return true;
    }
    else
    {
        return false;
    }
}














