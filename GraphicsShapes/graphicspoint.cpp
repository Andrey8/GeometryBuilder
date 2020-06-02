#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicspolygon.h"
#include "Widgets/planeview.h"
#include "planescene.h"
#include "mathnamespace.h"
#include "gbuilderhelper.h"
#include "interactionobjects.h"
#include "DebugHelpers/exception.h"
#include "GraphicsShapesIntersections/objects.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"
#include "BSEHandlers/bsehandlerforpoint.h"
#include "BSEHandlers/objects.h"
#include "constants.h"
#include "GraphicsShapes/helpers.h"
#include "GraphicsPointMoving/movingdata.h"
#include "GraphicsPointMoving/movingpointsgroup.h"
#include "GraphicsPointMoving/movingmanager.h"
#include "GraphicsPointMoving/objects.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"
#include "GraphicsShapesBuilders/objects.h"
#include "GraphicsShapesBuilders/polygonbuilder.h"

#include <math.h>

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPointF>
#include <QRectF>
#include <QDebug>
#include <QStyleOptionGraphicsItem>



qreal GraphicsPoint::sm_radius = 5.0;

GraphicsPoint::GraphicsPoint( const QPointF & pos )
    : m_position( pos ),

      m_oldPosition( m_position ),

      m_paintMode( GB::PointPaintMode::Usual )
{
    setPos( m_position );

    setAcceptHoverEvents( true );
    setZValue( GSConstants::POINT_ZVALUE );
    setCursor( Qt::OpenHandCursor );
}

GraphicsPoint::GraphicsPoint( qreal x, qreal y )
    : GraphicsPoint( QPointF( x, y ) )
{}


/*
uint16_t GraphicsPoint::GetNondefinedShapesCount() const
{
    return ( m_nondefinedLineSegments.size() + m_nondefinedLines.size() + m_nondefinedCircles.size() );
}
*/
/*
GraphicsPoint * GraphicsPoint::GetNearestPoint( GraphicsPoint * first, GraphicsPoint * second ) const
{
	if ( GSHelpers::GetDistance( this, first ) <= GSHelpers::GetDistance( this, second ) )
    {
        return first;
    }
    else
    {
        return second;
    }
}

GraphicsPoint * GraphicsPoint::GetNearestPoint( GBPointPair const & points ) const
{
	return GetNearestPoint( points.GetFirst(), points.GetSecond() );
}
*/
/*
Math::Angle const & GraphicsPoint::GetAngleInNondefinedCircle( GraphicsCircle * c )
{
    foreach ( CircleWithAngle const & pair, m_nondefinedCirclesWithAngles )
    {
        if ( pair.m_circle == c )
        {
            return pair.m_angle;
        }
    }

    throw Exception( "GBPoint::GetAngleInNondefinedCircle : circle is not nondefined for this point." );
}
*/


void GraphicsPoint::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    switch ( m_paintMode )
    {
    case GB::PointPaintMode::Usual :
    {
        painter->setPen( QPen( Qt::black ) );
        painter->setBrush( QBrush( Qt::black ) );

        QRectF pointRect( - sm_radius,
                          - sm_radius,
                          2 * sm_radius,
                          2 * sm_radius );

        painter->drawEllipse( pointRect );

        break;
    }
    case GB::PointPaintMode::AroundCircle :
    {
        painter->setPen( QPen( Qt::black ) );
        painter->setBrush( QBrush( Qt::black ) );

        QRectF pointRect( - sm_radius,
                          - sm_radius,
                          2 * sm_radius,
                          2 * sm_radius );

        painter->drawEllipse( pointRect );

        qreal r = sm_radius * 4;

        painter->setPen( QPen( QBrush( Qt::darkGreen ), 2 ) );
        painter->setBrush( Qt::NoBrush );

        painter->drawEllipse( QPointF( 0, 0 ), r, r );

        break;
    }
    case GB::PointPaintMode::Selected :
    {
        painter->setPen( QPen( Qt::lightGray ) );
        painter->setBrush( QBrush( Qt::lightGray ) );

        QRectF pointRect( - sm_radius,
                          - sm_radius,
                          2 * sm_radius,
                          2 * sm_radius );

        painter->drawEllipse( pointRect );

        break;
    }
    }

    GB::PushedBuildingButton const pushedBuildButton = InteractionObjects::buildingButtonsData.GetPushedBuildingButton();
    if ( InteractionObjects::userActionMode == GB::UserActionMode::Building &&
            pushedBuildButton == GB::PushedBuildingButton::DrawPolygon &&
            GSBuilders::Objects::GetPolygonBuilder().IsItStartPoint( this ) )
    {
        if ( option->state & QStyle::State_MouseOver )
        {
            qreal r = sm_radius * 4;

            painter->setPen( QPen( QBrush( Qt::darkGreen ), 2 ) );
            painter->setBrush( Qt::NoBrush );

            painter->drawEllipse( QPointF( 0, 0 ), r, r );

        }
    }
}

QRectF GraphicsPoint::boundingRect() const
{
    return QRectF( - sm_radius - 2,
                   - sm_radius - 2,
                   2 * sm_radius + 4, 2 * sm_radius + 4 );
}

QPainterPath GraphicsPoint::shape() const
{
    QPainterPath path;
    path.addEllipse( boundingRect() );

    return path;
}



void GraphicsPoint::SetPosition( QPointF const & pos )
{
    m_position = pos;

    setPos( m_position );
}


/*
void GraphicsPoint::AddLineAsDefined( GraphicsLine * line )
{
    m_definedLines.push_back( line );
    //m_containingLines.push_back( line );
}
*/
/*
void GraphicsPoint::AddLineAsNondefined( GraphicsLine * line )
{
    m_nondefinedLines.push_back( line );
    //m_containingLines.push_back( line );
}
*/
/*
void GraphicsPoint::AddCircleAsDefined( GraphicsCircle * c, GPM::CircleMovingGuidePointType type )
{
    m_definedCirclesWithData.push_back( new GPM::CircleWithGuidePointType( c, type ) );
}
*/

/*
void GraphicsPoint::AddCircleAsNondefined( GraphicsCircle * c )
{
    m_nondefinedCircles.push_back( c );
    //m_nondefinedCirclesWithAngles.push_back( CircleWithAngle( c, Math::GetPolarAngleOfVector( QLineF( c->GetCenterPosition(), m_center ) ) ) );
}
*/


/*
void GraphicsPoint::RemoveDefinedLineSegment( GraphicsLineSegment * ls )
{
	if ( !m_definedLineSegments.contains( ls ) )
	{
		throw Exception( "GBPoint::RemoveDefinedLineSegment : passed line segment is not defined for this point." );
	}

	m_definedLineSegments.removeOne( ls );
}
*/
/*
void GraphicsPoint::RemoveNondefinedLineSegment( GraphicsLineSegment * ls )
{
	if ( !m_nondefinedLineSegments.contains( ls ) )
	{
		throw Exception( "GBPoint::RemoveNondefinedLineSegment : passed line segment is not nondefined for this point." );
	}

	m_nondefinedLineSegments.removeOne( ls );
}
*/


/*
bool GraphicsPoint::IsMainOrigin() const
{
    return m_center.isNull();
}
*/


/*
bool GraphicsPoint::operator ==( GraphicsPoint const & other ) const
{
    return ( GetPosition() == other.GetPosition() );
}
*/

bool GraphicsPoint::operator<( GraphicsPoint const & other ) const
{
    return GBHelper::operator <( GetPosition(), other.GetPosition() );
}

/*
bool GraphicsPoint::operator <=( GraphicsPoint const & other ) const
{
    return GBHelper::operator <=( GetPosition(), other.GetPosition() );
}
*/


/*
void GPoint::MoveAfter( GPoint * guidePoint, GLine * joiningLine )
{
    if ( !joiningLine->ContainsPointAsDefining( guidePoint ) )
    {
        // throw
    }

    GPoint * secondDefPoint = joiningLine->DefiningPoints().GetOtherItem( guidePoint );

    qreal const d = Math::GetDistance( secondDefPoint->Center(), GetOldPointPosition( joiningLine, this ) );

    QPointF const newPos = Math::GetPointOnLineSegment( secondDefPoint->Center(), guidePoint->Center(), d );

    SetCenter( newPos );
}
*/



void GraphicsPoint::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    //this->installSceneEventFilter(0);

    if ( InteractionObjects::userActionMode ==
            GB::UserActionMode::PolygonSelection )
    {
        InteractionObjects::TwoPolygonsSelecter & selecter = InteractionObjects::GetTwoPolygonsSelecter();

        if ( selecter.PolygonsAreSelected() )
        {
            selecter.SetPoint( this );

            SetPaintMode( GB::PointPaintMode::AroundCircle );
            update();

            emit static_cast< PlaneScene * >( scene() )->MovingLineSegmentsShowingPrepared();
            emit static_cast< PlaneScene * >( scene() )->CharacteristicFieldShowingPrepared();
        }

        return;
    }

    InteractionObjects::userActionMode = GB::UserActionMode::PointMoving;

    if ( event->button() == Qt::RightButton )
    {
        m_pressPlace = event->scenePos() - m_position;

        try
        {
            GPM::Objects::GetMovingManager().CalculateMovingData( this );
        }
        catch ( Exception const & exc )
        {
            qDebug() << exc.GetMessage();
        }
    }


    GraphicsShapesBSEHandlers::HandlerForPoint & bseHandler = GraphicsShapesBSEHandlers::Objects::GetHandlerForPoint();
    bseHandler.SetPoint( this );
    switch ( InteractionObjects::buildingButtonsData.GetPushedBuildingButton() )
    {
    case GB::PushedBuildingButton::AddPoint :
    {
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
        if ( event->button() == Qt::LeftButton )
        {
            bseHandler.HandleLineSegmentMidpointAdding( event );
        }

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

    setCursor( QCursor( Qt::ClosedHandCursor ) );
}

void GraphicsPoint::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    /*
    // Moving must be bounded by scene rect.
    if ( !QRectF( QPointF( 0, 0 ), GB::mainWindowGeometryData.GetPlaneViewSize() )
         .marginsRemoved( QMarginsF( 0, 0, 2 * GBPoint::GetRadius(), 2 * GBPoint::GetRadius() ) )
         .contains( event->scenePos() ) )
    {
        return;
    }
    */

    if ( InteractionObjects::userActionMode == GB::UserActionMode::PolygonSelection )
    {
        return;
    }

    if ( event->buttons() == Qt::RightButton &&
         GPM::Objects::GetMovingManager().GetMovingData( this )->MovingIsAllowed() )
    {
        GPM::Objects::GetMovingManager().HandleMoveEvent( this, event );
    }
    else
    {
        return;
    }

    setCursor( QCursor( Qt::ClosedHandCursor ) );
}

void GraphicsPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
    InteractionObjects::userActionMode = GB::UserActionMode::Building;

    GPM::Objects::GetMovingManager().HandleReleaseEvent( this, event );

    setCursor( QCursor( Qt::OpenHandCursor ) );
}



void GraphicsPoint::SetOldPositionAsCurrent()
{
	m_oldPosition = m_position;
}

/*
void GraphicsPoint::CalculateMovingData()
{
	if ( GetNondefinedShapesCount() > 1 )
    {
        m_movingData->DoNotAllowMoving();

        return;
    }

	QList< GPM::MovingPointGroup > groups_1;
	foreach ( GraphicsLine * line, m_definedLines )
	{
		GPM::MovingPointGroup group( line, this );
		group.SetPoints( line->GetNondefiningPoints() );

        groups_1.push_back( group );
	}
	foreach ( GraphicsLineSegment * s, m_definedLineSegments )
	{
		GPM::MovingPointGroup group( s, this );
		group.SetPoints( s->GetInteriorPoints() );

		groups_1.push_back( group );
	}
	foreach ( GPM::CircleWithGuidePointType * pair, m_definedCirclesWithData )
    {
        GraphicsCircle * c = pair->m_circle;
        GPM::MovingPointGroup group( c, pair->m_guideType, this );
        group.SetPoints( c->GetNondefiningPoints() );

        groups_1.push_back( group );
    }

    QList< GPM::MovingPointGroup > allGroups;
    QList< GPM::MovingPointGroup > groups_2;
    bool allMovingPointsAreFound = false;
    while ( !allMovingPointsAreFound )
    {
		uint const n = groups_1.size();

		uint i = 0;
		foreach ( GPM::MovingPointGroup const & group, groups_1 )
        {
            if ( group.IsEmpty() )
            {
                ++i;
            }

			foreach ( GraphicsPoint * point, group.GetPoints() )
			{
				foreach ( GraphicsLine * line, point->GetDefinedLines() )
                {
                    GPM::MovingPointGroup group( line, point );
					group.SetPoints( line->GetNondefiningPoints() );

                    groups_2.push_back( group );
				}
				foreach ( GraphicsLineSegment * s, point->m_definedLineSegments )
				{
					GPM::MovingPointGroup group( s, point );
					group.SetPoints( s->GetInteriorPoints() );

					groups_2.push_back( group );
				}
				foreach ( GPM::CircleWithGuidePointType * pair, point->m_definedCirclesWithData )
                {
                    GraphicsCircle * c = pair->m_circle;
                    GPM::MovingPointGroup group( c, pair->m_guideType, point );
                    group.SetPoints( c->GetNondefiningPoints() );

                    groups_1.push_back( group );
                }
			}
        }

		allGroups.append( groups_1 );
		groups_1 = groups_2;
		groups_2.clear();

        if ( i == n )
        {
            allMovingPointsAreFound = true;
		}
	}
	m_movingData->SetGroups( allGroups );

	m_movingData->SetStartPositionData();
}
*/

/*
void GraphicsPoint::CalculateAngleInNondefinedCircles()
{
    for ( QList< CircleWithAngle >::iterator it = m_nondefinedCirclesWithAngles.begin();
          it != m_nondefinedCirclesWithAngles.end(); ++it )
    {
        GraphicsCircle * c = ( *it ).m_circle;

        ( *it ).m_angle = Math::GetPolarAngleOfVector( QLineF( c->GetCenterPosition(), m_center ) );
    }
}
*/
