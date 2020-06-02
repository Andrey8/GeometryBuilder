#include "GraphicsPointMoving/movingpointsgroup.h"
#include "DebugHelpers/exception.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"
#include "GraphicsShapes/helpers.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"
#include "GraphicsShapesIntersections/objects.h"



GPM::MovingPointGroup::MovingPointGroup( GraphicsLine * jointLine, GraphicsPoint * guidePoint )
    : m_jointShape( JointShape::Line ),
      m_jointLine( jointLine ),
      m_jointLineSegment( nullptr ),
      m_jointCircle( nullptr ),
      m_guidePoint( guidePoint )
{
    if ( !jointLine->ContainsPointAsDefining( guidePoint ) )
    {
        throw Exception( "GBPoint::MovingPointGroup creation with line : guide point doesn't define that line." );
    }
}

GPM::MovingPointGroup::MovingPointGroup( GraphicsLineSegment * jointLineSegment, GraphicsPoint * guidePoint )
    : m_jointShape( JointShape::LineSegment ),
      m_jointLine( nullptr ),
      m_jointLineSegment( jointLineSegment ),
      m_jointCircle( nullptr ),
      m_guidePoint( guidePoint )
{
    if ( jointLineSegment->p1() != guidePoint && jointLineSegment->p2() != guidePoint )
    {
        throw Exception( "GBPoint::MovingPointGroup creation with line segment : guide point is not end of that segment." );
    }
}

GPM::MovingPointGroup::MovingPointGroup( GraphicsCircle * c, GPM::CircleMovingGuidePointType type, GraphicsPoint * p )
    : m_jointShape( JointShape::Circle ),
      m_jointLine( nullptr ),
      m_jointLineSegment( nullptr ),
      m_jointCircle( c ),
      m_guidePointType( type ),
      m_guidePoint( p )
{

}



void GPM::MovingPointGroup::Move()
{
    switch ( m_jointShape )
    {
    case JointShape::Line :
    {
        GraphicsPoint * secondDefPoint = m_jointLine->GetDefiningPoints().GetOtherItem( m_guidePoint );
        qreal const l = Math::GetDistance( secondDefPoint->GetPosition(), m_guidePoint->GetOldPosition() );

        foreach ( GraphicsPoint * point, m_points )
        {
            if ( GSI::GetIntersectionManager().IsIntersectionPoint( point ) )
            {
                continue;
            }

            switch ( GetPositionTypeOf( point ) )
            {
            case PointPositionType::InsideDefiningSegemnt :
            {
                point->SetPosition( Math::GetPointOnLineSegment( secondDefPoint->GetPosition(), m_guidePoint->GetPosition(), GetDistanceFromRotationCenterTo( point ) ) );

                break;
            }
            case PointPositionType::OutDefiningSegmentAndNearerToGuide :
            {
                point->SetPosition( Math::GetPointOutOfLineSegment( m_guidePoint->GetPosition(), secondDefPoint->GetPosition(), GetDistanceFromRotationCenterTo( point ) - l ) );

                break;
            }
            case PointPositionType::OutDefiningSegmentAndNearerToNotGuide :
            {
                point->SetPosition( Math::GetPointOutOfLineSegment( secondDefPoint->GetPosition(), m_guidePoint->GetPosition(), GetDistanceFromRotationCenterTo( point ) ) );

                break;
            }
            }
        }

        break;
    }
    case JointShape::LineSegment :
    {
        GBPointPair ends( m_jointLineSegment->p1(), m_jointLineSegment->p2() );
        GraphicsPoint * otherEnd = ends.GetOtherItem( m_guidePoint );
        qreal const l = Math::GetDistance( otherEnd->GetPosition(), m_guidePoint->GetOldPosition() );

        foreach ( GraphicsPoint * point, m_points )
        {
            if ( GSI::GetIntersectionManager().IsIntersectionPoint( point ) )
            {
                continue;
            }

            //DebugHelper::ShowTextInInput( "UUUUUU" );

            switch ( GetPositionTypeOf( point ) )
            {
            case PointPositionType::InsideDefiningSegemnt :
            {
                point->SetPosition( Math::GetPointOnLineSegment( otherEnd->GetPosition(), m_guidePoint->GetPosition(), GetDistanceFromRotationCenterTo( point ) ) );

                break;
            }
            case PointPositionType::OutDefiningSegmentAndNearerToGuide :
            {
                point->SetPosition( Math::GetPointOutOfLineSegment( m_guidePoint->GetPosition(), otherEnd->GetPosition(), GetDistanceFromRotationCenterTo( point ) - l ) );

                break;
            }
            case PointPositionType::OutDefiningSegmentAndNearerToNotGuide :
            {
                point->SetPosition( Math::GetPointOutOfLineSegment( otherEnd->GetPosition(), m_guidePoint->GetPosition(), GetDistanceFromRotationCenterTo( point ) ) );

                break;
            }
            }
        }

        break;
    }
    case JointShape::Circle :
    {
        GraphicsCircle * c = m_jointCircle;
        QPointF const circleCenterPos = c->GetCenterPosition();

        switch ( m_guidePointType )
        {
        case CircleMovingGuidePointType::Center :
        {
            switch ( c->GetDefiningObjectsType() )
            {
            case GraphicsCircle::DefiningObjectsType::OnlyCenter :
            {
                foreach ( GraphicsPoint * p, m_points )
                {
                    QLineF l( circleCenterPos, p->GetPosition() );

                    p->SetPosition( m_guidePoint->GetPosition() + QPointF( l.dx(), l.dy() ) );
                }

                c->SetCenter( m_guidePoint );

                break;
            }
            case GraphicsCircle::DefiningObjectsType::CenterAndPointOnCircle :
            {
                qreal const r = Math::GetDistance( m_guidePoint->GetPosition(), c->GetDefiningObjects().GetPointOnCircle()->GetPosition() );

                c->SetCenter( m_guidePoint );
                c->SetRadius( r );

                QPointF const guidePos = m_guidePoint->GetPosition();
                foreach ( GraphicsPoint * p, m_points )
                {
                    try
                    {
                        //Math::Angle const a = p->GetAngleInNondefinedCircle( m_jointCircle );
                        Math::Angle const a = Helpers::Objects::GetConstructionData().GetAngleInNondefinedCircle( p, m_jointCircle );
                        p->SetPosition( guidePos + QPointF( r * cos( a.GetRadians() ), - r * sin( a.GetRadians() ) ) );
                    }
                    catch ( Exception const & exc )
                    {
                        qDebug() << exc.GetMessage() ;
                    }
                }

                break;
            }
            case GraphicsCircle::DefiningObjectsType::ThreePoints :
            {
                break;
            }
            }

            break;
        }
        case CircleMovingGuidePointType::OnlyPointOnCircle :
        {
            foreach ( GraphicsPoint * p, m_points )
            {
                QLineF l( circleCenterPos, p->GetPosition() );

                p->SetPosition( l.pointAt( c->GetRadius() / l.length() ) );
            }

            c->SetRadius( Math::GetDistance( c->GetCenterPosition(), m_guidePoint->GetPosition() ) );

            break;
        }
        case CircleMovingGuidePointType::OneOfThreePointsOnCircle :
        {
            CircleDefiningObjects const & defObjects = c->GetDefiningObjects();
            QPointF const pos1 = defObjects.GetP1()->GetPosition();
            QPointF const pos2 = defObjects.GetP2()->GetPosition();
            QPointF const pos3 = defObjects.GetP3()->GetPosition();

            QPointF const centerPos = Math::GetCircumcircleCenter( pos1, pos2, pos3 );
            qreal const r = Math::GetCircumcircleRadius( pos1, pos2, pos3 );
            c->SetCenterPos( centerPos );
            c->SetRadius( r );

            foreach ( GraphicsPoint * p, m_points )
            {
                //Math::Angle const a = p->GetAngleInNondefinedCircle( m_jointCircle );
                Math::Angle const a = Helpers::Objects::GetConstructionData().GetAngleInNondefinedCircle( p, m_jointCircle );
                p->SetPosition( centerPos + QPointF( r * cos( a.GetRadians() ), - r * sin( a.GetRadians() ) ) );
            }

            break;
        }
        }

        break;
    }
    }

    foreach ( GraphicsPoint * p, m_points )
    {
        if ( GSI::GetIntersectionManager().IsIntersectionPoint( p ) )
        {
            try
            {
                switch ( GSI::GetIntersectionManager().GetIntersectionType( p ) )
                {
                case GSI::IntersectionType::LineWithLine :
                {
                    std::list< GraphicsLine * > lines = Helpers::Objects::GetConstructionData().GetNondefinedLines( p );

                    if ( lines.size() != 2 )
                    {
                        throw Exception( "GBPoint::MovingPointGroup::Move : nondefined lines count for intersection point is not 2." );
                    }

                    GraphicsLine * l1 = lines.front();
                    GraphicsLine * l2 = lines.back();
                    Math::TwoLinesIntersectionData const data = GSHelpers::GetIntersectionData( l1, l2 );
                    p->SetPosition( data.GetIntersectionPoint() );

                    break;
                }
                case GSI::IntersectionType::LineSegmentsWithLineSegment :
                {
                    std::list< GraphicsLineSegment * > segments = Helpers::Objects::GetConstructionData().GetNondefinedLineSegments( p );

                    if ( segments.size() != 2 )
                    {
                        throw Exception( "GBPoint::MovingPointGroup::Move : nondefined line segments count for intersection point is not 2." );
                    }

                    GraphicsLineSegment * ls1 = segments.front();
                    GraphicsLineSegment * ls2 = segments.back();
                    Math::TwoLineSegmentsIntersectionData const data = GSHelpers::GetIntersectionData( ls1, ls2 );
                    p->SetPosition( data.GetIntersectionPoint() );

                    break;
                }
                case GSI::IntersectionType::LineWithLineSegment :
                {
                    GraphicsLine * l = Helpers::Objects::GetConstructionData().GetNondefinedLines( p ).front();
                    GraphicsLineSegment * ls = Helpers::Objects::GetConstructionData().GetNondefinedLineSegments( p ).front();
                    Math::LineWithLineSegmentIntersectionData const data =
                        GSHelpers::GetIntersectionData( l, ls );
                    p->SetPosition( data.GetIntersectionPoint() );

                    break;
                }
                case GSI::IntersectionType::CircleWithLine :
                {
                    GraphicsCircle * c = Helpers::Objects::GetConstructionData().GetNondefinedCircles( p ).front();
                    GraphicsLine * l = Helpers::Objects::GetConstructionData().GetNondefinedLines( p ).front();
                    Math::CircleWithLineIntersectionData const data =
                        GSHelpers::GetIntersectionData( c, l );

                    Math::CircleWithLineLocation const newLocation = data.GetLocation();
                    switch ( newLocation )
                    {
                    case Math::CircleWithLineLocation::IntersectionInTwoPoints :
                    {
                        std::pair< QPointF const, QPointF const > const pair =
                            data.GetIntersectionPointsPair();
                        QPointF const p1 = pair.first;
                        QPointF const p2 = pair.second;

                        GraphicsPoint * lp1 = l->GetDefiningPoints().GetFirst();
                        GraphicsPoint * lp2 = l->GetDefiningPoints().GetSecond();

                        GSI::CircleWithLineIntersectionData const data2 = GSI::GetIntersectionManager().GetIntersectionData( c, l );
                        Math::CircleWithLineLocation const oldLocation = data2.GetLocation();
                        switch ( oldLocation )
                        {
                        case Math::CircleWithLineLocation::IntersectionInTwoPoints :
                        {
                            std::vector< GraphicsPoint * > const intersPoints =  data2.GetIntersectionPoints();
                            GraphicsPoint * ip1 = intersPoints[ 0 ];
                            GraphicsPoint * ip2 = intersPoints[ 1 ];
                            GBPointPair const ippair( ip1, ip2 );
                            GraphicsPoint * otherIntersPoint = ippair.GetOtherItem( p );

                            double const scalarProduct1 = Math::Vector( lp1->GetOldPosition(), lp2->GetOldPosition() ) * Math::Vector( p->GetOldPosition(), otherIntersPoint->GetOldPosition() );
                            double const scalarProduct2 = Math::Vector( lp1->GetPosition(), lp2->GetPosition() ) * Math::Vector( p1, p2 );
                            if ( Math::GetSignum( scalarProduct1 ) == Math::GetSignum( scalarProduct2 ) )
                            {
                                p->SetPosition( p1 );
                            }
                            else
                            {
                                p->SetPosition( p2 );
                            }

                            break;
                        }
                        case Math::CircleWithLineLocation::Touching :
                        {

                        }
                        case Math::CircleWithLineLocation::NoIntersection :
                        {

                        }
                        }

                        break;
                    }
                    case Math::CircleWithLineLocation::Touching :
                    {


                        break;
                    }
                    case Math::CircleWithLineLocation::NoIntersection :
                    {


                        break;
                    }
                    }

                    break;
                }
                case GSI::IntersectionType::CircleWithLineSegment :
                {
                    GraphicsCircle * c = Helpers::Objects::GetConstructionData().GetNondefinedCircles( p ).front();
                    GraphicsLineSegment * ls = Helpers::Objects::GetConstructionData().GetNondefinedLineSegments( p ).front();
                    Math::CircleWithLineSegmentIntersectionData const data =
                        GSHelpers::GetIntersectionData( c, ls );

                    Math::CircleWithLineSegmentLocation const newLocation = data.GetLocation();
                    switch ( newLocation )
                    {
                    case Math::CircleWithLineSegmentLocation::IntersectionInTwoPoints :
                    {


                        break;
                    }
                    case Math::CircleWithLineSegmentLocation::IntersectionInOnePoint :
                    {
                        GSI::CircleWithLineSegmentIntersectionData const data2 = GSI::GetIntersectionManager().GetIntersectionData( c, ls );
                        Math::CircleWithLineSegmentLocation const oldLocation = data2.GetLocation();
                        switch ( oldLocation )
                        {
                        case Math::CircleWithLineSegmentLocation::IntersectionInOnePoint :
                        {
                            p->SetPosition( data.GetOnlyIntersectionPoint() );

                            break;
                        }
                        case Math::CircleWithLineSegmentLocation::IntersectionInTwoPoints :
                        {


                            break;
                        }
                        case Math::CircleWithLineSegmentLocation::Touching :
                        {


                            break;
                        }
                        case Math::CircleWithLineSegmentLocation::NoIntersection :
                        {


                            break;
                        }
                        }

                        break;
                    }



                    }

                    break;
                }
                }
            }
            catch ( Exception const & exc )
            {
                qDebug() << exc.GetMessage() ;
            }
        }
    }
}



void GPM::MovingPointGroup::SetStartPositionData()
{
    m_pointsWithStartPosData.clear();

    switch ( m_jointShape )
    {
    case JointShape::Line :
    {
        GraphicsPoint * otherDefiningPoint = m_jointLine->GetDefiningPoints().GetOtherItem( m_guidePoint );

        foreach ( GraphicsPoint * point, m_points )
        {
            if ( GSI::GetIntersectionManager().IsIntersectionPoint( point ) )
            {
                m_intersectionPoints.push_back( point );

                continue;
            }

            qreal const d = Math::GetDistance( point->GetOldPosition(), otherDefiningPoint->GetPosition() );

            if ( *m_guidePoint < *otherDefiningPoint )
            {
                if ( *point < *m_guidePoint )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( point, PointPositionType::OutDefiningSegmentAndNearerToGuide, d ) );
                }
                else if ( *m_guidePoint < *point && *point < *otherDefiningPoint )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( point, PointPositionType::InsideDefiningSegemnt, d ) );
                }
                else if ( *otherDefiningPoint < *point )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( point, PointPositionType::OutDefiningSegmentAndNearerToNotGuide, d ) );
                }
            }
            else if ( *otherDefiningPoint < *m_guidePoint )
            {
                if ( *point < *otherDefiningPoint )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( point, PointPositionType::OutDefiningSegmentAndNearerToNotGuide, d ) );
                }
                else if ( *otherDefiningPoint < *point && *point < *m_guidePoint )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( point, PointPositionType::InsideDefiningSegemnt, d ) );
                }
                else if ( *m_guidePoint < *point )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( point, PointPositionType::OutDefiningSegmentAndNearerToGuide, d ) );
                }
            }
        }

        break;
    }
    case JointShape::LineSegment :
    {
        GBPointPair ends( m_jointLineSegment->p1(), m_jointLineSegment->p2() );
        GraphicsPoint * otherEnd = ends.GetOtherItem( m_guidePoint );

        foreach ( GraphicsPoint * p, m_points )
        {
            if ( GSI::GetIntersectionManager().IsIntersectionPoint( p ) )
            {
                m_intersectionPoints.push_back( p );

                continue;
            }

            qreal const d = Math::GetDistance( p->GetOldPosition(), otherEnd->GetPosition() );

            if ( *m_guidePoint < *otherEnd )
            {
                if ( *p < *m_guidePoint )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( p, PointPositionType::OutDefiningSegmentAndNearerToGuide, d ) );
                }
                else if ( *m_guidePoint < *p && *p < *otherEnd )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( p, PointPositionType::InsideDefiningSegemnt, d ) );
                }
                else if ( *otherEnd < *p )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( p, PointPositionType::OutDefiningSegmentAndNearerToNotGuide, d ) );
                }
            }
            else if ( *otherEnd < *m_guidePoint )
            {
                if ( *p < *otherEnd )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( p, PointPositionType::OutDefiningSegmentAndNearerToNotGuide, d ) );
                }
                else if ( *otherEnd < *p && *p < *m_guidePoint )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( p, PointPositionType::InsideDefiningSegemnt, d ) );
                }
                else if ( *m_guidePoint < *p )
                {
                    m_pointsWithStartPosData.push_back( PointWithStartPosData( p, PointPositionType::OutDefiningSegmentAndNearerToGuide, d ) );
                }
            }
        }

        break;
    }
    }
}



GPM::MovingPointGroup::PointPositionType GPM::MovingPointGroup::GetPositionTypeOf( GraphicsPoint * point ) const
{
    if ( !m_points.contains( point ) )
    {
        // throw
    }

    foreach ( PointWithStartPosData data, m_pointsWithStartPosData )
    {
        if ( data.m_point == point )
        {
            return data.m_posType;
        }
    }
}

qreal GPM::MovingPointGroup::GetDistanceFromRotationCenterTo( GraphicsPoint * point ) const
{
    if ( !m_points.contains( point ) )
    {
        // throw
    }

    foreach ( PointWithStartPosData data, m_pointsWithStartPosData )
    {
        if ( data.m_point == point )
        {
            return data.m_distanceToRotationCenter;
        }
    }
}



void GPM::MovingPointGroup::Clear()
{
    m_points.clear();

    m_jointLine = nullptr;
    m_guidePoint = nullptr;
}
