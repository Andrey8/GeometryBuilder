#pragma once

#include <QList>

class QPointF;

class GraphicsPoint;



namespace GBHelper
{
    bool operator <( QPointF const & lhs, QPointF const & rhs );
    bool operator <=( QPointF const & lhs, QPointF const & rhs );

	//template < typename T > bool IsAllItemsSatisfyCondition( QList< T > list, bool * condition() );


}

