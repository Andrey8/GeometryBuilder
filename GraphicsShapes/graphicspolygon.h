#pragma once

#include "gbuildernamespace.h"

#include <cstdint>
#include <vector>

#include <QGraphicsItem>

class GraphicsPoint;
class GraphicsLineSegment;



class GraphicsPolygon : public QGraphicsItem
{
public:
	GraphicsPolygon( std::vector< GraphicsPoint * > const & vertices, std::vector< GraphicsLineSegment * > const & edges );

	// getters
	uint16_t GetVerticesCount() const { return m_vertices.size(); }
	std::vector< GraphicsPoint * > const & GetVertices() const { return m_vertices; }
	std::vector< QPointF > const & GetVerticesPositions() const { return m_verticesPositions; }
	std::vector< GraphicsLineSegment * > const & GetEdges() const { return m_edges; }

	// setters
	void SetPaintMode( GB::PolygonPaintMode mode ) { m_paintMode = mode; }

	void CalculateSelectionAreaPath();
	void CalculateVerticesPositions();

	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
protected:
	void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
	//void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;
	//void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;

	//void hoverEnterEvent( QGraphicsSceneHoverEvent * ) override;
	//void hoverLeaveEvent( QGraphicsSceneHoverEvent * ) override;
private:
	std::vector< GraphicsPoint * > m_vertices;
	std::vector< GraphicsLineSegment * > m_edges;

	std::vector< QPointF > m_verticesPositions;
	QRectF m_boundingRect;
	QPainterPath m_selectionAreaPath;

	GB::PolygonPaintMode m_paintMode;

	void SetClockwiseOrderForVertices( std::vector< GraphicsPoint * > const & vertices );
	void CalculateBoundingRect();
};

