#include "coordsystem.h"
#include "gvector.h"

#include <QPointF>



CoordSystem::CoordSystem( QPointF const & origin, GVector const & xAxisOrt,
                          Orientation orientation )
    : m_origin( origin ),
      m_xAxisOrt( xAxisOrt ),
      m_orientation( orientation )
{}



GVector CoordSystem::YAxisOrt() const
{
    switch ( m_orientation )
    {
    case Orientation::CounterClockwise :
    {
        return GVector( -m_xAxisOrt.Y(), m_xAxisOrt.X() ).GetNormalized();
    }
    case Orientation::Clockwise :
    {
        return GVector( m_xAxisOrt.Y(), -m_xAxisOrt.X() ).GetNormalized();
    }
    default :
        break;
    }
}












