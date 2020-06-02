#include "mathnamespace.h"
#include "gvector.h"
#include "DebugHelpers/debughelper.h"
//#include "GraphicsShapes/graphicsline.h"
//#include "GraphicsShapes/graphicscircle.h"
#include "interactionobjects.h"
#include "DebugHelpers/exception.h"

#include <QLineF>
#include <QDebug>



int Math::GetSignum( double x )
{
    if ( x > 0 )
    {
        return 1;
    }
    else if ( x < 0 )
    {
        return -1;
    }

    return 0;
}

Math::Angle Math::Vector::GetRelativeAngle( Vector const & fixed, Math::CircleDirection baseOrientation, CircleDirection direction ) const
{
    switch ( direction )
    {
    case CircleDirection::Counterclockwise :
    {
        double const scal1 = ( *this ) * fixed;
        double const scal2 = ( *this ) * fixed.GetNormal( baseOrientation, CircleDirection::Clockwise );

        double const x = acos( scal1 / ( GetNorm() * fixed.GetNorm() ) );
        if ( scal1 >= 0 )
        {
            if ( scal2 >= 0 )
            {
                return Angle( x );
            }
            else
            {
                return Angle( 2 * M_PI - x );
            }
        }
        else
        {
            if ( scal2 >= 0 )
            {
                return Angle( x );
            }
            else
            {
                return Angle( 2 * M_PI - x );
            }
        }
    }
    case CircleDirection::Clockwise :
    {
        double const scal1 = ( *this ) * fixed;
        double const scal2 = ( *this ) * fixed.GetNormal( baseOrientation, CircleDirection::Counterclockwise );

        double const x = acos( scal1 / ( GetNorm() * fixed.GetNorm() ) );
        if ( scal1 >= 0 )
        {
            if ( scal2 >= 0 )
            {
                return Angle( x );
            }
            else
            {
                return Angle( 2 * M_PI - x );
            }
        }
        else
        {
            if ( scal2 >= 0 )
            {
                return Angle( x );
            }
            else
            {
                return Angle( 2 * M_PI - x );
            }
        }
    }
    }
}

bool Math::operator<( QPointF const & p1, QPointF const & p2 )
{
    if ( p1.x() < p2.x() )
    {
        return true;
    }
    else if ( p1.x() > p2.x() )
    {
        return false;
    }
    else
    {
        if ( p1.y() < p2.y() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

uint16_t Math::GetRemainder( int a, uint16_t b )
{
    int r = a % b;

    if ( r < 0 )
    {
        return ( b + r );
    }

    return r;
}

qreal Math::GetDistance( QPointF const & first, QPointF const & second )
{
    QLineF line( first, second );

    return line.length();
}

bool Math::PointsAreNearly( QPointF const & p1, QPointF const & p2, double d )
{
    return GetDistance( p1, p2 ) <= d;
}



QPointF Math::GetPointOnLine( QLineF const & line, qreal x )
{
    qreal x1 = line.p1().x();
    qreal y1 = line.p1().y();
    qreal x2 = line.p2().x();
    qreal y2 = line.p2().y();

    if ( x1 == x2 )
    {
        if ( x == x1 )
        {
            throw Exception( "Math::GetPointOnLine( QLineF const & line, qreal x ) : there are infinitely many such points." );
        }
        else
        {
            throw Exception( "Math::GetPointOnLine( QLineF const & line, qreal x ) : there are no such point." );
        }
    }

    qreal y = ( y2 - y1 ) * ( x - x1 ) / ( x2 - x1 ) + y1;

    return QPointF( x, y );
}



qreal Math::GetScalarProduct( QLineF const & v1, QLineF const & v2 )
{
    return ( v1.dx() * v2.dx() + v1.dy() * v2.dy() );
}

qreal Math::GetScalarProduct( GVector const & first, GVector const & second )
{
    return ( first.X() * second.X() + first.Y() * second.Y() );
}



Math::Angle Math::GetPolarAngleOfVector( QLineF const & v )
{
    QLineF const xAxis( 0, 0, 1, 0 );

    if ( GetScalarProduct( xAxis, v.normalVector() ) == 0 )
    {
        if ( GetScalarProduct( xAxis, v ) > 0 )
        {
            return Angle( 0 );
        }
        else
        {
            return Angle( 180, AngleMeasure::Degrees );
        }
    }
    else if ( GetScalarProduct( xAxis, v.normalVector() ) < 0 )
    {
        return Angle( xAxis.angle( v ), AngleMeasure::Degrees );
    }
    else
    {
        return Angle( 360 - xAxis.angle( v ), AngleMeasure::Degrees );
    }
}

Math::Angle Math::GetPolarAngleOfVector( GVector const & vector, CoordSystem const & system, PolarAngleOfVectorAlgorithm algorithm )
{
    if ( vector.IsNull() )
    {
        return Angle( 0 );
    }

    switch ( algorithm )
    {
    case WithQLineFAngles :
    {
        GVector const xAxisOrt = system.XAxisOrt();

        QLineF const vectorLine( 0, 0, vector.X(), vector.Y() );
        QLineF const xAxisLine( 0, 0, xAxisOrt.X(), xAxisOrt.Y() );

        qreal const a1 = vectorLine.angle();
        qreal const a2 = xAxisLine.angle();

        switch ( system.GetOrientation() )
        {
        case CoordSystem::CounterClockwise :
        {
            if ( a1 >= a2 )
            {
                return Angle( a1 - a2, AngleMeasure::Degrees );
            }
            else
            {
                return Angle( 360 - a2 + a1, AngleMeasure::Degrees );
            }
        }
        case CoordSystem::Clockwise :
        {
            if ( a2 >= a1 )
            {
                return Angle( a2 - a1, AngleMeasure::Degrees );
            }
            else
            {
                return Angle( 360 - a1 + a2, AngleMeasure::Degrees );
            }
        }
        }
    }
    case WithArccosine :
    {
        GVector const ex = system.XAxisOrt();
        GVector const ey = system.YAxisOrt();

        qreal const scalarProductWith_X_Ort = Math::GetScalarProduct( vector, ex );
        qreal const scalarProductWith_Y_Ort = Math::GetScalarProduct( vector, ey );

        qreal const a = acos( scalarProductWith_X_Ort / vector.Norm() );

        if ( scalarProductWith_X_Ort > 0 )
        {
            if ( scalarProductWith_Y_Ort > 0 )
            {
                return Angle( a );
            }
            else if ( scalarProductWith_Y_Ort < 0 )
            {
                return Angle( 2 * M_PI - a );
            }
            else
            {
                return Angle( 0 );
            }
        }
        else if ( scalarProductWith_X_Ort < 0 )
        {
            if ( scalarProductWith_Y_Ort > 0 )
            {
                return Angle( a );
            }
            else if ( scalarProductWith_Y_Ort < 0 )
            {
                return Angle( 2 * M_PI - a );
            }
            else
            {
                return Angle( M_PI );
            }
        }
        else
        {
            if ( scalarProductWith_Y_Ort > 0 )
            {
                return Angle( M_PI_2 );
            }
            else if ( scalarProductWith_Y_Ort < 0 )
            {
                return Angle( 3 * M_PI_2 );
            }
        }
    }
    default :
        break;
    }
}



GVector const Math::operator *( qreal k, GVector const & vector )
{
    return ( vector * k );
}



QPointF Math::GetPointFromPolarToRect( PointInPolar const & point )
{
    Angle const angle = point.GetAngle();
    qreal const r = point.GetRadius();

    return QPointF( r * cos( angle.GetRadians() ), r * sin( angle.GetRadians() ) );
}



Math::PointInPolar Math::GetMappedPointInPolarCoords( QPointF const & point, CoordSystem const & system )
{
    Angle const angle = GetPolarAngleOfVector( GVector( system.Origin(), point ), system, WithQLineFAngles );
    qreal const r = GetDistance( system.Origin(), point );

    return PointInPolar( angle, r );
}

QPointF Math::GetMappedPointFromMain( QPointF const & point, CoordSystem const & newSystem )
{
    PointInPolar mappedInPolar = GetMappedPointInPolarCoords( point, newSystem );

    Angle const angle = mappedInPolar.GetAngle();
    qreal const r = mappedInPolar.GetRadius();

    return QPointF( r * cos( angle.GetRadians() ), r * sin( angle.GetRadians() ) );
}

QPointF Math::GetMappedPointToMain( QPointF const & point, CoordSystem const & oldSystem )
{
    GVector const radiusVectorInMain = GVector( QPointF( 0, 0 ), oldSystem.Origin() ) + point.x() * oldSystem.XAxisOrt() + point.y() * oldSystem.YAxisOrt();

    return radiusVectorInMain.ToPoint();
}

QPointF Math::GetMappedPoint( QPointF const & point, CoordSystem const & oldSystem, CoordSystem const & newSystem )
{
    // ...
}



QPointF Math::GetPointOnLineSegment( QPointF const & p1, QPointF const & p2, qreal distanceToFirstPoint )
{
    QLineF const line( p1, p2 );

    qreal const t = distanceToFirstPoint / line.length();

    return line.pointAt( t );
}

QPointF Math::GetPointOutOfLineSegment( QPointF const & nearestEndPoint, QPointF const & p2, qreal distanceToFirstPoint )
{
    QLineF const line( nearestEndPoint, p2 );

    qreal const t = -distanceToFirstPoint / line.length();

    return line.pointAt( t );
}



QPointF Math::GetCircumcircleCenter( QPointF const & pos1, QPointF const & pos2, QPointF const & pos3 )
{
    qreal const x1 = pos1.x();
    qreal const y1 = pos1.y();
    qreal const x2 = pos2.x();
    qreal const y2 = pos2.y();
    qreal const x3 = pos3.x();
    qreal const y3 = pos3.y();

    qreal const expr12 = ( x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2 ) / 2;
    qreal const expr13 = ( x1 * x1 + y1 * y1 - x3 * x3 - y3 * y3 ) / 2;

    qreal const x = ( expr12 * ( y1 - y3 ) - expr13 * ( y1 - y2 ) ) / ( ( x1 - x2 ) * ( y1 - y3 ) - ( x1 - x3 ) * ( y1 - y2 ) );
    qreal const y = ( expr12 * ( x1 - x3 ) - expr13 * ( x1 - x2 ) ) / ( ( y1 - y2 ) * ( x1 - x3 ) - ( y1 - y3 ) * ( x1 - x2 ) );

    return QPointF( x, y );
}

qreal Math::GetCircumcircleRadius( QPointF const & pos1, QPointF const & pos2, QPointF const & pos3 )
{
    qreal const x1 = pos1.x();
    qreal const y1 = pos1.y();
    qreal const x2 = pos2.x();
    qreal const y2 = pos2.y();
    qreal const x3 = pos3.x();
    qreal const y3 = pos3.y();

    qreal const a = sqrt( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) );
    qreal const b = sqrt( ( x2 - x3 ) * ( x2 - x3 ) + ( y2 - y3 ) * ( y2 - y3 ) );
    qreal const c = sqrt( ( x3 - x1 ) * ( x3 - x1 ) + ( y3 - y1 ) * ( y3 - y1 ) );
    qreal const p = ( a + b + c ) / 2;
    qreal const s = sqrt( p * ( p - a ) * ( p - b ) * ( p - c ) );

    return ( a * b * c / ( 4 * s ) );
}

Math::ValuesTriple< double > Math::GetGeneralCoefficients( GeomShapes::Line const & l )
{
    QPointF const p1 = l.p1();
    QPointF const p2 = l.p2();

    double const x1 = p1.x();
    double const y1 = p1.y();
    double const x2 = p2.x();
    double const y2 = p2.y();

    if ( y1 != 0 && y2 != 0 )
    {
        if ( x1 / y1 != x2 / y2 )
        {
            double const d = x1 * y2 - x2 * y1;

            return ValuesTriple< double >( ( y1 - y2 ) / d, ( x2 - x1 ) / d, 1 );
        }
        else
        {
            double const k = x1 / y1;

            return ValuesTriple< double >( 1, -k, 0 );
        }
    }
    else if ( y1 == 0 && y2 != 0 )
    {
        if ( x1 != 0 )
        {
            return ValuesTriple< double >( -1 / x1, ( x2 - x1 ) / ( x1 * y2 ), 1 );
        }
        else
        {
            return ValuesTriple< double >( 1, -x2 / y2, 0 );
        }
    }
    else if ( y2 == 0 && y1 != 0 )
    {
        if ( x2 != 0 )
        {
            return ValuesTriple< double >( -1 / x2, ( x1 - x2 ) / ( x2 * y1 ), 1 );
        }
        else
        {
            return ValuesTriple< double >( 1, -x1 / y1, 0 );
        }
    }
    else
    {
        return ValuesTriple< double >( 0, 1, 0 );
    }
}

/*
Math::ValuesTriple< double > Math::GetGeneralCoefficients( GraphicsLine * l )
{
	QPointF const p1 = l->GetDefiningPoints().GetFirst()->GetCenter();
	QPointF const p2 = l->GetDefiningPoints().GetSecond()->GetCenter();

	return GetGeneralCoefficients( GeomShapes::Line( p1, p2 ) );
}
*/



Math::TwoLineSegmentsIntersectionData Math::GetIntersectionData( GeomShapes::LineSegment const & ls1, GeomShapes::LineSegment const & ls2 )
{
    QPointF * intersectionPoint = new QPointF;

    QLineF const lf1 = ls1.GetQLineF();
    QLineF const lf2 = ls2.GetQLineF();

    switch ( lf1.intersect( lf2, intersectionPoint ) )
    {
    case QLineF::BoundedIntersection :
    {
        return TwoLineSegmentsIntersectionData( TwoLineSegmentsLocation::Intersect, intersectionPoint );
    }
    case QLineF::UnboundedIntersection :
    {
        return TwoLineSegmentsIntersectionData( TwoLineSegmentsLocation::ExtensionIntersect, nullptr );
    }
    case QLineF::NoIntersection :
    {
        return TwoLineSegmentsIntersectionData( TwoLineSegmentsLocation::Parallel, nullptr );
    }
    }
}

Math::CircleWithLineIntersectionData Math::GetIntersectionData( GeomShapes::Circle const & circle, GeomShapes::Line const & l )
{
    QPointF const p1 = l.p1();
    QPointF const p2 = l.p2();
    QPointF const circleCenter = circle.GetCenter();
    double const r = circle.GetRadius();

    double const x1 = p1.x();
    double const y1 = p1.y();
    double const x2 = p2.x();
    double const y2 = p2.y();
    double const x0 = circleCenter.x();
    double const y0 = circleCenter.y();

    //Shapes::Line const transferedLine( p1 - circleCenter, p2 - circleCenter );
    ValuesTriple< double > const generalCoefficients = GetGeneralCoefficients( l );
    double const a = generalCoefficients.GetFirst();
    double const b = generalCoefficients.GetSecond();
    double const c = generalCoefficients.GetThird();

    QPointF const perpBase( ( b * ( b * x0 - a * y0 ) - a * c ) / ( a * a + b * b ),
                            ( a * ( a * y0 - b * x0 ) - b * c ) / ( a * a + b * b ) );
    double const h = std::abs( ( y2 - y1 ) * x0 - ( x2 - x1 ) * y0 + x2 * y1 - x1 * y2 ) /
                     std::sqrt( ( y2 - y1 ) * ( y2 - y1 ) + ( x2 - x1 ) * ( x2 - x1 ) );
    if ( h < r )
    {
        double const m = std::sqrt( r * r - h * h );
        Math::Vector const v( b * m / std::sqrt( a * a + b * b ), -a * m / std::sqrt( a * a + b * b ) );

        return CircleWithLineIntersectionData( CircleWithLineLocation::IntersectionInTwoPoints, perpBase + v, perpBase - v );
    }
    else if ( h == r )
    {
        return CircleWithLineIntersectionData( CircleWithLineLocation::Touching, perpBase );
    }
    else
    {
        return CircleWithLineIntersectionData( CircleWithLineLocation::NoIntersection );
    }
}

Math::CircleWithLineSegmentIntersectionData Math::GetIntersectionData( GeomShapes::Circle const & circle, GeomShapes::LineSegment const & ls )
{
    CircleWithLineIntersectionData const data = GetIntersectionData( circle, GeomShapes::Line( ls ) );
    switch ( data.GetLocation() )
    {
    case CircleWithLineLocation::IntersectionInTwoPoints :
    {
        std::pair< QPointF const, QPointF const > const points = data.GetIntersectionPointsPair();
        QPointF const p1 = points.first;
        QPointF const p2 = points.second;
        bool const p1IsInside = ( ls.p1() < p1 && p1 < ls.p2() ) || ( ls.p2() < p1 && p1 < ls.p1() );
        bool const p2IsInside = ( ls.p1() < p2 && p2 < ls.p2() ) || ( ls.p2() < p2 && p2 < ls.p1() );
        if ( p1IsInside && p2IsInside )
        {
            return CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation::IntersectionInTwoPoints, p1, p2 );
        }
        else if ( p1IsInside )
        {
            return CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation::IntersectionInOnePoint, p1 );
        }
        else if ( p2IsInside )
        {
            return CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation::IntersectionInOnePoint, p2 );
        }
        else
        {
            return CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation::NoIntersection );
        }
    }
    case CircleWithLineLocation::Touching :
    {
        QPointF const p = data.GetOnlyIntersectionPoint();
        if ( ( ls.p1() < p && p < ls.p2() ) || ( ls.p2() < p && p < ls.p1() ) )
        {
            return CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation::Touching, p );
        }
        else
        {
            return CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation::NoIntersection );
        }
    }
    case CircleWithLineLocation::NoIntersection :
    {
        return CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation::NoIntersection );

        break;
    }
    }
}

Math::TwoCirclesIntersectionData Math::GetIntersectionData( GeomShapes::Circle const & c1, GeomShapes::Circle const & c2 )
{
    QPointF const center1 = c1.GetCenter();
    QPointF const center2 = c2.GetCenter();
    double const r1 = c1.GetRadius();
    double const r2 = c2.GetRadius();

    double const maxr = qMax( r1, r2 );
    double const minr = qMin( r1, r2 );

    double const d = GetDistance( center1, center2 );

    if ( d == 0 ||
            d > r1 + r2 ||
            d < qAbs( r1 - r2 ) )
    {
        return TwoCirclesIntersectionData( TwoCirclesLocation::NoIntersection );
    }
    else if ( d == r1 + r2 )
    {
        QPointF const p = QLineF( center1, center2 ).pointAt( r1 / ( r1 + r2 ) );

        return TwoCirclesIntersectionData( TwoCirclesLocation::Touching, new QPointF( p ) );
    }
    else if ( d == qAbs( r1 - r2 ) )
    {
        if ( r1 == maxr )
        {
            QPointF const p = QLineF( center1, center2 ).pointAt( r1 / d );

            return TwoCirclesIntersectionData( TwoCirclesLocation::Touching, new QPointF( p ) );
        }
        else if ( r2 == maxr )
        {
            QPointF const p = QLineF( center2, center1 ).pointAt( r2 / d );

            return TwoCirclesIntersectionData( TwoCirclesLocation::Touching, new QPointF( p ) );
        }
        else
        {
            // throw Exception
        }
    }
    else
    {
        double const d1 = ( ( r1 * r1 - r2 * r2 ) / d + d ) / 2;
        double const h1 = sqrt( r1 * r1 - d1 * d1 );
        QPointF const p0 = QLineF( center1, center2 ).pointAt( d1 / d );

        Math::Vector const n = Math::Vector( center1, center2 ).GetNormal(
                                   CircleDirection::Counterclockwise, CircleDirection::Clockwise );
        QPointF const p1 = p0 + n;
        QPointF const p2 = p0 - n;

        QPointF const p3 = QLineF( p0, p1 ).pointAt( h1 / n.GetNorm() );
        QPointF const p4 = QLineF( p0, p2 ).pointAt( h1 / n.GetNorm() );

        return TwoCirclesIntersectionData(
                   TwoCirclesLocation::IntersectionInTwoPoints,
                   new QPointF( p3 ),
                   new QPointF( p4 ) );
    }

    // throw Exception
}

QPointF Math::GetPerpendicularBasePoint( QPointF const & from, QLineF const & ls )
{
    QLineF const lineVector = ls;
    QLineF const vectorToPos( ls.p1(), from );

    qreal a = lineVector.angleTo( vectorToPos );
    Math::Angle const angle( a, Math::AngleMeasure::Degrees );

    qreal const d = vectorToPos.length() * cos( angle.GetRadians() );

    return QPointF( lineVector.pointAt( d / lineVector.length() ) );
}

std::pair< QPointF const, QPointF const > Math::GetTangentPoints( QPointF const & sourcePoint, GeomShapes::Circle const & circle )
{
    QPointF const center = circle.GetCenter();
    double const r = circle.GetRadius();

    double const x = GetDistance( sourcePoint, center );
    QPointF const base = QLineF( center, sourcePoint ).pointAt( ( r / x ) * ( r / x ) );
    double const h = sqrt( r * r - ( r * r / x ) * ( r * r / x ) );

    Math::Vector const n = Math::Vector( sourcePoint, center ).GetNormal(
                               CircleDirection::Counterclockwise,
                               CircleDirection::Clockwise );

    QPointF const p1 = QLineF( base, base + n ).pointAt( h / n.GetNorm() );
    QPointF const p2 = QLineF( base, base - n ).pointAt( h / n.GetNorm() );

    return std::pair< QPointF const, QPointF const >( p1, p2 );
}

bool Math::PointIsOutsideCircle( QPointF const & scenePos, GeomShapes::Circle const & circle )
{
    return GetDistance( scenePos, circle.GetCenter() ) >= circle.GetRadius();
}



std::list< int > Math::GetBadAngleVertexIndexes( GeomShapes::Polygon const & pol, CircleDirection baseOrientation )
{
    std::list< int > result;
    for ( int i = 0; i < pol.GetVerticesCount(); ++i )
    {
        Math::Vector const e1( pol.GetVertex( i - 1 ), pol.GetVertex( i ) );
        Math::Vector const e2( pol.GetVertex( i ), pol.GetVertex( i + 1 ) );
        Math::Vector const n2 = e2.GetNormal( baseOrientation, Math::CircleDirection::Clockwise );

        if ( e1 * n2 > 0 )
        {
            result.push_back( i );
        }
    }

    return result;
}

std::list< int > Math::GetGoodAngleVertexIndexes( GeomShapes::Polygon const & pol, CircleDirection baseOrientation )
{
    std::list< int > result;
    for ( int i = 0; i < pol.GetVerticesCount(); ++i )
    {
        Math::Vector const e1( pol.GetVertex( i - 1 ), pol.GetVertex( i ) );
        Math::Vector const e2( pol.GetVertex( i ), pol.GetVertex( i + 1 ) );
        Math::Vector const n2 = e2.GetNormal( baseOrientation, Math::CircleDirection::Clockwise );

        if ( e1 * n2 < 0 )
        {
            result.push_back( i );
        }
    }

    return result;
}



void Math::Tests()
{
    Test_GetGeneralCoefficients();
}

void Math::Test_VectorGetRelativeAngle()
{
    CircleDirection const baseOrientation1 = CircleDirection::Counterclockwise;
    CircleDirection const baseOrientation2 = CircleDirection::Clockwise;

    Vector const v1( 1, 0 );
    Vector const v2( 1, 1 );
    //qDebug() << v1.GetRelativeAngle( v2, baseOrientation1, CircleDirection::Counterclockwise ).GetDegrees();
    //qDebug() << v1.GetRelativeAngle( v2, baseOrientation2, CircleDirection::Counterclockwise ).GetDegrees();

    Vector const v3( sqrt( 3 ), 1 );
    Vector const v4( 0, 1 );
    qDebug() << v3.GetRelativeAngle( v4, baseOrientation1, CircleDirection::Counterclockwise ).GetDegrees();
}

void Math::Test_GetBadAngleVertexIndexes()
{
    std::vector< QPointF > const vertices = { QPointF( 1, 5 ),
                                              QPointF( 6, 9 ),
                                              QPointF( 10, 7 ),
                                              QPointF( 8, 5 ),
                                              QPointF( 12, 3 ),
                                              QPointF( 5, 4 ),
                                              QPointF( 3, 2 )
                                            };

    try
    {
        GeomShapes::Polygon polygon( vertices );

        for ( int i : GetBadAngleVertexIndexes( polygon, CircleDirection::Counterclockwise ) )
        {
            qDebug() << i ;
        }
    }
    catch ( Exception const & exc )
    {
        qDebug() << exc.GetMessage() ;
    }
}

void Math::Test_GetGeneralCoefficients()
{
    try
    {
        GeomShapes::Line const l1( QPointF( 1, -2 ), QPointF( 3, -3 ) );

        qDebug() << GetGeneralCoefficients( l1 ) ;
    }
    catch ( Exception const & exc )
    {
        qDebug() << exc.GetMessage() ;
    }
}













