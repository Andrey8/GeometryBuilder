#pragma once

#include "DebugHelpers/exception.h";
#include "Shapes/linesegment.h"

class QPointF;



namespace GeomShapes
{
	class Line
	{
	public:
		Line( QPointF const & p1, QPointF const & p2 )
			: m_p1( p1 ),
			  m_p2( p2 )
		{
			if ( p1 == p2 )
			{
				throw Exception( "Shapes::Line constructor : p1 == p2." );
			}
		}
		Line( LineSegment const & ls )
			: Line( ls.p1(), ls.p2() )
		{}

		QPointF const & p1() const { return m_p1; }
		QPointF const & p2() const { return m_p2; }

	private:
		QPointF const m_p1;
		QPointF const m_p2;
	};
}
