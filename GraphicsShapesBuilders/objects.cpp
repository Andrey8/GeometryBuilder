#include "GraphicsShapesBuilders/objects.h"
#include "GraphicsShapesBuilders/pointbuilder.h"
#include "GraphicsShapesBuilders/linesegmentbuilder.h"
#include "GraphicsShapesBuilders/linebuilder.h"
#include "GraphicsShapesBuilders/circlebuilder.h"
#include "GraphicsShapesBuilders/polygonbuilder.h"
#include "GraphicsShapesBuilders/Additional/linesegmentmidpointbuilder.h"
#include "GraphicsShapesBuilders/Additional/intersectionpointbuilder.h"
#include "GraphicsShapesBuilders/Additional/perpendicularlinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/tangentlinesegmentbuilder.h"



GSBuilders::PointBuilder & GSBuilders::Objects::GetPointBuilder()
{
	static PointBuilder pb;

	return pb;
}

GSBuilders::LineSegmentBuilder & GSBuilders::Objects::GetLineSegmentBuilder()
{
	static LineSegmentBuilder lsb;

	return lsb;
}

GSBuilders::LineBuilder & GSBuilders::Objects::GetLineBuilder()
{
	static LineBuilder lb;

	return lb;
}

GSBuilders::CircleBuilder & GSBuilders::Objects::GetCircleBuilder()
{
	static CircleBuilder cb;

	return cb;
}

GSBuilders::PolygonBuilder & GSBuilders::Objects::GetPolygonBuilder()
{
	static PolygonBuilder polb;

	return polb;
}

GSBuilders::Additional::LineSegmentMidpointBuilder & GSBuilders::Objects::GetLineSegmentMidpointBuilder()
{
	static Additional::LineSegmentMidpointBuilder builder;

	return builder;
}

GSBuilders::Additional::IntersectionPointsBuilder & GSBuilders::Objects::GetIntersectionPointsBuilder()
{
	static Additional::IntersectionPointsBuilder ipbuilder;

	return ipbuilder;
}

GSBuilders::Additional::PerpendicularLineSegmentBuilder & GSBuilders::Objects::GetPerpendicularLineSegmentBuilder()
{
	static Additional::PerpendicularLineSegmentBuilder perpbuilder;

	return perpbuilder;
}

GSBuilders::Additional::ParallelLineSegmentBuilder & GSBuilders::Objects::GetParallelLineSegmentBuilder()
{
	static Additional::ParallelLineSegmentBuilder parBuilder;

	return parBuilder;
}

GSBuilders::Additional::TangentLineSegmentBuilder & GSBuilders::Objects::GetTangentLineSegmentBuilder()
{
	static Additional::TangentLineSegmentBuilder builder;

	return builder;
}
