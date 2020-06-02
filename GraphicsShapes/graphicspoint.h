#pragma once

#include "gbuildernamespace.h"

#include <QGraphicsItem>

class QPainter;
class QPointF;
class QRectF;



class GraphicsPoint : public QGraphicsItem
{
public:
	GraphicsPoint( QPointF const & pos );
	GraphicsPoint( qreal x, qreal y );

    // getters
    QPointF const & GetPosition() const { return m_position; }
    QPointF const & GetOldPosition() const { return m_oldPosition; }
    qreal X() const { return m_position.x(); }
    qreal Y() const { return m_position.y(); }
	static qreal GetRadius() { return sm_radius; }

    // setters
	void SetPosition( QPointF const & pos );
	void SetPaintMode( GB::PointPaintMode mode ) { m_paintMode = mode; }

    // operators
    //bool operator==( GraphicsPoint const & other ) const;
    bool operator<( GraphicsPoint const & other ) const;
    //bool operator<=( GraphicsPoint const & other ) const;

	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;    
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;



    QPointF m_pressPlace;
    void SetOldPositionAsCurrent();
private:
	QPointF m_position;
	static qreal sm_radius;

	QPointF m_oldPosition;

	GB::PointPaintMode m_paintMode;
};
