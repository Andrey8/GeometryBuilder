#pragma once

#include "Shapes/linesegment.h"

#include <QPointF>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QObject>
#include <QList>

class GraphicsPoint;
class GraphicsLine;
class GraphicsLineSegment;



namespace Helpers
{
    enum class SlopeType
    {
        Horizontal,
        Vertical,
        Increasing,
        Decreasing
    };

	struct GLSGeometryData
	{
		SlopeType m_slopeType;
		qreal m_boundingRectWidth;
		qreal m_boundingRectHeight;
		QPainterPath m_selectionAreaPath;
		QPainterPath m_linearPath;
		std::pair< GeomShapes::LineSegment *, GeomShapes::LineSegment * > m_extensionLineSegments;
	};

	struct GLSLocationData
	{
		GLSLocationData()
			: m_isLinePart( false ),
			  m_isLineSegmentPart( false ),
			  m_isContaining( false ),
			  m_containingLine( nullptr ),
			  m_containingLineSegment( nullptr )
		{}

		bool m_isLinePart;
		bool m_isLineSegmentPart;
		bool m_isContaining;
		GraphicsLine * m_containingLine;
		GraphicsLineSegment * m_containingLineSegment;
	};

	struct GraphicsLineSegmentData
	{
		GraphicsLineSegmentData()
			: m_geometryData( new GLSGeometryData() ),
			  m_locationData( new GLSLocationData() )
		{}

		GLSGeometryData * m_geometryData;
		GLSLocationData * m_locationData;
		QList< GraphicsPoint * > m_interiorPoints;
		std::list< GraphicsLineSegment * > m_partitionSegments;
	};
}
