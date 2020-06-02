#pragma once

#include "gbuildernamespace.h"
#include "BSEHandlers/bsehandlerforcircle.h"

#include <QGraphicsItem>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>

class GraphicsPoint;
class GBPointPair;



class CircleDefiningObjects
{
public:
    CircleDefiningObjects( GraphicsPoint * center );
    CircleDefiningObjects( GraphicsPoint * center, GraphicsPoint * pointOnCircle );
    CircleDefiningObjects( GraphicsPoint * p1, GraphicsPoint * p2, GraphicsPoint * p3 );

    // getters
    GraphicsPoint * GetPointOnCircle() const { return m_pointOnCircle; }
    GBPointPair GetOtherTwoPoints( GraphicsPoint * p ) const;
    GraphicsPoint * GetP1() const { return m_p1; }
    GraphicsPoint * GetP2() const { return m_p2; }
    GraphicsPoint * GetP3() const { return m_p3; }

private:
    GraphicsPoint * m_center;
    GraphicsPoint * m_pointOnCircle;

    GraphicsPoint * m_p1;
    GraphicsPoint * m_p2;
    GraphicsPoint * m_p3;
};



class GraphicsCircle : public QGraphicsItem
{
public:
    enum class DefiningObjectsType
    {
        OnlyCenter,
        CenterAndPointOnCircle,
        ThreePoints
    };



    GraphicsCircle( QPointF const & c, qreal r );

    void PutPoint( GraphicsPoint * );

    // getters
	GeomShapes::Circle GetCircle() const { return GeomShapes::Circle( m_centerPos, m_radius ); }
    QPointF const & GetCenterPosition() const { return m_centerPos; }
    qreal GetRadius() const { return m_radius; }
    QList< GraphicsPoint * > const & GetNondefiningPoints() const { return m_nondefiningPoints; }
    DefiningObjectsType GetDefiningObjectsType() const { return m_definingObjectsType; }
    CircleDefiningObjects const & GetDefiningObjects() const { return m_definingObjects; }

    // setters
    void SetRadius( qreal r );
    void SetCenterPos( QPointF const & pos );
    void SetCenter( GraphicsPoint * p );
    void SetInBuildingProcess( bool value ) { m_isInBuildingProcess = value; }
    void SetDefiningObjectsType( DefiningObjectsType type ) { m_definingObjectsType = type; }
    void SetDefiningObjects( CircleDefiningObjects const & defObj ) { m_definingObjects = defObj; }
	void SetPaintMode( GB::CirclePaintMode mode ) { m_paintMode = mode; }

    void paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget ) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    //void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;
    //void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
private:
    QPointF m_centerPos;
    GraphicsPoint * m_center;
    qreal m_radius;
    DefiningObjectsType m_definingObjectsType;
    CircleDefiningObjects m_definingObjects;

	GB::CirclePaintMode m_paintMode;

    QList< GraphicsPoint * > m_points;
    QList< GraphicsPoint * > m_nondefiningPoints;

    QPainterPath m_selectionAreaPath;

    bool m_isInBuildingProcess;

    void CalculateSelectionArea();
};
