#pragma once

#include "gbuildernamespace.h"
#include "gbpointpair.h"
#include "Shapes/linesegment.h"

#include <QPointF>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QObject>

class GraphicsPoint;
class GraphicsLine;
class GraphicsLineSegment;
namespace GeomConstr { enum class SlopeType; }
namespace GeomConstr { struct GLSGeometryData; }
namespace GeomConstr { struct GLSLocationData; }



class GraphicsLineSegment : public QObject, public QGraphicsItem
{
public:
	GraphicsLineSegment( GraphicsPoint * beg, GraphicsPoint * end );

    // getters
    GraphicsPoint * p1() const { return m_p1; }
    GraphicsPoint * p2() const { return m_p2; }
    QLineF GetQLineF() const;
	QPainterPath GetSelectionAreaPath() const;
	QPainterPath GetLinearPath() const;
	GraphicsLineSegment * GetContainingLineSegment() const;
	GeomConstr::GLSGeometryData * GetGeometryData() const { return m_geometryData; }
	GeomConstr::GLSLocationData * GetLocationData() const { return m_locationData; }
	//QList< GraphicsPoint * > const & GetInteriorPoints() const { return m_interiorPoints; }

    // setters
    void SetPaintMode( GB::LineSegmentPaintMode mode ) { m_paintMode = mode; }
	void SetAsInPolygonBuildingProcess( bool value ) { m_isInPolygonBuildingProcess = value; }

    // questions
    bool HasPointOnExtension() const;

    void PutPoint( GraphicsPoint * p );
	void PutPoint( GraphicsPoint * p, GraphicsLineSegment * );

    void CalculatePaintData();
    void CalculateExtensionLineSegments();

    void paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget ) override;
    QRectF boundingRect() const override;
	QPainterPath shape() const override;

	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;

	~GraphicsLineSegment() {}
private:
	GraphicsPoint * m_p1;
	GraphicsPoint * m_p2;

	GeomConstr::GLSGeometryData * m_geometryData;
	GeomConstr::GLSLocationData * m_locationData;
	QList< GraphicsPoint * > m_interiorPoints;
	std::list< GraphicsLineSegment * > m_partitionSegments;

	static uint16_t sm_pressedLineSegmentCount;
	GB::LineSegmentPaintMode m_paintMode;

	bool m_isInPolygonBuildingProcess;



	void CalculateGeometryData();
	void CalculateSlopeType( GraphicsPoint * p1, GraphicsPoint * p2, GeomConstr::SlopeType & type );
	void CalculateBoundingRectData();
	void CalculateSelectionAreaPath();
	void CalculateLinearPath();	
	QPointF GetLeftEndCenter() const;
};


