#include "polygon.h"
#include "DebugHelpers/exception.h"
#include "mathnamespace.h"

#include <cmath>
#include <QLineF>



GeomShapes::Polygon::Polygon( std::vector< QPointF > const & vertices )
{
	uint16_t const n = vertices.size();

	if ( vertices.size() < 3 )
	{
		throw Exception( "Polygon creation : vertices are too few." );
	}

	if ( VerticesContainsEqualPoints( vertices ) )
	{
		throw Exception( "Polygon creation : there are equal points in vertices." );
	}

	for ( int s = 2; s <= n / 2; ++s )
	{
		for ( int i = 0; i < n; ++i )
		{
			if ( GetLineSegment( vertices, i ).Intersects( GetLineSegment( vertices, i + s ) ) )
			{
				throw Exception( "Polygon creation : there is selfintersection." );
			}
		}
	}

	for ( uint16_t i = 0; i < vertices.size(); ++i )
	{
		if ( VerticesAreCollinear( GetVertex( vertices, i - 1 ),
								   GetVertex( vertices, i ),
								   GetVertex( vertices, i + 1 ) ) )
		{
			throw Exception( "Polygon creation : three consecutive vertices are collinear." );
		}
	}

	m_vertices = vertices;
	m_edges.reserve( vertices.size() );
	for ( uint16_t i = 0; i < vertices.size() - 1; ++i )
	{
		m_edges.push_back( LineSegment( vertices[ i ], vertices[ i + 1 ] ) );
	}
	m_edges.push_back( LineSegment( vertices.front(), vertices.back() ) );
}

GeomShapes::Polygon::Polygon( std::list< QPointF > const & vertices )
	: Polygon( std::vector< QPointF >( vertices.begin(), vertices.end() ) )
{}



bool GeomShapes::Polygon::VerticesContainsEqualPoints( std::vector< QPointF > const & vertices )
{
	for ( uint16_t i = 0; i < vertices.size() - 1; ++i )
	{
		for ( uint16_t k = i + 1; k < vertices.size(); ++k )
		{
			if ( vertices[ i ] == vertices[ k ] )
			{
				return true;
			}
		}
	}

	return false;
}

bool GeomShapes::Polygon::VerticesAreCollinear( QPointF const & p1, QPointF const & p2, QPointF const & p3 )
{
	if ( Math::GetScalarProduct( QLineF( p1, p2 ).normalVector(), QLineF( p1, p3 ) ) == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

GeomShapes::LineSegment GeomShapes::Polygon::GetLineSegment( std::vector< QPointF > const & vertices, int index )
{
	return LineSegment( GetVertex( vertices, index ), GetVertex( vertices, index + 1 ) );
}

QPointF const & GeomShapes::Polygon::GetVertex( std::vector< QPointF > const & vertices, int index )
{
	int r = Math::GetRemainder( index, vertices.size() );

	return vertices[ r ];
}



QPolygonF GeomShapes::ToPolygonF( GeomShapes::Polygon const & pol )
{
	QVector< QPointF > vertices;
	vertices.reserve( pol.GetVerticesCount() );
	for ( QPointF const & p : pol.GetVertices() )
	{
		vertices.push_back( p );
	}

	return vertices;
}



