#pragma once

#include "Shapes/linesegment.h"

#include <vector>
#include <list>

#include <QPolygonF>

class QPointF;



namespace GeomShapes
{
    class Polygon
    {
    public:
		Polygon( std::vector< QPointF > const & vertices );
		Polygon( std::list< QPointF > const & vertices );

        // getters
        std::vector< QPointF > const & GetVertices() const { return m_vertices; }
        std::vector< GeomShapes::LineSegment > const & GetEdges() const { return m_edges; }
		QPointF const & GetVertex( int index ) const { return GetVertex( m_vertices, index ); }
		uint16_t GetVerticesCount() const { return m_vertices.size(); }

    private:
        std::vector< QPointF > m_vertices;
		std::vector< GeomShapes::LineSegment > m_edges;

		static bool VerticesContainsEqualPoints( std::vector< QPointF > const & vertices );
		static bool VerticesAreCollinear( QPointF const & p1, QPointF const & p2, QPointF const & p3 );
		static LineSegment GetLineSegment( std::vector< QPointF > const & vertices, int index );
		static QPointF const & GetVertex( std::vector< QPointF > const & vertices, int index );
    };

	QPolygonF ToPolygonF( Polygon const & );
}
