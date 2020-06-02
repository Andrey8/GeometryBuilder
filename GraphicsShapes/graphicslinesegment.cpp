#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "coordsystem.h"
#include "mathnamespace.h"
#include "DebugHelpers/debughelper.h"
#include "interactionobjects.h"
#include "DebugHelpers/exception.h"
#include "planescene.h"
#include "BSEHandlers/objects.h"
#include "BSEHandlers/bsehandlerforlinesegment.h"
#include "IntersectionPointsBSEHandlers/handlerforlinesegment.h"
#include "IntersectionPointsBSEHandlers/objects.h"
#include "GraphicsShapes/helpers.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"
#include "Helpers/graphicslinesegmentdata.h"
#include "GraphicsShapes/constants.h"

#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>



uint16_t GraphicsLineSegment::sm_pressedLineSegmentCount = 0;

GraphicsLineSegment::GraphicsLineSegment( GraphicsPoint * p1, GraphicsPoint * p2 )
    : m_p1( p1 ),
      m_p2( p2 ),

      m_geometryData( new Helpers::GLSGeometryData() ),
      m_locationData( new Helpers::GLSLocationData() ),

      m_isInPolygonBuildingProcess( false ),
      m_paintMode( GB::LineSegmentPaintMode::Usual )
{
    if ( !p1 || !p2 )
    {
        throw Exception( "GBLineSegment creation : p1 or p2 is null." );
    }

    m_partitionSegments.push_back( this );

    setPos( QLineF( m_p1->GetPosition(), m_p2->GetPosition() ).center() );
    CalculatePaintData();

    setZValue( GSConstants::LINE_SEGMENT_ZVALUE );
    setAcceptHoverEvents( true );
}



QLineF GraphicsLineSegment::GetQLineF() const
{
    return QLineF( m_p1->GetPosition(), m_p2->GetPosition() );
}

QPainterPath GraphicsLineSegment::GetSelectionAreaPath() const
{
    return mapToScene( m_geometryData->m_selectionAreaPath );
}

QPainterPath GraphicsLineSegment::GetLinearPath() const
{
    return m_geometryData->m_linearPath;
}

GraphicsLineSegment * GraphicsLineSegment::GetContainingLineSegment() const
{
    return m_locationData->m_containingLineSegment;
}




bool GraphicsLineSegment::HasPointOnExtension() const
{
    return ( m_geometryData->m_extensionLineSegments.first ||
             m_geometryData->m_extensionLineSegments.second );
}



void GraphicsLineSegment::PutPoint( GraphicsPoint * p )
{
    if ( !m_locationData->m_isContaining )
    {
        //DebugHelper::ShowTextInInput( "not parent" );

        return;
    }

    if ( !p )
    {
        throw Exception( "GBLineSegment::PutPoint : point is null." );
    }

    PutPoint( p, GSHelpers::GetContainingSegment( m_partitionSegments, p ) );
}

void GraphicsLineSegment::PutPoint( GraphicsPoint * p, GraphicsLineSegment * segment )
{
    if ( !m_locationData->m_isContaining )
    {
        return;
    }

    if ( segment != this )
    {
        scene()->removeItem( segment );
    }
    m_partitionSegments.remove( segment );

    Helpers::ConstructionData & cdata = Helpers::Objects::GetConstructionData();

    GraphicsLineSegment * ls1 = new GraphicsLineSegment( segment->p1(), p );
    cdata.StoreGraphicsLineSegment( ls1 );
    //ls1->SetAsLineSegmentPart( true );
    cdata.SetGLSAsLineSegmentPart( ls1, true );
    //ls1->SetContainingLineSegment( this );
    cdata.SetGLSContainingLineSegment( ls1, this );
    GraphicsLineSegment * ls2 = new GraphicsLineSegment( segment->p2(), p );
    cdata.StoreGraphicsLineSegment( ls2 );
    //ls2->SetAsLineSegmentPart( true );
    cdata.SetGLSAsLineSegmentPart( ls2, true );
    //ls2->SetContainingLineSegment( this );
    cdata.SetGLSContainingLineSegment( ls2, this );
    m_partitionSegments.push_back( ls1 );
    m_partitionSegments.push_back( ls2 );
    scene()->addItem( ls1 );
    scene()->addItem( ls2 );

    Helpers::Objects::GetConstructionData().AddPointAsLineSegmentInterior( p, this );
    m_interiorPoints.push_back( p );

    //p->AddLineSegmentAsNondefined( this );
    cdata.AddLineSegmentAsNondefined( p, this );
}



void GraphicsLineSegment::CalculatePaintData()
{
    prepareGeometryChange();

    setPos( QLineF( m_p1->GetPosition(), m_p2->GetPosition() ).center() );
    CalculateGeometryData();
}



void GraphicsLineSegment::paint( QPainter * painter, QStyleOptionGraphicsItem const * option, QWidget * widget )
{
    GB::PushedBuildingButton const pushedBuildButton =
        InteractionObjects::buildingButtonsData.GetPushedBuildingButton();

    if ( !m_locationData->m_isLinePart && !m_locationData->m_isLineSegmentPart )
    {
        QLineF const lineSegment( mapFromScene( m_p1->GetPosition() ), mapFromScene( m_p2->GetPosition() ) );
        switch ( m_paintMode )
        {
        case GB::LineSegmentPaintMode::Usual :
        {
            painter->setPen( QPen( QBrush( Qt::black ), 2 ) );
            painter->drawLine( lineSegment );

            break;
        }
        case GB::LineSegmentPaintMode::Selected :
        {
            painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
            painter->setBrush( QBrush( Qt::green ) );
            painter->drawPath( m_geometryData->m_selectionAreaPath );

            break;
        }
        case GB::LineSegmentPaintMode::WithExtension :
        {
            painter->setPen( QPen( QBrush( Qt::black ), 2 ) );
            painter->drawLine( lineSegment );

            if ( m_geometryData->m_extensionLineSegments.first )
            {
                GeomShapes::LineSegment * s = m_geometryData->m_extensionLineSegments.first;

                painter->setPen( QPen( QBrush( Qt::black ), 1.5, Qt::DotLine ) );

                QLineF l( s->GetQLineF() );
                painter->drawLine( mapFromScene( l.p1() ), mapFromScene( l.p2() ) );
            }
            if ( m_geometryData->m_extensionLineSegments.second )
            {
                GeomShapes::LineSegment * s = m_geometryData->m_extensionLineSegments.second;

                painter->setPen( QPen( QBrush( Qt::black ), 1.5, Qt::DotLine ) );

                QLineF l( s->GetQLineF() );
                painter->drawLine( mapFromScene( l.p1() ), mapFromScene( l.p2() ) );
            }

            break;
        }
        case GB::LineSegmentPaintMode::DotLine :
        {
            painter->setPen(
                QPen( QBrush( Qt::black ), 1.5, Qt::DotLine ) );
            painter->drawLine( lineSegment );

            break;
        }
        }

        if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
                !m_isInPolygonBuildingProcess &&
                ( pushedBuildButton == GB::PushedBuildingButton::AddPoint ||
                  pushedBuildButton == GB::PushedBuildingButton::DrawLineSegment ||
                  pushedBuildButton == GB::PushedBuildingButton::DrawLine ||
                  pushedBuildButton == GB::PushedBuildingButton::AddCircle ||
                  pushedBuildButton == GB::PushedBuildingButton::DrawPolygon ||
                  pushedBuildButton == GB::PushedBuildingButton::AddIntersectionPoint ||
                  pushedBuildButton == GB::PushedBuildingButton::ExtendLineSegment ||
                  pushedBuildButton == GB::PushedBuildingButton::AddLineSegmentMidpoint ||
                  pushedBuildButton == GB::PushedBuildingButton::AddPerpendicular ||
                  pushedBuildButton == GB::PushedBuildingButton::AddParallel ||
                  pushedBuildButton == GB::PushedBuildingButton::AddTangent ) )
        {
            if ( option->state & QStyle::State_MouseOver )
            {
                painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
                painter->setBrush( QBrush( Qt::green ) );
                painter->drawPath( shape() );

                //painter->setBrush( Qt::NoBrush );
                //painter->drawRect( boundingRect() );

                setZValue( 5 );
            }
            else
            {
                setZValue( 4 );
            }
        }
    }

    if ( m_locationData->m_isLinePart )
    {
        if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
                !m_isInPolygonBuildingProcess )
        {
            if ( pushedBuildButton == GB::PushedBuildingButton::AddLineSegmentMidpoint )
            {
                if ( option->state & QStyle::State_MouseOver )
                {
                    painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
                    painter->setBrush( QBrush( Qt::green ) );
                    painter->drawPath( shape() );

                    //painter->setBrush( Qt::NoBrush );
                    //painter->drawRect( boundingRect() );

                    setZValue( 5 );
                }
                else
                {
                    setZValue( 4 );
                }
            }

            if ( pushedBuildButton == GB::PushedBuildingButton::AddPoint ||
                    pushedBuildButton == GB::PushedBuildingButton::DrawLineSegment ||
                    pushedBuildButton == GB::PushedBuildingButton::DrawLine ||
                    pushedBuildButton == GB::PushedBuildingButton::AddCircle ||
                    pushedBuildButton == GB::PushedBuildingButton::DrawPolygon ||
                    pushedBuildButton == GB::PushedBuildingButton::AddIntersectionPoint ||
                    pushedBuildButton == GB::PushedBuildingButton::AddPerpendicular ||
                    pushedBuildButton == GB::PushedBuildingButton::AddParallel ||
                    pushedBuildButton == GB::PushedBuildingButton::AddTangent )
            {
                if ( option->state & QStyle::State_MouseOver )
                {
                    painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
                    painter->setBrush( QBrush( Qt::green ) );
                    painter->drawPath( mapFromScene( m_locationData->m_containingLine->shape() ) );

                    //painter->setBrush( Qt::NoBrush );
                    //painter->drawRect( boundingRect() );

                    //setZValue( 5 );
                }
                else
                {
                    //setZValue( 4 );
                }
            }
        }
    }

    if ( m_locationData->m_isLineSegmentPart )
    {
        //DebugHelper::ShowMessageBox();

        if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
                !m_isInPolygonBuildingProcess )
        {
            if ( pushedBuildButton == GB::PushedBuildingButton::AddLineSegmentMidpoint )
            {
                if ( option->state & QStyle::State_MouseOver )
                {
                    painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
                    painter->setBrush( QBrush( Qt::green ) );
                    painter->drawPath( shape() );

                    //painter->setBrush( Qt::NoBrush );
                    //painter->drawRect( boundingRect() );

                    setZValue( 5 );
                }
                else
                {
                    setZValue( 4 );
                }
            }

            if ( pushedBuildButton == GB::PushedBuildingButton::AddPoint ||
                    pushedBuildButton == GB::PushedBuildingButton::DrawLineSegment ||
                    pushedBuildButton == GB::PushedBuildingButton::DrawLine ||
                    pushedBuildButton == GB::PushedBuildingButton::AddCircle ||
                    pushedBuildButton == GB::PushedBuildingButton::DrawPolygon ||
                    pushedBuildButton == GB::PushedBuildingButton::AddIntersectionPoint ||
                    pushedBuildButton == GB::PushedBuildingButton::ExtendLineSegment )
            {
                if ( option->state & QStyle::State_MouseOver )
                {
                    painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
                    painter->setBrush( QBrush( Qt::green ) );
                    painter->drawPath( mapFromItem(
                                           m_locationData->m_containingLineSegment,
                                           m_locationData->m_containingLineSegment->shape() ) );

                    //painter->setBrush( Qt::NoBrush );
                    //painter->drawRect( boundingRect() );

                    //setZValue( 5 );
                }
                else
                {
                    //setZValue( 4 );
                }
            }
        }
    }

}

QRectF GraphicsLineSegment::boundingRect() const
{
    qreal const w = m_geometryData->m_boundingRectWidth;
    qreal const h = m_geometryData->m_boundingRectHeight;

    return QRectF( -w / 2, -h / 2, w, h );
}

QPainterPath GraphicsLineSegment::shape() const
{
    if ( m_isInPolygonBuildingProcess )
    {
        return QPainterPath();
    }

    return m_geometryData->m_selectionAreaPath;
}



void GraphicsLineSegment::CalculateGeometryData()
{
    CalculateSlopeType( m_p1, m_p2, m_geometryData->m_slopeType );
    CalculateBoundingRectData();
    CalculateSelectionAreaPath();
    CalculateLinearPath();
    CalculateExtensionLineSegments();
}

void GraphicsLineSegment::CalculateSlopeType( GraphicsPoint * p1, GraphicsPoint * p2, Helpers::SlopeType & slopeType )
{
    if ( p1->Y() == p2->Y() )
    {
        slopeType = Helpers::SlopeType::Horizontal;
    }
    else if ( p1->X() == p2->X() )
    {
        slopeType = Helpers::SlopeType::Vertical;
    }
    else
    {
        if ( p1->X() < p2->X() )
        {
            if ( p1->Y() < p2->Y() )
            {
                slopeType = Helpers::SlopeType::Increasing;
            }
            else
            {
                slopeType = Helpers::SlopeType::Decreasing;
            }
        }
        else if ( p1->X() > p2->X() )
        {
            if ( p1->Y() < p2->Y() )
            {
                slopeType = Helpers::SlopeType::Decreasing;
            }
            else
            {
                slopeType = Helpers::SlopeType::Increasing;
            }
        }
    }
}

void GraphicsLineSegment::CalculateBoundingRectData()
{
    m_geometryData->m_boundingRectWidth = qAbs( m_p1->X() - m_p2->X() ) + 2 * GraphicsPoint::GetRadius();
    m_geometryData->m_boundingRectHeight = qAbs( m_p1->Y() - m_p2->Y() ) + 2 * GraphicsPoint::GetRadius();
}

void GraphicsLineSegment::CalculateSelectionAreaPath()
{
    qreal const r = GraphicsPoint::GetRadius();

    qreal const w = m_geometryData->m_boundingRectWidth - 2 * r;
    qreal const h = m_geometryData->m_boundingRectHeight - 2 * r;

    QPainterPath path;
    switch ( m_geometryData->m_slopeType )
    {
    case Helpers::SlopeType::Horizontal :
    {
        path.moveTo( - w / 2, - r );

        path.lineTo( w / 2, - r );
        path.lineTo( w / 2, r );
        path.lineTo( - w / 2, r );

        break;
    }
    case Helpers::SlopeType::Vertical :
    {
        path.moveTo( - r, - h / 2 );

        path.lineTo( r, - h / 2 );
        path.lineTo( r, h / 2 );
        path.lineTo( - r, h / 2 );

        break;
    }
    case Helpers::SlopeType::Increasing :
    {
        QPointF const p1 = mapFromScene( GetLeftEndCenter() );

        path.moveTo( p1 );

        QLineF aLine( p1, QPointF( p1.x() + h, p1.y() - w ) );
        aLine.setLength( r );
        QPointF const a = aLine.p2();
        path.lineTo( a );

        QPointF const b( a.x() + w, a.y() + h );
        path.lineTo( b );

        QLineF cLine( b, QPointF( b.x() - h, b.y() + w ) );
        cLine.setLength( 2 * r );
        QPointF const c = cLine.p2();
        path.lineTo( c );

        QPointF const d( c.x() - w, c.y() - h );
        path.lineTo( d );

        break;
    }
    case Helpers::SlopeType::Decreasing :
    {
        QPointF const p1 = mapFromScene( GetLeftEndCenter() );

        path.moveTo( p1 );

        QLineF aLine( p1, QPointF( p1.x() - h, p1.y() - w ) );
        aLine.setLength( r );
        QPointF const a = aLine.p2();
        path.lineTo( a );

        QPointF const b( a.x() + w, a.y() - h );
        path.lineTo( b );

        QLineF cLine( b, QPointF( b.x() + h, b.y() + w ) );
        cLine.setLength( 2 * r );
        QPointF const c = cLine.p2();
        path.lineTo( c );

        QPointF d( c.x() - w, c.y() + h );
        path.lineTo( d );

        break;
    }
    }
    path.closeSubpath();

    m_geometryData->m_selectionAreaPath = path;
}

void GraphicsLineSegment::CalculateLinearPath()
{
    QPainterPath p( m_p1->GetPosition() );
    p.lineTo( m_p2->GetPosition() );

    m_geometryData->m_linearPath = p;
}

void GraphicsLineSegment::CalculateExtensionLineSegments()
{
    delete m_geometryData->m_extensionLineSegments.first;
    m_geometryData->m_extensionLineSegments.first = nullptr;
    delete m_geometryData->m_extensionLineSegments.second;
    m_geometryData->m_extensionLineSegments.second = nullptr;



    GraphicsPoint * p1 = nullptr;
    GraphicsPoint * p2 = nullptr;
    if ( *m_p1 < *m_p2 )
    {
        p1 = m_p1;
        p2 = m_p2;
    }
    else
    {
        p1 = m_p2;
        p2 = m_p1;
    }

    GraphicsPoint * minPoint = m_p1;
    GraphicsPoint * maxPoint = m_p2;
    foreach ( GraphicsPoint * p, m_interiorPoints )
    {
        if ( *p < *p1 && *p < *minPoint )
        {
            minPoint = p;
        }

        if ( *p2 < *p && *maxPoint < *p )
        {
            maxPoint = p;
        }
    }
    if ( minPoint != m_p1 )
    {
        m_geometryData->m_extensionLineSegments.first = new GeomShapes::LineSegment( minPoint, m_p1 );
    }
    if ( maxPoint != m_p2 )
    {
        m_geometryData->m_extensionLineSegments.second = new GeomShapes::LineSegment( m_p2, maxPoint );
    }
}

QPointF GraphicsLineSegment::GetLeftEndCenter() const
{
    if ( m_geometryData->m_slopeType == Helpers::SlopeType::Vertical )
    {
        // throw
    }

    if ( m_p1->X() < m_p2->X() )
    {
        return m_p1->GetPosition();
    }
    else
    {
        return m_p2->GetPosition();
    }
}



void GraphicsLineSegment::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    GB::PushedBuildingButton const pushedBuildButton =
        InteractionObjects::buildingButtonsData.GetPushedBuildingButton();

    //DebugHelper::ShowTextInInput( "line segment 1" );

    if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
            m_locationData->m_isLinePart )
    {
        //DebugHelper::ShowTextInInput( "line segment is line part" );

        m_locationData->m_containingLine->mousePressEvent( event );

        return;
    }

    if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
            m_locationData->m_isLineSegmentPart )
    {
        //DebugHelper::ShowTextInInput( "line segment is line segment part" );

        if ( pushedBuildButton != GB::PushedBuildingButton::AddLineSegmentMidpoint )
        {
            m_locationData->m_containingLineSegment->mousePressEvent( event );

            return;
        }
        else
        {
            GraphicsShapesBSEHandlers::HandlerForLineSegment & bseHandler = GraphicsShapesBSEHandlers::Objects::GetHandlerForLineSegment();
            bseHandler.SetLineSegment( this );

            bseHandler.HandleLineSegmentMidpointAdding( event, GB::GraphicsLineSegmentLocationType::LineSegmentPart );

            return;
        }
    }

    //DebugHelper::ShowTextInInput( "line segment 2" );

    GraphicsShapesBSEHandlers::HandlerForLineSegment & bseHandler =
        GraphicsShapesBSEHandlers::Objects::GetHandlerForLineSegment();
    bseHandler.SetLineSegment( this );
    switch ( InteractionObjects::userActionMode )
    {
    case GB::UserActionMode::None :
    {
        break;
    }
    case GB::UserActionMode::Building :
    {
        switch ( pushedBuildButton )
        {
        case GB::PushedBuildingButton::AddPoint :
        {
            if ( event->button() == Qt::LeftButton )
            {
                ++sm_pressedLineSegmentCount;

                if ( sm_pressedLineSegmentCount == 1 )
                {
                    //DebugHelper::ShowTextInInput( "line segment ADD POINT" );

                    bseHandler.HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Point );
                }
                else
                {
                    return;
                }
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
                //DebugHelper::ShowTextInInput( "line segment DRAW LINE" );

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
        case GB::PushedBuildingButton::ExtendLineSegment :
        {
            static_cast< PlaneScene * >( scene() )->ExtendLineSegment( this );

            break;
        }
        case GB::PushedBuildingButton::AddIntersectionPoint :
        {
            if ( event->button() == Qt::LeftButton )
            {
                IntersectionPointsBSEHandlers::HandlerForLineSegment & handler = IntersectionPointsBSEHandlers::Objects::GetHandlerForLineSegment();
                handler.SetLineSegment( this );

                handler.HandleIntersectionPointsAdding( event );
            }

            break;
        }
        case GB::PushedBuildingButton::DrawPolygon :
        {
            if ( event->button() == Qt::LeftButton )
            {
                bseHandler.HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Polygon );
            }

            break;
        }
        case GB::PushedBuildingButton::AddLineSegmentMidpoint :
        {
            //DebugHelper::ShowTextInInput( "line segment ADD MIDPOINT" );

            bseHandler.HandleLineSegmentMidpointAdding( event, GB::GraphicsLineSegmentLocationType::Containing );

            break;
        }
        case GB::PushedBuildingButton::AddPerpendicular :
        {
            if ( event->button() == Qt::LeftButton )
            {
                bseHandler.HandlePerpendicularLSAdding( event );
            }

            break;
        }
        case GB::PushedBuildingButton::AddParallel :
        {
            if ( event->button() == Qt::LeftButton )
            {
                bseHandler.HandleParallelLSAdding( event );
            }

            break;
        }
        case GB::PushedBuildingButton::AddTangent :
        {
            if ( event->button() == Qt::LeftButton )
            {
                bseHandler.HandleTangentLSAdding( event );
            }

            break;
        }
        }

        break;
    }
    }
}

void GraphicsLineSegment::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    sm_pressedLineSegmentCount = 0;
}




