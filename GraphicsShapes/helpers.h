#pragma once

#include <list>

namespace Math { class TwoLineSegmentsIntersectionData; }
namespace Math { class TwoLinesIntersectionData; }
namespace Math { class LineWithLineSegmentIntersectionData; }
namespace Math { class CircleWithLineIntersectionData; }
namespace Math { class CircleWithLineSegmentIntersectionData; }
namespace Math { class TwoCirclesIntersectionData; }
namespace Math { class Vector; }

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsLine;
class GraphicsCircle;

class QPointF;



namespace GSHelpers
{
	GraphicsLineSegment * GetContainingSegment(
			std::list< GraphicsLineSegment * > const & segments,
			GraphicsPoint * p );

	double GetDistance( GraphicsPoint const * first, GraphicsPoint const * second );

	QPointF GetMidpoint( GraphicsPoint * p1, GraphicsPoint * p2 );

	Math::TwoLineSegmentsIntersectionData GetIntersectionData( GraphicsLineSegment * ls1, GraphicsLineSegment * ls2 );
	Math::TwoLinesIntersectionData GetIntersectionData( GraphicsLine * l1, GraphicsLine * l2 );
	Math::LineWithLineSegmentIntersectionData GetIntersectionData( GraphicsLine * l, GraphicsLineSegment * ls );
	Math::CircleWithLineIntersectionData GetIntersectionData( GraphicsCircle * c, GraphicsLine * l );
	Math::CircleWithLineSegmentIntersectionData GetIntersectionData( GraphicsCircle *, GraphicsLineSegment * );
	Math::TwoCirclesIntersectionData GetIntersectionData( GraphicsCircle * c1, GraphicsCircle * c2 );

	QPointF GetPerpendicularBasePoint( QPointF const & pos, GraphicsLine * line );
	QPointF GetPerpendicularBasePoint( QPointF const & pos, GraphicsLineSegment * ls );

	QPointF GetPointOnCircle( QPointF const & pos, GraphicsCircle * c );

	Math::Vector GetVector( GraphicsLine * line );
	Math::Vector GetVector( GraphicsLineSegment * ls );

	std::pair< QPointF const, QPointF const > GetTangentPoints( QPointF const & sourcePoint, GraphicsCircle * circle );

	bool PointIsOutsideCircle( QPointF const & scenePos, GraphicsCircle * );
}
