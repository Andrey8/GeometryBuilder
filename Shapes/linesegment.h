#pragma once

#include <QLineF>

class GraphicsPoint;

class QPointF;



namespace GeomShapes
{
    class LineSegment
    {
    public:
        LineSegment( QPointF const p1, QPointF const p2 );
        LineSegment( GraphicsPoint * p1, GraphicsPoint * p2 );

		// getters
        QPointF const & p1() const { return m_p1; }
        QPointF const & p2() const { return m_p2; }
        QLineF GetQLineF() const;
		QPointF GetMidpoint() const;

		// questions
		bool Intersects( LineSegment const & other ) const;

    private:
        QPointF m_p1;
        QPointF m_p2;
    };
}
