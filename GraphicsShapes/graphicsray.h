#pragma once

#include <QGraphicsItem>
//#include <QGraphicsSceneHoverEvent>

class GraphicsPoint;



class GraphicsRay : public QGraphicsItem
{
public:
	GraphicsRay( GraphicsPoint * vertex, QPointF const & insidePoint );

private:
	GraphicsPoint * m_vertex;
	QPointF m_insidePoint;
};

