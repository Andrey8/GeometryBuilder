#pragma once

#include "coordsystem.h"

#include <QSize>
#include <QRect>
#include <QPointF>



namespace GB
{
    class MainWindowGeometryData
    {
    public:
        // getters
		QRect GetMainWindowRect() const { return m_mainWindowRect; }
		QSize GetPlaneViewSize() const { return m_planeViewSize; }

        // setters		
		void SetMainWindowRect( QRect const & rect );
		void SetPlaneViewSize( QSize const & size );

		MainWindowGeometryData();
    private:
        QRect m_mainWindowRect;
		QSize m_planeViewSize;
    };
	extern MainWindowGeometryData mainWindowGeometryData;

	enum class GraphicsShapeType
	{
		Point,
		LineSegment,
		Line,
		Polygon,
		Circle
	};

	enum class GraphicsLineSegmentLocationType
	{
		Containing,
		LinePart,
		LineSegmentPart
	};

	enum class UserActionMode
	{
		None,
		Building,
		PointMoving,
		PolygonSelection,
		PolygonAlgorithms
	};

	enum class ToolBarActiveWidget
	{
		Building,
		PolygonAlgorithms
	};

	enum class UserActionSubMode
	{
		None,
		LineBuildingInProcess
	};

	enum class PushedBuildingButton
    {
        None,
        AddPoint,
		DrawLineSegment,
        DrawLine,
        AddCircle,
		DrawPolygon,
		AddIntersectionPoint,
        ExtendLineSegment,
		AddLineSegmentMidpoint,
		AddPerpendicular,
		AddParallel,
		AddTangent
    };

	enum class BuildingButtonGroupObject
	{
		BuildingMode,
		PolygonAlgorithmsMode
	};

	enum class PointPaintMode
    {
        Usual,
        AroundCircle,
		Selected
    };

	enum class LineSegmentPaintMode
	{
		Usual,
		Selected,
		WithExtension,
		DotLine
	};

	enum class LinePaintMode
    {
		Usual,
		Selected
	};

	enum class PolygonPaintMode
	{
		Usual,
		Selected,
		BlueColor,
		RedColor
	};

	enum class CirclePaintMode
	{
		Usual,
		Selected
	};

    enum class CircleBuildingType
    {
        OnlyCircle,
        WithCenter,
        ThroughThreePoints
    };

	enum class PerpendicularBuildingType
	{
		LineSegment,
		Line
	};

	enum class ParallelBuildingType
	{
		LineSegment,
		Line
	};

	enum class TangentShapeBuildingType
	{
		LineSegments,
		Lines
	};
}
