#include "GraphicsShapes/helpers.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "interactionobjects.h"
#include "mathnamespace.h"

#include <QPointF>



GraphicsLineSegment * GSHelpers::GetContainingSegment(
    std::list< GraphicsLineSegment * > const & segments,
    GraphicsPoint * p )
{
    foreach ( GraphicsLineSegment * ls, segments )
    {
        if ( ( *( ls->p1() ) < *p && *p < * ( ls->p2() ) ) ||
                ( *( ls->p2() ) < *p && *p < * ( ls->p1() ) ) )
        {
            return ls;
        }
    }

    //throw
}



double GSHelpers::GetDistance( GraphicsPoint const * p1, GraphicsPoint const * p2 )
{
    return Math::GetDistance( p1->GetPosition(), p2->GetPosition() );
}



QPointF GSHelpers::GetMidpoint( GraphicsPoint * p1, GraphicsPoint * p2 )
{
    QLineF const l( p1->GetPosition(), p2->GetPosition() );

    return l.center();
}



Math::TwoLineSegmentsIntersectionData GSHelpers::GetIntersectionData( GraphicsLineSegment * ls1, GraphicsLineSegment * ls2 )
{
    switch ( InteractionObjects::userActionMode )
    {
    case GB::UserActionMode::None :
    {
        break;
    }
    case GB::UserActionMode::Building :
    {
        QPointF * intersectionPoint = new QPointF;

        QLineF const lf1 = ls1->GetQLineF();
        QLineF const lf2 = ls2->GetQLineF();

        switch ( lf1.intersect( lf2, intersectionPoint ) )
        {
        case QLineF::BoundedIntersection :
        {
            return Math::TwoLineSegmentsIntersectionData(
                       Math::TwoLineSegmentsLocation::Intersect,
                       intersectionPoint );
        }
        case QLineF::UnboundedIntersection :
        {
            return Math::TwoLineSegmentsIntersectionData(
                       Math::TwoLineSegmentsLocation::ExtensionIntersect,
                       nullptr );
        }
        case QLineF::NoIntersection :
        {
            return Math::TwoLineSegmentsIntersectionData(
                       Math::TwoLineSegmentsLocation::Parallel,
                       nullptr );
        }
        }

        break;
    }
    case GB::UserActionMode::PointMoving :
    {
        QPointF * intersectionPoint = new QPointF;

        QLineF const lf1 = ls1->GetQLineF();
        QLineF const lf2 = ls2->GetQLineF();

        switch ( lf1.intersect( lf2, intersectionPoint ) )
        {
        case QLineF::BoundedIntersection :
        {
            return Math::TwoLineSegmentsIntersectionData(
                       Math::TwoLineSegmentsLocation::Intersect,
                       intersectionPoint );
        }
        case QLineF::UnboundedIntersection :
        {
            GraphicsLine * l1 = new GraphicsLine( ls1->p1(), ls1->p2() );
            GraphicsLine * l2 = new GraphicsLine( ls2->p1(), ls2->p2() );

            Math::TwoLinesIntersectionData const data = GSHelpers::GetIntersectionData( l1, l2 );
            intersectionPoint = new QPointF( data.GetIntersectionPoint() );

            return Math::TwoLineSegmentsIntersectionData(
                       Math::TwoLineSegmentsLocation::ExtensionIntersect,
                       intersectionPoint );
        }
        case QLineF::NoIntersection :
        {
            return Math::TwoLineSegmentsIntersectionData(
                       Math::TwoLineSegmentsLocation::Parallel,
                       nullptr );
        }
        }

        break;
    }
    }
}

Math::TwoLinesIntersectionData GSHelpers::GetIntersectionData( GraphicsLine * l1, GraphicsLine * l2 )
{
    QPointF * intersectionPoint = new QPointF;

    GBPointPair const ends1 = l1->GetExtensionEnds();
    GBPointPair const ends2 = l2->GetExtensionEnds();
    QLineF const lf1( ends1.GetFirst()->GetPosition(), ends1.GetSecond()->GetPosition() );
    QLineF const lf2( ends2.GetFirst()->GetPosition(), ends2.GetSecond()->GetPosition() );

    switch ( lf1.intersect( lf2, intersectionPoint ) )
    {
    case QLineF::UnboundedIntersection :
    {
        break;
    }
    case QLineF::NoIntersection :
    {
        return Math::TwoLinesIntersectionData(
                   Math::TwoLinesLocation::Parallel,
                   nullptr );
    }
    case QLineF::BoundedIntersection :
    {
        return Math::TwoLinesIntersectionData(
                   Math::TwoLinesLocation::Intersect,
                   intersectionPoint );
    }
    }
}

Math::LineWithLineSegmentIntersectionData GSHelpers::GetIntersectionData( GraphicsLine * l, GraphicsLineSegment * ls )
{
    QPointF * intersectionPoint = new QPointF;

    GBPointPair const ends = l->GetExtensionEnds();
    QLineF const lf1( ends.GetFirst()->GetPosition(), ends.GetSecond()->GetPosition() );
    QLineF const lf2 = ls->GetQLineF();

    switch ( lf1.intersect( lf2, intersectionPoint ) )
    {
    case QLineF::BoundedIntersection :
    {
        return Math::LineWithLineSegmentIntersectionData(
                   Math::LineWithLineSegmentLocation::Intersect,
                   intersectionPoint );

        break;
    }
    case QLineF::UnboundedIntersection :
    {
        return Math::LineWithLineSegmentIntersectionData(
                   Math::LineWithLineSegmentLocation::ExtensionIntersect,
                   intersectionPoint );

        break;
    }
    case QLineF::NoIntersection :
    {
        return Math::LineWithLineSegmentIntersectionData(
                   Math::LineWithLineSegmentLocation::Parallel,
                   0 );

        break;
    }
    }
}

Math::CircleWithLineIntersectionData GSHelpers::GetIntersectionData( GraphicsCircle * circle, GraphicsLine * l )
{
    return Math::GetIntersectionData( circle->GetCircle(), l->GetLine() );
}

Math::CircleWithLineSegmentIntersectionData GSHelpers::GetIntersectionData( GraphicsCircle * circle, GraphicsLineSegment * ls )
{
    return Math::GetIntersectionData( circle->GetCircle(),
                                      GeomShapes::LineSegment( ls->p1()->GetPosition(), ls->p2()->GetPosition() ) );
}

Math::TwoCirclesIntersectionData GSHelpers::GetIntersectionData( GraphicsCircle * c1, GraphicsCircle * c2 )
{
    return Math::GetIntersectionData( c1->GetCircle(), c2->GetCircle() );
}



QPointF GSHelpers::GetPerpendicularBasePoint( QPointF const & pos, GraphicsLine * line )
{
    QLineF const lineVector( line->GetDefiningPoints().GetFirst()->GetPosition(), line->GetDefiningPoints().GetSecond()->GetPosition() );
    QLineF const vectorToPos( line->GetDefiningPoints().GetFirst()->GetPosition(), pos );

    qreal a = lineVector.angleTo( vectorToPos );
    Math::Angle const angle( a, Math::AngleMeasure::Degrees );

    qreal const d = vectorToPos.length() * cos( angle.GetRadians() );

    return QPointF( lineVector.pointAt( d / lineVector.length() ) );
}

QPointF GSHelpers::GetPerpendicularBasePoint( QPointF const & pos, GraphicsLineSegment * ls )
{
    QLineF const lineVector( ls->p1()->GetPosition(), ls->p2()->GetPosition() );
    QLineF const vectorToPos( ls->p1()->GetPosition(), pos );

    qreal a = lineVector.angleTo( vectorToPos );
    Math::Angle const angle( a, Math::AngleMeasure::Degrees );

    qreal const d = vectorToPos.length() * cos( angle.GetRadians() );

    return QPointF( lineVector.pointAt( d / lineVector.length() ) );
}



QPointF GSHelpers::GetPointOnCircle( QPointF const & pos, GraphicsCircle * c )
{
    QLineF const l( c->GetCenterPosition(), pos );

    return l.pointAt( c->GetRadius() / l.length() );
}



Math::Vector GSHelpers::GetVector( GraphicsLine * line )
{
    return Math::Vector(
               line->GetDefiningPoints().GetFirst()->GetPosition(),
               line->GetDefiningPoints().GetSecond()->GetPosition() );
}

Math::Vector GSHelpers::GetVector( GraphicsLineSegment * ls )
{
    return Math::Vector(
               ls->p1()->GetPosition(),
               ls->p2()->GetPosition() );
}



std::pair< QPointF const, QPointF const > GSHelpers::GetTangentPoints( QPointF const & sourcePoint, GraphicsCircle * circle )
{
    return Math::GetTangentPoints( sourcePoint, circle->GetCircle() );
}



bool GSHelpers::PointIsOutsideCircle( QPointF const & scenePos, GraphicsCircle * circle )
{
    return Math::PointIsOutsideCircle( scenePos, circle->GetCircle() );
}
