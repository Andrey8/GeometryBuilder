#include "gvector.h"
#include "GraphicsShapes/graphicspoint.h"
#include "mathnamespace.h"
#include "coordsystem.h"

#include <math.h>



GVector::GVector( qreal x, qreal y )
    : m_x( x ),
      m_y( y )
{}

GVector::GVector( QPointF const & beg, QPointF const & end )
    : GVector( end.x() - beg.x(), end.y() - beg.y() )
{}

GVector::GVector( GraphicsPoint * beg, GraphicsPoint * end )
    : GVector( beg->GetPosition(), end->GetPosition() )
{}



qreal GVector::Norm() const
{
    return sqrt( m_x * m_x + m_y * m_y );
}

GVector GVector::GetNormalized() const
{
    if ( IsNull() )
    {
        // throw
    }

    qreal norm = Norm();

    return GVector( m_x / norm, m_y / norm );
}

QPointF GVector::ToPoint() const
{
    return QPointF( m_x, m_y );
}



GVector const GVector::operator +( GVector const & other ) const
{
    return GVector( m_x + other.X(), m_y + other.Y() );
}

GVector const GVector::operator *( qreal k ) const
{
    return GVector( k * m_x, k * m_y );
}



bool GVector::IsNull() const
{
    return ( m_x == 0 && m_y == 0 );
}










