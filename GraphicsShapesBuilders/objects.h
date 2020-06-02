#pragma once

namespace GSBuilders { class PointBuilder; }
namespace GSBuilders { class LineSegmentBuilder; }
namespace GSBuilders { class LineBuilder; }
namespace GSBuilders { class CircleBuilder; }
namespace GSBuilders { class PolygonBuilder; }
namespace GSBuilders { namespace Additional { class LineSegmentMidpointBuilder; } }
namespace GSBuilders { namespace Additional { class IntersectionPointsBuilder; } }
namespace GSBuilders { namespace Additional { class PerpendicularLineSegmentBuilder; } }
namespace GSBuilders { namespace Additional { class ParallelLineSegmentBuilder; } }
namespace GSBuilders { namespace Additional { class TangentLineSegmentBuilder; } }



namespace GSBuilders
{
	namespace Objects
	{		
		PointBuilder & GetPointBuilder();
		LineSegmentBuilder & GetLineSegmentBuilder();
		LineBuilder & GetLineBuilder();
		CircleBuilder & GetCircleBuilder();
		PolygonBuilder & GetPolygonBuilder();
		Additional::LineSegmentMidpointBuilder & GetLineSegmentMidpointBuilder();
		Additional::IntersectionPointsBuilder & GetIntersectionPointsBuilder();
		Additional::PerpendicularLineSegmentBuilder & GetPerpendicularLineSegmentBuilder();
		Additional::ParallelLineSegmentBuilder & GetParallelLineSegmentBuilder();
		Additional::TangentLineSegmentBuilder & GetTangentLineSegmentBuilder();
	}
}
