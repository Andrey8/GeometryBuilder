#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "gbuildernamespace.h"
#include "DebugHelpers/debughelper.h"
#include "interactionobjects.h"
#include "DebugHelpers/exception.h"
#include "planescene.h"
#include "GraphicsShapesIntersections/objects.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"
#include "BSEHandlers/bsehandlerforline.h"
#include "BSEHandlers/objects.h"
#include "IntersectionPointsBSEHandlers/handlerforline.h"
#include "IntersectionPointsBSEHandlers/objects.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include <QCursor>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QMarginsF>
#include <QDebug>



GraphicsLine::GraphicsLine( GraphicsPoint * p1, GraphicsPoint * p2 )
    : m_definingPoints( p1, p2 ),

      m_paintMode( GB::LinePaintMode::Usual ),
      m_previousPaintMode( GB::LinePaintMode::Usual ),

      m_ends( GBPointPair( p1, p2 ) ),

      m_isInBuildingProcess( false )
{
    if ( !p1 || !p2 )
    {
        throw Exception( "GraphicsLine creation : p1 or p2 is null." );
    }

    m_points.push_back( p1 );
    m_points.push_back( p2 );

    m_extensionEnds = GetExtensionEnds( p1, p2, GB::mainWindowGeometryData.GetPlaneViewSize() );

    m_extendingSegments = GetExtendingSegments();

    m_initialSegment = new GraphicsLineSegment(
        m_definingPoints.GetFirst(),
        m_definingPoints.GetSecond() );
    GeomConstr::ConstructionData & cdata = GeomConstr::Objects::GetConstructionData();
    cdata.StoreGraphicsLineSegment( m_initialSegment );
    //m_initialSegment->SetAsLinePart( true );
    cdata.SetGLSAsLinePart( m_initialSegment, true );
    //m_initialSegment->SetContainingLine( this );
    cdata.SetGLSContainingLine( m_initialSegment, this );
    m_partitionSegments.push_back( m_initialSegment );

    m_partitionSegments.push_back( m_extendingSegments.first );
    m_partitionSegments.push_back( m_extendingSegments.second );

    CalculateSelectionArea();
    CalculateLinearPath();

    setZValue( 2 );
    setAcceptHoverEvents( true );
}



QList< GraphicsPoint * > GraphicsLine::GetNondefiningPoints() const
{
    QList< GraphicsPoint * > result;

    foreach ( GraphicsPoint * p, m_points )
    {
        if ( !m_definingPoints.ContainsItem( p ) )
        {
            result.push_back( p );
        }
    }

    return result;
}



bool GraphicsLine::operator==( GraphicsLine const & other ) const
{
    return ( Contains( other.m_definingPoints.GetFirst() ) && Contains( other.m_definingPoints.GetSecond() ) );
}



void GraphicsLine::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    CalculatePaintData();

    switch ( m_paintMode )
    {
    case GB::LinePaintMode::Usual :
    {
        painter->setPen( QPen( QBrush( Qt::black ), 2 ) );
        painter->drawLine( m_extensionEnds.GetFirst()->GetPosition(), m_extensionEnds.GetSecond()->GetPosition() );

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
    case GB::LinePaintMode::Selected :
    {
        painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
        painter->setBrush( QBrush( Qt::green ) );
        painter->drawPath( m_selectionAreaPath );

        setZValue( 2 );

        break;
    }
    }

    GB::PushedBuildingButton const pushedBuildButton =
        InteractionObjects::buildingButtonsData.GetPushedBuildingButton();
    if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
            !m_isInBuildingProcess &&
            ( pushedBuildButton == GB::PushedBuildingButton::AddPoint ||
              pushedBuildButton == GB::PushedBuildingButton::DrawLineSegment ||
              pushedBuildButton == GB::PushedBuildingButton::DrawLine ||
              pushedBuildButton == GB::PushedBuildingButton::AddCircle ||
              pushedBuildButton == GB::PushedBuildingButton::DrawPolygon ||
              pushedBuildButton == GB::PushedBuildingButton::AddIntersectionPoint ||
              pushedBuildButton == GB::PushedBuildingButton::AddPerpendicular ||
              pushedBuildButton == GB::PushedBuildingButton::AddParallel ||
              pushedBuildButton == GB::PushedBuildingButton::AddTangent ) )
    {
        if ( option->state & QStyle::State_MouseOver )
        {
            //DebugHelper::ShowMessageBox();

            painter->setPen( QPen( QBrush( Qt::darkBlue ), 1 ) );
            painter->setBrush( QBrush( Qt::green ) );
            painter->drawPath( m_selectionAreaPath );

            //painter->setBrush( Qt::NoBrush );
            //painter->drawRect( boundingRect() );

            setZValue( 3 );

            return;
        }
        else
        {
            setZValue( 2 );
        }
    }
}

QRectF GraphicsLine::boundingRect() const
{
    GraphicsPoint * beg = m_extensionEnds.GetFirst();
    GraphicsPoint * end = m_extensionEnds.GetSecond();

    return QRectF( beg->GetPosition(),
                   QSizeF( end->GetPosition().x() - beg->GetPosition().x(),
                           end->GetPosition().y() - beg->GetPosition().y() ) ).normalized()
           .marginsAdded( QMarginsF ( 100, 100, 100, 100 ) );
}

QPainterPath GraphicsLine::shape() const
{
    return m_selectionAreaPath;
}



void GraphicsLine::SetPaintMode( GB::LinePaintMode mode )
{
    if ( mode != m_paintMode )
    {
        m_previousPaintMode = m_paintMode;

        m_paintMode = mode;
    }
}



bool GraphicsLine::IsVertical() const
{
    return ( m_definingPoints.GetFirst()->X() == m_definingPoints.GetSecond()->X() );
}

bool GraphicsLine::IsHorizontal() const
{
    return ( m_definingPoints.GetFirst()->Y() == m_definingPoints.GetSecond()->Y() );
}

bool GraphicsLine::Contains( GraphicsPoint * point ) const
{
    return m_points.contains( point );
}

bool GraphicsLine::HasSegment( GraphicsLineSegment * segment ) const
{
    foreach ( GraphicsLineSegment * s, m_partitionSegments )
    {
        if ( segment == s )
        {
            return true;
        }
    }

    return false;
}

bool GraphicsLine::ContainsPointAsDefining( GraphicsPoint const * point ) const
{
    return ( m_definingPoints.ContainsItem( point ) );
}



void GraphicsLine::PutPoint( GraphicsPoint * point, GraphicsLineSegment * segment )
{
    if ( !point )
    {
        throw Exception( "GBLine::PutPoint : point is null." );
    }
    if ( !segment )
    {
        throw Exception( "GBLine::PutPoint : segment is null." );
    }

    if ( !HasSegment( segment ) )
    {
        throw Exception( "GBLine::PutPoint : line hasn't that segment." );
    }

    if ( segment != m_extendingSegments.first ||
            segment != m_extendingSegments.second )
    {
        scene()->removeItem( segment );
    }
    m_partitionSegments.removeOne( segment );



    m_points.push_back( point );

    GeomConstr::ConstructionData & cdata = GeomConstr::Objects::GetConstructionData();

    if ( segment == m_extendingSegments.first )
    {
        if ( segment->p1() == m_extensionEnds.GetFirst() ||
                segment->p1() == m_extensionEnds.GetSecond() )
        {
            // Delete old ...
            GraphicsLineSegment * extending = new GraphicsLineSegment( segment->p1(), point );
            m_extendingSegments.first = extending;
            m_partitionSegments.push_back( extending );

            GraphicsLineSegment * inside = new GraphicsLineSegment( segment->p2(), point );
            cdata.StoreGraphicsLineSegment( inside );
            //inside->SetAsLinePart( true );
            cdata.SetGLSAsLinePart( inside, true );
            //inside->SetContainingLine( this );
            cdata.SetGLSContainingLine( inside, this );
            m_partitionSegments.push_back( inside );
            scene()->addItem( inside );
        }
        else if ( segment->p2() == m_extensionEnds.GetFirst() ||
                  segment->p2() == m_extensionEnds.GetSecond() )
        {
            // Delete old ...
            GraphicsLineSegment * extending = new GraphicsLineSegment( segment->p2(), point );
            m_extendingSegments.first = extending;
            m_partitionSegments.push_back( extending );

            GraphicsLineSegment * inside = new GraphicsLineSegment( segment->p1(), point );
            cdata.StoreGraphicsLineSegment( inside );
            //inside->SetAsLinePart( true );
            cdata.SetGLSAsLinePart( inside, true );
            //inside->SetContainingLine( this );
            cdata.SetGLSContainingLine( inside, this );
            m_partitionSegments.push_back( inside );
            scene()->addItem( inside );
        }
        else
        {
            // throw
        }
    }
    else if ( segment == m_extendingSegments.second )
    {
        if ( segment->p1() == m_extensionEnds.GetFirst() ||
                segment->p1() == m_extensionEnds.GetSecond() )
        {
            GraphicsLineSegment * extending = new GraphicsLineSegment( segment->p1(), point );
            m_extendingSegments.second = extending;
            m_partitionSegments.push_back( extending );

            GraphicsLineSegment * inside = new GraphicsLineSegment( segment->p2(), point );
            cdata.StoreGraphicsLineSegment( inside );
            //inside->SetAsLinePart( true );
            cdata.SetGLSAsLinePart( inside, true );
            //inside->SetContainingLine( this );
            cdata.SetGLSContainingLine( inside, this );
            m_partitionSegments.push_back( inside );
            scene()->addItem( inside );
        }
        else if ( segment->p2() == m_extensionEnds.GetFirst() ||
                  segment->p2() == m_extensionEnds.GetSecond() )
        {
            GraphicsLineSegment * extending = new GraphicsLineSegment( segment->p2(), point );
            m_extendingSegments.second = extending;
            m_partitionSegments.push_back( extending );

            GraphicsLineSegment * inside = new GraphicsLineSegment( segment->p1(), point );
            cdata.StoreGraphicsLineSegment( inside );
            //inside->SetAsLinePart( true );
            cdata.SetGLSAsLinePart( inside, true );
            //inside->SetContainingLine( this );
            cdata.SetGLSContainingLine( inside, this );
            m_partitionSegments.push_back( inside );
            scene()->addItem( inside );
        }
        else
        {
            // throw
        }
    }
    else
    {
        GraphicsLineSegment * inside1 = new GraphicsLineSegment( segment->p1(), point );
        cdata.StoreGraphicsLineSegment( inside1 );
        //inside1->SetAsLinePart( true );
        cdata.SetGLSAsLinePart( inside1, true );
        //inside1->SetContainingLine( this );
        cdata.SetGLSContainingLine( inside1, this );
        m_partitionSegments.push_back( inside1 );
        scene()->addItem( inside1 );

        GraphicsLineSegment * inside2 = new GraphicsLineSegment( point, segment->p2() );
        cdata.StoreGraphicsLineSegment( inside2 );
        //inside2->SetAsLinePart( true );
        cdata.SetGLSAsLinePart( inside2, true );
        //inside2->SetContainingLine( this );
        cdata.SetGLSContainingLine( inside2, this );
        m_partitionSegments.push_back( inside2 );
        scene()->addItem( inside2 );
    }

    if ( *m_ends.GetFirst() < *m_ends.GetSecond() )
    {
        if ( *point < *m_ends.GetFirst() )
        {
            m_ends.SetFirst( point );
        }
        else if ( *m_ends.GetSecond() < *point )
        {
            m_ends.SetSecond( point );
        }
    }
    else if ( *m_ends.GetSecond() < *m_ends.GetFirst() )
    {
        if ( *point < *m_ends.GetSecond() )
        {
            m_ends.SetSecond( point );
        }
        else if ( *m_ends.GetFirst() < *point )
        {
            m_ends.SetFirst( point );
        }
    }

    //point->AddLineAsNondefined( this );
    cdata.AddLineAsNondefined( point, this );

    scene()->removeItem( segment );

    //delete segment;
}

void GraphicsLine::PutPoint( GraphicsPoint * p )
{
    if ( !p )
    {
        throw Exception( "GBLine::PutPoint( p ) : point is null." );
    }

    GraphicsLineSegment * segment = nullptr;
    foreach ( GraphicsLineSegment * ls, m_partitionSegments )
    {
        if ( ( *( ls->p1() ) < *p && *p < * ( ls->p2() ) ) || ( *( ls->p2() ) < *p && *p < * ( ls->p1() ) ) )
        {
            segment = ls;

            break;
        }
    }

    if ( !segment )
    {
        throw Exception( "GBLine::PutPoint( p ) : line segment for point is not found." );
    }

    PutPoint( p, segment );
}



GBPointPair GraphicsLine::GetExtensionEnds( GraphicsPoint * beg, GraphicsPoint * end, QSizeF const & areaSize ) const
{
    if ( !beg || !end )
    {
        throw Exception( "GraphicsLine::GetExtensionEnds : one of points is null." );
    }

    if ( beg == end )
    {
        throw Exception( "GraphicsLine::GetExtensionEnds : equal points." );
    }

    GBPointPair result;

    qreal w = qAbs( areaSize.width() );
    qreal h = qAbs( areaSize.height() );

    if ( beg->X() == end->X() )  // vertical case
    {
        result.SetPoints( new GraphicsPoint( beg->X(), 0 ),
                          new GraphicsPoint( beg->X(), h ) );
    }
    else if ( beg->Y() == end->Y() )  // horizontal case
    {
        result.SetPoints( new GraphicsPoint( 0, beg->Y() ),
                          new GraphicsPoint( w, beg->Y() ) );
    }
    else  // slope case
    {
        qreal x1 = beg->X();
        qreal y1 = beg->Y();

        qreal x2 = end->X();
        qreal y2 = end->Y();

        qreal k = ( y2 - y1 ) / ( x2 - x1 );

        qreal z;

        z = - k * x1 + y1;
        if ( !result.IsValid() )
        {
            if ( z >= 0 && z < h )
            {
                result.AddPoint( new GraphicsPoint( 0, z ) );
            }
        }

        z = ( h - y1 ) / k + x1;
        if ( !result.IsValid() )
        {
            if ( z >= 0 && z < w )
            {
                result.AddPoint( new GraphicsPoint( z, h ) );
            }
        }

        z = k * ( w - x1 ) + y1;
        if ( !result.IsValid() )
        {
            if ( z <= h && z > 0 )
            {
                result.AddPoint( new GraphicsPoint( w, z ) );
            }
        }

        z = - y1 / k + x1;
        if ( !result.IsValid() )
        {
            if ( z <= w && z > 0 )
            {
                result.AddPoint( new GraphicsPoint( z, 0 ) );
            }
        }
    }

    if ( !result.IsValid() )
    {
        throw Exception( "GraphicsLine::GetExtensionEnds : result point pair is not valid." );
    }

    return result;
}

QPair< GraphicsLineSegment *, GraphicsLineSegment * > GraphicsLine::GetExtendingSegments()
{
    if ( !m_extensionEnds.IsValid() )
    {
        throw Exception( "GBLine::GetExtendingSegments : m_extensionEnds is not valid." );
    }

    if ( !m_ends.IsValid() )
    {
        throw Exception( "GBLine::GetExtendingSegments : m_ends is not valid." );
    }

    GBPointPair sorted1 = m_extensionEnds.GetSorted();
    GBPointPair sorted2 = m_ends.GetSorted();

    GraphicsLineSegment * segment1 = new GraphicsLineSegment( sorted1.GetFirst(), sorted2.GetFirst() );
    GraphicsLineSegment * segment2 = new GraphicsLineSegment( sorted1.GetSecond(), sorted2.GetSecond() );

    return QPair< GraphicsLineSegment *, GraphicsLineSegment * >( segment1, segment2 );
}

void GraphicsLine::CalculateGeometryData()
{
    prepareGeometryChange();

    DestroyOldGeometryData();

    m_extensionEnds = GetExtensionEnds( m_definingPoints.GetFirst(), m_definingPoints.GetSecond(), GB::mainWindowGeometryData.GetPlaneViewSize() );
    m_extendingSegments = GetExtendingSegments();
    m_partitionSegments.push_back( m_extendingSegments.first );
    m_partitionSegments.push_back( m_extendingSegments.second );
}



void GraphicsLine::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    //DebugHelper::ShowTextInInput( "line" );

    if ( m_isInBuildingProcess )
    {
        return;
    }

    GraphicsShapesBSEHandlers::HandlerForLine & bseHandler = GraphicsShapesBSEHandlers::Objects::GetHandlerForLine();
    bseHandler.SetLine( this );
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
        case GB::PushedBuildingButton::None :
        {
            break;
        }
        case GB::PushedBuildingButton::AddPoint :
        {
            if ( event->button() == Qt::LeftButton )
            {
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
            bseHandler.HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Circle );

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
        case GB::PushedBuildingButton::AddIntersectionPoint :
        {
            if ( event->button() == Qt::LeftButton )
            {
                IntersectionPointsBSEHandlers::HandlerForLine & handler = IntersectionPointsBSEHandlers::Objects::GetHandlerForLine();
                handler.SetLine( this );

                handler.HandleIntersectionPointsAdding( event );
            }

            break;
        }
        case GB::PushedBuildingButton::AddLineSegmentMidpoint :
        {
            bseHandler.HandleLineSegmentMidpointAdding( event );

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

void GraphicsLine::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    // ...
}



void GraphicsLine::DestroyOldGeometryData()
{
    m_partitionSegments.removeOne( m_extendingSegments.first );
    m_partitionSegments.removeOne( m_extendingSegments.second );

    delete m_extendingSegments.first;
    delete m_extendingSegments.second;

    delete m_extensionEnds.GetFirst();
    delete m_extensionEnds.GetSecond();
    m_extensionEnds.Invalidate();
}

void GraphicsLine::CalculateSelectionArea()
{
    m_selectionAreaPath = QPainterPath();
    foreach ( GraphicsLineSegment * s, m_partitionSegments )
    {
        s->CalculatePaintData();
        m_selectionAreaPath.addPath( mapFromScene( s->GetSelectionAreaPath() ) );
    }
}

void GraphicsLine::CalculateLinearPath()
{
    QPainterPath p;
    foreach ( GraphicsLineSegment * s, m_partitionSegments )
    {
        s->CalculatePaintData();
        p.addPath( s->GetLinearPath() );
    }

    m_linearPath = p;
}

void GraphicsLine::CalculatePaintData()
{
    prepareGeometryChange();

    CalculateGeometryData();
    CalculateSelectionArea();
    CalculateLinearPath();
}












