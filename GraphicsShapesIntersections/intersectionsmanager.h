#pragma once

#include <vector>

#include "mathnamespace.h"

class GraphicsPoint;
class GraphicsLine;
class GraphicsCircle;
class GraphicsLineSegment;



// Graphics Shapes Intersections.
namespace GSI
{
	class CircleWithLineIntersectionData
	{
	public:
		CircleWithLineIntersectionData( Math::CircleWithLineLocation l, std::vector< GraphicsPoint * > const & points )
			: m_location( l ),
			  m_intersectionPoints( points )
		{}

		Math::CircleWithLineLocation GetLocation() const { return m_location; }
		std::vector< GraphicsPoint * > const & GetIntersectionPoints() const { return m_intersectionPoints; }
	private:
		Math::CircleWithLineLocation m_location;
		std::vector< GraphicsPoint * > m_intersectionPoints;
	};

	class CircleWithLineSegmentIntersectionData
	{
	public:
		CircleWithLineSegmentIntersectionData( Math::CircleWithLineSegmentLocation l, std::vector< GraphicsPoint * > const & points )
			: m_location( l ),
			  m_intersectionPoints( points )
		{}

		Math::CircleWithLineSegmentLocation GetLocation() const { return m_location; }
		std::vector< GraphicsPoint * > const & GetIntersectionPoints() const { return m_intersectionPoints; }
	private:
		Math::CircleWithLineSegmentLocation m_location;
		std::vector< GraphicsPoint * > m_intersectionPoints;
	};

	class ShapesPairWithIntersectionData
	{
	public:
		ShapesPairWithIntersectionData( GraphicsCircle * c, GraphicsLine * l, CircleWithLineIntersectionData const & data )
			: m_circle( c ),
			  m_line( l ),
			  m_lineSegment( nullptr ),

			  m_circleWithLineData( new CircleWithLineIntersectionData( data ) ),
			  m_circleWithLineSegmentData( nullptr )
		{}
		ShapesPairWithIntersectionData( GraphicsCircle * c, GraphicsLineSegment * ls, CircleWithLineSegmentIntersectionData const & data )
			: m_circle( c ),
			  m_line( nullptr ),
			  m_lineSegment( ls ),

			  m_circleWithLineData( nullptr ),
			  m_circleWithLineSegmentData( new CircleWithLineSegmentIntersectionData( data ) )
		{}

		// getters
		GraphicsCircle * GetCircle() const { return m_circle; }
		GraphicsLine * GetLine() const { return m_line; }
		GraphicsLineSegment * GetLineSegment() const { return m_lineSegment; }
		CircleWithLineIntersectionData const & GetCircleWithLineIntersectionData() const { return *m_circleWithLineData; }
		CircleWithLineSegmentIntersectionData const & GetCircleWithLineSegmentIntersectionData() const { return *m_circleWithLineSegmentData; }

		// setters
		void SetIntersectionData( CircleWithLineIntersectionData const & data ) { *m_circleWithLineData = data; }
		void SetIntersectionData( CircleWithLineSegmentIntersectionData const & data ) { *m_circleWithLineSegmentData = data; }
	private:
		GraphicsCircle * m_circle;
		GraphicsLine * m_line;
		GraphicsLineSegment * m_lineSegment;

		CircleWithLineIntersectionData * m_circleWithLineData;
		CircleWithLineSegmentIntersectionData * m_circleWithLineSegmentData;

	};

    enum class IntersectionType
    {
        None,
        LineWithLine,
        LineSegmentsWithLineSegment,
        LineWithLineSegment,
        CircleWithLine,
        CircleWithLineSegment,
        CircleWithCircle
    };



	class IntersectionManager
	{
		typedef std::map< GraphicsPoint *, IntersectionType >::iterator MapIterator;
	public:
		// getters
		CircleWithLineIntersectionData const & GetIntersectionData( GraphicsCircle *, GraphicsLine * ) const;
		CircleWithLineSegmentIntersectionData const & GetIntersectionData( GraphicsCircle *, GraphicsLineSegment * ) const;
		IntersectionType GetIntersectionType( GraphicsPoint * ) const;

		// questions
		bool IsIntersectionPoint( GraphicsPoint * ) const;

		void RewriteData( GraphicsCircle * c, GraphicsLine * l, CircleWithLineIntersectionData const & data );

		// adders
		void AddData( GraphicsCircle *, GraphicsLine *, CircleWithLineIntersectionData const & );
		void AddData( GraphicsCircle *, GraphicsLineSegment *, CircleWithLineSegmentIntersectionData const & );
		void AddPointWithIntersectionType( GraphicsPoint *, IntersectionType );
	private:
		std::list< ShapesPairWithIntersectionData > m_shapesPairsWithIntersectionDatas;
		std::map< GraphicsPoint *, IntersectionType > m_pointsWithIntersectionTypes;
	};
}
