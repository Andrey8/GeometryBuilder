#include "graphicscircle.h"
#include "gbuildernamespace.h"
#include "interactionobjects.h"
#include "DebugHelpers/debughelper.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "gbpointpair.h"
#include "DebugHelpers/exception.h"
#include "GraphicsShapesIntersections/objects.h"
#include "planescene.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"
#include "BSEHandlers/bsehandlerforcircle.h"
#include "BSEHandlers/objects.h"
#include "IntersectionPointsBSEHandlers/handlerforcircle.h"
#include "IntersectionPointsBSEHandlers/objects.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>



GraphicsCircle::GraphicsCircle( QPointF const & c, qreal r )
    : m_centerPos( c ),
      m_center( new GraphicsPoint( c ) ),
      m_radius( r ),
      m_definingObjectsType( DefiningObjectsType::OnlyCenter ),
      m_definingObjects( CircleDefiningObjects( m_center ) ),

      m_paintMode( GB::CirclePaintMode::Usual ),

      m_isInBuildingProcess( false )
{
    setPos( m_centerPos );
    CalculateSelectionArea();

    setZValue( 1 );
    setAcceptHoverEvents( true );
}



void GraphicsCircle::SetRadius( qreal r )
{
    m_radius = r;

    prepareGeometryChange();

    CalculateSelectionArea();
}

void GraphicsCircle::SetCenterPos( QPointF const & pos )
{
    m_centerPos = pos;
    setPos( m_centerPos );

    prepareGeometryChange();

    CalculateSelectionArea();
}

void GraphicsCircle::SetCenter( GraphicsPoint * p )
{
    m_centerPos = p->GetPosition();
    setPos( m_centerPos );

    prepareGeometryChange();

    CalculateSelectionArea();
}



void GraphicsCircle::paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget )
{
    switch ( m_paintMode )
    {
    case GB::CirclePaintMode::Usual :
    {
        painter->setPen( QPen( QBrush( Qt::black ), 2 ) );
        painter->setBrush( QBrush( Qt::NoBrush ) );

        painter->drawEllipse( QPointF( 0, 0 ), m_radius, m_radius );

        if ( !m_isInBuildingProcess )
        {
            setZValue( 1 );
        }
        else
        {
            setZValue( -10 );
        }

        break;
    }
    case GB::CirclePaintMode::Selected :
    {
        painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
        painter->setBrush( QBrush( Qt::green ) );
        painter->drawPath( m_selectionAreaPath );

        setZValue( 2 );

        break;
    }
    }

    GB::PushedBuildingButton const pushedBuildButton = InteractionObjects::buildingButtonsData.GetPushedBuildingButton();
    if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
            !m_isInBuildingProcess &&
            ( pushedBuildButton == GB::PushedBuildingButton::AddPoint ||
              pushedBuildButton == GB::PushedBuildingButton::DrawLineSegment ||
              pushedBuildButton == GB::PushedBuildingButton::DrawLine ||
              pushedBuildButton == GB::PushedBuildingButton::AddCircle ||
              pushedBuildButton == GB::PushedBuildingButton::DrawPolygon ||
              pushedBuildButton == GB::PushedBuildingButton::AddIntersectionPoint ||
              pushedBuildButton == GB::PushedBuildingButton::AddParallel ||
              pushedBuildButton == GB::PushedBuildingButton::AddTangent ) )
    {
        if ( option->state & QStyle::State_MouseOver )
        {
            painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
            painter->setBrush( QBrush( Qt::green ) );
            painter->drawPath( m_selectionAreaPath );

            setZValue( 2 );

            return;
        }
        else
        {
            setZValue( 1 );
        }
    }
}

QRectF GraphicsCircle::boundingRect() const
{
    qreal const r = m_radius;

    return  QRectF( -r, -r, 2 * r, 2 * r );
}

QPainterPath GraphicsCircle::shape() const
{
    return m_selectionAreaPath;
}



void GraphicsCircle::PutPoint( GraphicsPoint * p )
{
    m_points.push_back( p );
    m_nondefiningPoints.push_back( p );

    //p->AddCircleAsNondefined( this );
    Helpers::Objects::GetConstructionData().AddCircleAsNondefined( p, this );
}



void GraphicsCircle::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    GraphicsShapesBSEHandlers::HandlerForCircle & bseHandler = GraphicsShapesBSEHandlers::Objects::GetHandlerForCircle();
    bseHandler.SetCircle( this );
    switch ( InteractionObjects::userActionMode )
    {
    case GB::UserActionMode::None :
    {
        break;
    }
    case GB::UserActionMode::Building :
    {
        switch ( InteractionObjects::buildingButtonsData.GetPushedBuildingButton() )
        {
        case GB::PushedBuildingButton::AddPoint :
        {
            if ( event->button() == Qt::LeftButton )
            {
                //DebugHelper::ShowTextInInput( "circle" );

                bseHandler.HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Point );
            }

            break;
        }
        case GB::PushedBuildingButton::DrawLineSegment :
        {
            if ( event->button() == Qt::LeftButton )
            {
                bseHandler.HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::LineSegment );
            }

            break;
        }
        case GB::PushedBuildingButton::DrawLine :
        {
            if ( event->button() == Qt::LeftButton )
            {
                bseHandler.HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Line );
            }

            break;
        }
        case GB::PushedBuildingButton::AddCircle :
        {
            if ( event->button() == Qt::LeftButton )
            {
                bseHandler.HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Circle );
            }

            break;
        }
        case GB::PushedBuildingButton::AddIntersectionPoint :
        {
            if ( event->button() == Qt::LeftButton )
            {
                IntersectionPointsBSEHandlers::HandlerForCircle & handler = IntersectionPointsBSEHandlers::Objects::GetHandlerForCircle();
                handler.SetCircle( this );

                handler.HandleIntersectionPointsAdding( event );
            }

            break;
        }
        case GB::PushedBuildingButton::DrawPolygon :
        {
            // ...

            break;
        }
        case GB::PushedBuildingButton::AddTangent :
        {
            if ( event->button() == Qt::LeftButton )
            {
                //DebugHelper::ShowTextInInput( "In circle." );

                bseHandler.HandleTangentLSAdding( event );
            }
        }
        }

        break;
    }
    }
}



void GraphicsCircle::CalculateSelectionArea()
{
    qreal const r = 4;

    QPainterPath p1;
    p1.addEllipse( QPointF( 0, 0 ), m_radius + r, m_radius + r );
    QPainterPath p2;
    p2.addEllipse( QPointF( 0, 0 ), m_radius - r, m_radius - r );

    m_selectionAreaPath = p1.subtracted( p2 );
}



// class DefiningObjects :



CircleDefiningObjects::CircleDefiningObjects( GraphicsPoint * c )
    : m_center( c )
{}

CircleDefiningObjects::CircleDefiningObjects( GraphicsPoint * c, GraphicsPoint * p )
    : m_center( c ),
      m_pointOnCircle( p )
{}

CircleDefiningObjects::CircleDefiningObjects( GraphicsPoint * p1, GraphicsPoint * p2, GraphicsPoint * p3 )
    : m_p1( p1 ),
      m_p2( p2 ),
      m_p3( p3 )
{}



GBPointPair CircleDefiningObjects::GetOtherTwoPoints( GraphicsPoint * p ) const
{
    if ( p == m_p1 )
    {
        return GBPointPair( m_p2, m_p3 );
    }
    else if ( p == m_p2 )
    {
        return GBPointPair( m_p1, m_p3 );
    }
    else if ( p == m_p3 )
    {
        return GBPointPair( m_p1, m_p2 );
    }
    else
    {
        throw Exception( "CircleDefiningObjects::GetOtherTwoPoints( GBPoint * p ) : p is not in points triple." );
    }
}



















