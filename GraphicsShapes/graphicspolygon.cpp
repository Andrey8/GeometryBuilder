#include "GraphicsShapes/graphicspolygon.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "interactionobjects.h"
#include "DebugHelpers/debughelper.h"
#include "polygonalgorithms.h"

#include <algorithm>

#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>



GraphicsPolygon::GraphicsPolygon( std::vector< GraphicsPoint * > const & vertices, std::vector< GraphicsLineSegment * > const & edges )
    : m_vertices( vertices ),
      m_edges( edges ),

      m_paintMode( GB::PolygonPaintMode::Usual )
{
    SetClockwiseOrderForVertices( vertices );

    CalculateBoundingRect();

    setPos( m_boundingRect.center() );
    CalculateSelectionAreaPath();
    CalculateVerticesPositions();

    setZValue( 5 );
    setAcceptHoverEvents( true );
}



void GraphicsPolygon::paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget )
{
    //painter->drawRect( boundingRect() );

    switch ( m_paintMode )
    {
    case GB::PolygonPaintMode::Usual :
    {
        painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
        //painter->setBrush( QBrush( Qt::lightGray ) );

        break;
    }
    case GB::PolygonPaintMode::Selected :
    {
        painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
        painter->setBrush( QBrush( Qt::darkGreen ) );

        break;
    }
    case GB::PolygonPaintMode::BlueColor :
    {
        painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
        painter->setBrush( QBrush( Qt::blue ) );

        painter->drawPath( shape() );

        break;
    }
    case GB::PolygonPaintMode::RedColor :
    {
        painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
        painter->setBrush( QBrush( Qt::red ) );

        painter->drawPath( shape() );

        break;
    }
    }

    //painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
    //painter->setBrush( QBrush( Qt::yellow ) );
    //painter->drawPath( m_selectionAreaPath );

    //GB::PushedBuildingButton const pushedBuildButton = InteractionObjects::buildingButtonsData.GetPushedBuildingButton();
    if ( InteractionObjects::userActionMode == GB::UserActionMode::PolygonSelection )
    {
        if ( option->state & QStyle::State_MouseOver )
        {
            painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
            painter->setBrush( QBrush( Qt::darkGreen ) );
            painter->drawPath( shape() );

            setZValue( 6 );
        }
        else
        {
            setZValue( 5 );
        }
    }
}

QRectF GraphicsPolygon::boundingRect() const
{
    //double const w = m_boundingRect.width();
    //double const h = m_boundingRect.height();
    //return QRectF( -w / 2, -h / 2, w, h );

    return m_selectionAreaPath.boundingRect();
}

QPainterPath GraphicsPolygon::shape() const
{
    return m_selectionAreaPath;
}



void GraphicsPolygon::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    if ( event->button() == Qt::LeftButton &&
            InteractionObjects::userActionMode == GB::UserActionMode::PolygonSelection )
    {
        InteractionObjects::TwoPolygonsSelecter & selecter = InteractionObjects::GetTwoPolygonsSelecter();

        if ( !selecter.FirstPolygonIsSelected() )
        {
            selecter.SetFirstPolygon( this );

            SetPaintMode( GB::PolygonPaintMode::BlueColor );
            update();
        }
        else
        {
            if ( this == selecter.GetFirstPolygon() )
            {
                SetPaintMode( GB::PolygonPaintMode::Usual );
                update();
            }
            else
            {
                selecter.SetSecondPolygon( this );

                SetPaintMode( GB::PolygonPaintMode::RedColor );
                update();
            }
        }
    }



}

/*
void GraphicsPolygon::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	SetPaintMode( GB::PolygonPaintMode::Selected );
	update();

	//DebugHelper::ShowMessageBox();
}

void GraphicsPolygon::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	SetPaintMode( GB::PolygonPaintMode::Usual );
	update();
}
*/



void GraphicsPolygon::SetClockwiseOrderForVertices( std::vector< GraphicsPoint * > const & vertices )
{
    std::vector< QPointF > positions;
    positions.reserve( vertices.size() );
    for ( GraphicsPoint * p : vertices )
    {
        positions.push_back( p->GetPosition() );
    }

    if ( PolygonAlgorithms::GetPolygonVerticesDirection( positions, Math::CircleDirection::Clockwise ) ==
            Math::CircleDirection::Counterclockwise )
    {
        m_vertices = std::vector< GraphicsPoint * >( vertices.rbegin(), vertices.rend() );
    }
    else
    {
        m_vertices = vertices;
    }
}

void GraphicsPolygon::CalculateBoundingRect()
{
    std::vector< GraphicsPoint * > vertices = m_vertices;

    std::sort( vertices.begin(), vertices.end(), [] ( GraphicsPoint * p1, GraphicsPoint * p2 ) {
        return p1->X() <= p2->X();
    } );
    double const x1 = vertices.front()->X();
    double const x2 = vertices.back()->X();

    std::sort( vertices.begin(), vertices.end(), [] ( GraphicsPoint * p1, GraphicsPoint * p2 ) {
        return p1->Y() <= p2->Y();
    } );
    double const y1 = vertices.front()->Y();
    double const y2 = vertices.back()->Y();

    m_boundingRect.setBottomLeft( ( QPointF( x1, y1 ) ) );
    m_boundingRect.setTopRight( ( QPointF( x2, y2 ) ) );
    double const z = GraphicsPoint::GetRadius();
    m_boundingRect.adjust( -z, z, z, -z );
}

void GraphicsPolygon::CalculateSelectionAreaPath()
{
    QPainterPath path;
    for ( GraphicsLineSegment * e : m_edges )
    {
        path = path.united( mapFromScene( e->GetSelectionAreaPath() ) );
    }

    m_selectionAreaPath = path;
}

void GraphicsPolygon::CalculateVerticesPositions()
{
    std::vector< QPointF > positions;
    positions.reserve( m_vertices.size() );
    for ( GraphicsPoint * p : m_vertices )
    {
        positions.push_back( p->GetPosition() );
    }
    m_verticesPositions = positions;
}











