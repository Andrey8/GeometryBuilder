#include "polygonalgorithms.h"
#include "Shapes/polygon.h"
#include "mathnamespace.h"
#include "DebugHelpers/debughelper.h"
#include "DebugHelpers/exception.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <QPointF>
#include <QDebug>

QDebug operator<<( QDebug, PolygonAlgorithms::Slab const & );
QDebug operator<<( QDebug, PolygonAlgorithms::SlabLineSegment const & );
QDebug operator<<( QDebug, PolygonAlgorithms::PointLocationOverPolygon );
QDebug operator<<( QDebug, Math::Vector const & );
QDebug operator<<( QDebug, GeomShapes::LineSegment const & );
QDebug operator<<( QDebug, PolygonAlgorithms::MLSPointData const & );
QDebug operator<<( QDebug, PolygonAlgorithms::LineSegmentIndexWithPointData const & );
QDebug operator<<( QDebug, Math::CircleDirection );
QDebug operator<<( QDebug, PolygonAlgorithms::TwoPolygonsLocation );



void PolygonAlgorithms::Tests()
{
	Test_GetLocationsInCaseOfTranslations();
	//Test_CharacteristicRegionAlgorithm();
	//Test_MovingLineSegmentsAlgorithm();
	//Test_PointLocationAlgorithm();
	//Test_GetPolygonVerticesDirection();
}



std::list< std::pair< Math::Vector, PolygonAlgorithms::TwoPolygonsLocation > > PolygonAlgorithms::GetLocations( GeomShapes::Polygon const & fixed, GeomShapes::Polygon const & moving, std::vector< Math::Vector > const & translations )
{
	QPointF const p = moving.GetVertices().front();

	//qDebug() << p ;

	std::list< SlabSystem > slabSystems1;
	for ( GeomShapes::Polygon const & pol : GetCharacteristicRegionForInside( fixed, moving, p ) )
	{
		slabSystems1.push_back( SlabSystem( pol ) );
	}
	std::list< SlabSystem > slabSystems2;
	for ( GeomShapes::Polygon const & pol : GetComplementToCharacteristicRegionForOutside( fixed, moving, p ) )
	{
		slabSystems2.push_back( SlabSystem( pol ) );
	}

	std::list< std::pair< Math::Vector, TwoPolygonsLocation > > result;
	for ( Math::Vector const & t : translations )
	{
		std::pair< Math::Vector, TwoPolygonsLocation > pair( t, TwoPolygonsLocation::Intersection );

		bool label = false;
		for ( SlabSystem const & ss : slabSystems1 )
		{
			if ( GetPointLocationOverPolygon( p + t, ss ) != PointLocationOverPolygon::Outside )
			{
				pair = std::pair< Math::Vector, TwoPolygonsLocation >( t, TwoPolygonsLocation::SecondIsInsideFirst );

				label = true;

				break;
			}
		}

		if ( !label )
		{
			for ( SlabSystem const & ss : slabSystems2 )
			{
				if ( GetPointLocationOverPolygon( p + t, ss ) == PointLocationOverPolygon::Outside )
				{
					pair = std::pair< Math::Vector, TwoPolygonsLocation >( t, TwoPolygonsLocation::Outside );

					break;
				}
			}
		}

		result.push_back( pair );
	}

	return result;
}

std::list< std::pair< std::pair< Math::Vector, Math::Angle >, PolygonAlgorithms::TwoPolygonsLocation > > PolygonAlgorithms::GetLocations( GeomShapes::Polygon const & fixed, GeomShapes::Polygon const & moving, std::vector< std::pair< Math::Vector, Math::Angle > > const & translationsWithAngles )
{


	std::list< std::pair< std::pair< Math::Vector, Math::Angle >, PolygonAlgorithms::TwoPolygonsLocation > > result;
	for ( std::pair< Math::Vector, Math::Angle > translationWithAngle : translationsWithAngles )
	{

	}
}



void PolygonAlgorithms::Test_GetLocationsInCaseOfTranslations()
{
	std::vector< QPointF > const vertices1 = { QPointF( 2, 3 ),
											   QPointF( 4, 8 ),
											   QPointF( 11, 10 ),
											   QPointF( 16, 9 ),
											   QPointF( 12, 6 ),
											   QPointF( 19, 2 ),
											   QPointF( 7, 4 ) };
	GeomShapes::Polygon const fixed( vertices1 );

	std::vector< QPointF > const vertices2 = { QPointF( 3, 1 ),
											   QPointF( 5, 2 ),
											   QPointF( 6, 1 ) };
	GeomShapes::Polygon const moving( vertices2 );

	std::vector< Math::Vector > const translations = { Math::Vector( 3, 4 ),
													   Math::Vector( 8, 2 ),
													   Math::Vector( -3, 4 ),
													   Math::Vector( 11, 5 ),
													   Math::Vector( 8, 7 ),
													   Math::Vector( 6, 0 ),
													   Math::Vector( 8, 1 ),
													   Math::Vector( 0, 4.5 ) };

	try
	{
		std::list< std::pair< Math::Vector, TwoPolygonsLocation > > const translationsWithLocations = GetLocations( fixed, moving, translations );
		SKIP();
		for ( std::pair< Math::Vector, TwoPolygonsLocation > translationWithLocation : translationsWithLocations )
		{
			qDebug() << translationWithLocation.first << " : " << translationWithLocation.second ;
		}
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void PolygonAlgorithms::Test_CharacteristicRegionAlgorithm()
{
	qDebug() << "Characteristic region defining algorithm test :\n" ;

	std::vector< QPointF > const vertices1 = { QPointF( 1, 3 ),
											   QPointF( 2, 8 ),
											   QPointF( 9 ,9 ),
											   QPointF( 12, 2 ) };

	std::vector< QPointF > const vertices2 = { QPointF( 15, 15 ),
											   QPointF( 16, 18 ),
											   QPointF( 17, 16 ) };

	std::vector< QPointF > const vertices11 = { QPointF( 2, 3 ),
												QPointF( 4, 8 ),
												QPointF( 11, 10 ),
												QPointF( 16, 9 ),
												QPointF( 12, 6 ),
												QPointF( 19, 2 ),
												QPointF( 7, 4 ) };

	std::vector< QPointF > const vertices22 = { QPointF( 3, 1 ),
												QPointF( 5, 2 ),
												QPointF( 6, 1 ) };

	std::vector< QPointF > const vertices3 = { QPointF( 1, 4 ),
											   QPointF( 3, 9 ),
											   QPointF( 8, 10 ),
											   QPointF( 11, 7 ),
											   QPointF( 15, 8 ),
											   QPointF( 20, 3 ),
											   QPointF( 12, 1 ),
											   QPointF( 10, 5 ) };

	std::vector< QPointF > const vertices33 = { QPointF( 2, 2 ),
												QPointF( 5, 3 ),
												QPointF( 3, 1 ) };

	std::vector< QPointF > const vertices44 = { QPointF( 6, 1 ),
												QPointF( 7, 4 ),
												QPointF( 9, 2 ) };

	try
	{
		GeomShapes::Polygon const pol1( vertices11 );
		GeomShapes::Polygon const pol2( vertices22 );
		QPointF const p( 3, 1 );

		for ( GeomShapes::LineSegment const & mls : GetMotionLineSegmentsForOutside( pol1, pol2, p ) )
		{
			qDebug() << mls ;
		}

		std::list< GeomShapes::Polygon > const region = GetComplementToCharacteristicRegionForOutside( pol1, pol2, p );

		qDebug() << "\nPolygons count : " << region.size() << "\n" ;
		uint16_t c = 1;
		for ( GeomShapes::Polygon const & pol : region )
		{
			qDebug() << "Polygon " << c << " :" ;
			SHOW_stdvector( pol.GetVertices() );
			//qDebug() << "\n" ;

			++c;
		}
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void PolygonAlgorithms::Test_GetPolygonVerticesDirection()
{
	std::vector< QPointF > const vertices1 = { QPointF( 1, 4 ),
											   QPointF( 3, 9 ),
											   QPointF( 8, 10 ),
											   QPointF( 11, 7 ),
											   QPointF( 15, 8 ),
											   QPointF( 20, 3 ),
											   QPointF( 12, 1 ),
											   QPointF( 10, 5 ) };

	std::vector< QPointF > const vertices2 = { QPointF( 5, 3 ),
											   QPointF( 3, 2 ),
											   QPointF( 1, 3 ),
											   QPointF( 4, 4 ),
											   QPointF( 1, 5 ),
											   QPointF( 3, 7 ),
											   QPointF( 8, 6 ),
											   QPointF( 7, 4 ),
											   QPointF( 8, 3 ),
											   QPointF( 8, 2 ),
											   QPointF( 6, 1 ) };

	try
	{
		qDebug() << GetPolygonVerticesDirection( vertices2, Math::CircleDirection::Counterclockwise );
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void PolygonAlgorithms::Test_MotionLineSegmentsAlgorithm()
{
	qDebug() << "Moving line segments defining algorithm test :\n" ;

	std::vector< QPointF > const vertices1 = { QPointF( 1, 3 ),
											   QPointF( 2, 8 ),
											   QPointF( 9 ,9 ),
											   QPointF( 12, 2 ) };
	std::vector< QPointF > const vertices11 = { QPointF( 2, 3 ),
												QPointF( 4, 8 ),
												QPointF( 11, 10 ),
												QPointF( 16, 9 ),
												QPointF( 12, 6 ),
												QPointF( 19, 2 ),
												QPointF( 7, 4 ) };

	std::vector< QPointF > const vertices2 = { QPointF( 15, 15 ),
											   QPointF( 16, 18 ),
											   QPointF( 17, 16 ) };
	std::vector< QPointF > const vertices22 = { QPointF( 3, 1 ),
												QPointF( 5, 2 ),
												QPointF( 6, 1 ) };

	try
	{
		Math::CircleDirection const baseOrientation = Math::CircleDirection::Counterclockwise;

		GeomShapes::Polygon const pol1( vertices1 );
		GeomShapes::Polygon const pol2( vertices2 );
		QPointF const p = QPointF( 17, 16 );

		uint16_t const n1 = pol1.GetVerticesCount();
		uint16_t const n2 = pol2.GetVerticesCount();

		std::vector< QPointF > slidingVertices;
		slidingVertices.reserve( n1 );
		std::list< GeomShapes::LineSegment > movingLineSegments;
		std::list< int > const badAngleVertexIndexes = Math::GetBadAngleVertexIndexes( pol1, Math::CircleDirection::Counterclockwise );
		for ( int i1 = 0; i1 < n1; ++i1 )
		{
			Math::Vector const e( pol1.GetVertex( i1 ), pol1.GetVertex( i1 + 1 ) );
			qDebug() << e ;

			Math::Vector const normal = e.GetNormal( Math::CircleDirection::Counterclockwise, Math::CircleDirection::Clockwise );
			qDebug() << "Normal : " << normal ;
			for ( int i2 = 0; i2 < n2; ++i2 )
			{
				Math::Vector const e1( pol2.GetVertex( i2 ), pol2.GetVertex( i2 - 1 ) );
				Math::Vector const e2( pol2.GetVertex( i2 ), pol2.GetVertex( i2 + 1 ) );

				//qDebug() << e1 ;
				//qDebug() << e2 ;
				//qDebug() << "Scalar product 1 : " << ( e1 * normal ) ;
				//qDebug() << "Scalar product 2 : " << ( e2 * normal ) ;
				if ( e1 * normal >= 0 && e2 * normal >= 0 )
				{
					qDebug() << pol2.GetVertex( i2 ) ;

					slidingVertices.push_back( pol2.GetVertex( i2 ) );

					break;
				}
				else
				{
					//qDebug() << pol2.GetVertex( i2 ) << "is not sliding vertex." ;
				}
			}

			Math::Vector const v( slidingVertices[ i1 ], p );
			movingLineSegments.push_back( GeomShapes::LineSegment( pol1.GetVertex( i1 ) + v, pol1.GetVertex( i1 + 1 ) + v ) );
		}

		qDebug() << "Bad angle indexes : " ;
		for ( int i : badAngleVertexIndexes )
		{
			qDebug() << i ;
		}
		for ( int i2 = 0; i2 < n2; ++i2 )
		{
			Math::Vector const e( pol2.GetVertex( i2 ), pol2.GetVertex( i2 + 1 ) );
			Math::Vector const normal = e.GetNormal( baseOrientation, Math::CircleDirection::Counterclockwise );

			qDebug() << "edge : " << e << "normal : " << normal ;

			for ( int index : badAngleVertexIndexes )
			{
				Math::Vector const e1( pol1.GetVertex( index ), pol1.GetVertex( index - 1 ) );
				Math::Vector const e2( pol1.GetVertex( index ), pol1.GetVertex( index + 1 ) );

				qDebug() << "e1 : " << e1 << "e2 : " << e2 ;

				if ( e1 * normal > 0 && e2 * normal > 0 )
				{
					qDebug() << "Yes" ;

					Math::Vector const v( pol2.GetVertex( i2 + 1 ), p );
					QPointF const a = pol1.GetVertex( index ) + v;
					movingLineSegments.push_back( GeomShapes::LineSegment( a, a + e ) );
				}
			}
		}

		for ( GeomShapes::LineSegment const & ls : movingLineSegments )
		{
			qDebug() << ls ;
		}
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void PolygonAlgorithms::Test_PointLocationAlgorithm()
{
	qDebug() << "Point location algorithm tests :\n" ;

	std::vector< QPointF > const vertices1 = { QPointF( 1, 5 ),
											  QPointF( 6, 9 ),
											  QPointF( 10, 7 ),
											  QPointF( 8, 5 ),
											  QPointF( 12, 3 ),
											  QPointF( 3, 2 ) };

	std::vector< QPointF > const points = { QPointF( 7, 6 ),
											QPointF( 7, 9 ),
											QPointF( 2, 2 ),
											QPointF( 9, 5 ),
											QPointF( 2, 5.5 ),
											QPointF( 4, 6 ),
											QPointF( 9, 3 ),
											QPointF( 2, 6 ),
											QPointF( 8, 8 ),
											QPointF( 12, 3 ),
											QPointF( 8, 5 ),
											QPointF( 1, 5 ),
											QPointF( 9, 7.5 ),
											QPointF( 2, 5.8 ),
											QPointF( 5, 2.22 ) };

	std::vector< QPointF > const vertices2 = { QPointF( 1, 3 ),
											   QPointF( 2, 8 ),
											   QPointF( 9 ,9 ),
											   QPointF( 12, 2 ) };
	QPointF const p1 = QPointF( 9.86538,6.98077 );
	QPointF const p2 = QPointF( 11.473,3.22973 );

	try
	{
		qDebug() << GetPointLocationOverPolygon( p1, GeomShapes::Polygon( vertices2 ) );
		qDebug() << GetPointLocationOverPolygon( p2, GeomShapes::Polygon( vertices2 ) );
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}

void PolygonAlgorithms::SKIP()
{
	qDebug() << "\n\n\n" ;
}

void PolygonAlgorithms::SHOW_ExpressionValue( bool expression )
{
	if ( expression )
	{
		qDebug() << "True\n" ;
	}
	else
	{
		qDebug() << "False\n" ;
	}
}

template < typename T > void PolygonAlgorithms::SHOW_stdvector( std::vector< T > const & values )
{
	for ( T const & value : values )
	{
		qDebug() << value ;
	}
}

template < typename T > void PolygonAlgorithms::SHOW_stdlist( std::list< T > const & values )
{
	for ( T const & value : values )
	{
		qDebug() << value ;
	}
}

template < typename T > void PolygonAlgorithms::SHOW_stdset( std::set< T > const & values )
{
	for ( T const & value : values )
	{
		qDebug() << value ;
	}
}

void PolygonAlgorithms::SHOW_pointsstdset( std::set< QPointF, PointsComparer > const & s )
{
	qDebug() << "Points set : [ " ;
	for ( QPointF const & p : s )
	{
		qDebug() << p << " " ;
	}
	qDebug() << " ]" ;
}



bool PolygonAlgorithms::MLSPointDataComparer::operator ()( MLSPointData * pd1, MLSPointData * pd2 ) const
{
	QPointF const p1 = m_lineSegment.p1();
	QPointF const p2 = m_lineSegment.p2();
	QPointF const pdPos1 = pd1->GetPosition();
	QPointF const pdPos2 = pd2->GetPosition();

	if ( p1.x() < p2.x() )
	{
		return ( pdPos1.x() < pdPos2.x() );
	}
	else if ( p1.x() > p2.x() )
	{
		return ( pdPos1.x() > pdPos2.x() );
	}
	else if ( p1.x() == p2.x() )
	{
		if ( p1.y() < p2.y() )
		{
			return ( pdPos1.y() < pdPos2.y() );
		}
		else if ( p1.y() > p2.y() )
		{
			return ( pdPos1.y() > pdPos2.y() );
		}
		else
		{
			throw Exception( "PolygonAlgorithms::MLSPointDataComparer::operator ()( MLSPointData * pd1, MLSPointData * pd2 ) : p1 == p2." );
		}
	}
}

void PolygonAlgorithms::SortPointDatasAlongLineSegment( std::list< MLSPointData * > & pdsList, GeomShapes::LineSegment const & ls )
{
	std::vector< MLSPointData * > pdsVector( pdsList.begin(), pdsList.end() );
	std::sort( pdsVector.begin(), pdsVector.end(), MLSPointDataComparer( ls ) );

	pdsList = std::list< MLSPointData * >( pdsVector.begin(), pdsVector.end() );
}

uint16_t PolygonAlgorithms::GetNextLineSegmentIndex( MLSPointData * pd, uint16_t currentIndex, std::vector< GeomShapes::LineSegment > const & lineSegments )
{
	std::set< uint16_t > lineSegmentsIndexes = pd->GetLineSegmentsIndexes();
	lineSegmentsIndexes.erase( currentIndex );

	// Optimization
	if ( lineSegmentsIndexes.size() == 1 )
	{
		return *( lineSegmentsIndexes.begin() );
	}

	uint16_t result = *( lineSegmentsIndexes.begin() );
	Math::Vector const v1( pd->GetPosition(), lineSegments[ currentIndex ].p1() );
	Math::Vector const resultV( pd->GetPosition(), lineSegments[ result ].p2() );
	Math::CircleDirection const baseOrientation = Math::CircleDirection::Clockwise;
	Math::Angle a = v1.GetRelativeAngle( resultV, baseOrientation, Math::CircleDirection::Counterclockwise );
	for ( uint16_t index : lineSegmentsIndexes )
	{
		Math::Vector const v2( pd->GetPosition(), lineSegments[ index ].p2() );
		Math::Angle const a2 = v1.GetRelativeAngle( v2, baseOrientation, Math::CircleDirection::Counterclockwise );
		if ( a2 < a )
		{
			a = a2;
			result = index;
		}
	}

	return result;
}

PolygonAlgorithms::MLSPointData * PolygonAlgorithms::GetNextPointData( MLSPointData * pointData, std::list< MLSPointData * > const & pointDataList )
{
	MLSPointData * result = nullptr;
	bool x = false;
	qDebug() << *pointData << pointData ;
	for ( MLSPointData * pd : pointDataList )
	{
		qDebug() << "pd : " << *pd << pd ;

		if ( x == true )
		{
			result = pd;

			break;
		}

		if ( *pd == *pointData )
		{
			//qDebug() << "AAAAAAAAAAAAA" ;
			x = true;
		}
	}

	return result;
}



PolygonAlgorithms::PointLocationOverPolygon PolygonAlgorithms::GetPointLocationOverPolygon( QPointF const & p, GeomShapes::Polygon const & polygon )
{
	return GetPointLocationOverPolygon( p, SlabSystem( polygon ) );
}

PolygonAlgorithms::PointLocationOverPolygon PolygonAlgorithms::GetPointLocationOverPolygon( QPointF const & p, SlabSystem const & slabSystem )
{
	std::vector< Slab > const slabs = slabSystem.GetSlabs();

	//qDebug() << p ;
	/*
	qDebug() << "Slabs :" ;
	SHOW_stdvector( slabs );
	qDebug() << "Slabs border vertices sets :" ;
	for ( int i = 0; i < slabSystem.GetXCoords().size(); ++i )
	{
		SHOW_pointsstdset( slabSystem.GetSlabBorderVertices( i ) );
	}
	*/

	std::vector< double > const x = slabSystem.GetXCoords();
	int i = 0;
	int k = x.size() - 1;
	int slabIndex = 0;
	if ( p.x() < x[ 0 ] || p.x() > x[ k ] )
	{
		return PointLocationOverPolygon::Outside;
	}
	if ( p.x() == x[ 0 ] )
	{
		std::set< QPointF, PointsComparer > const points = slabSystem.GetSlabBorderVertices( 0 );
		if ( points.find( p ) != points.end() )
		{
			return PointLocationOverPolygon::Vertex;
		}
		else
		{
			return PointLocationOverPolygon::Boundary;
		}
	}
	while ( true )
	{
		if ( slabs[ i ].x2() == x[ k ] )
		{
			slabIndex = i;

			break;
		}

		int const m = ( i + k ) / 2;
		if ( p.x() <= x[ m ] )
		{
			k = m;
		}
		else
		{
			i = m;
		}
	}
	Slab const slab = slabs[ slabIndex ];

	std::vector< SlabLineSegment > const lineSegments = slab.GetLineSegments();

	/*
	qDebug() << "Slab's line segments :" ;
	SHOW_stdvector( lineSegments );
	*/

	i = 0;
	k = lineSegments.size() - 1;
	if ( lineSegments[ 0 ].IsOverPoint( p ) || lineSegments[ k ].IsUnderPoint( p ) )
	{
		return PointLocationOverPolygon::Outside;
	}
	if ( lineSegments[ 0 ].Contains( p ) )
	{
		std::set< QPointF, PointsComparer > const & points = slabSystem.GetSlabBorderVertices( slabIndex + 1 );
		if ( points.find( p ) != points.end() )
		{
			return PointLocationOverPolygon::Vertex;
		}
		else
		{
			return PointLocationOverPolygon::Boundary;
		}
	}
	int lineSegmentIndex = 0;
	while ( true )
	{
		if ( i + 1 == k )
		{
			if ( lineSegments[ k ].Contains( p ) )
			{
				std::set< QPointF, PointsComparer > const & points = slabSystem.GetSlabBorderVertices( slabIndex + 1 );
				if ( points.find( p ) != points.end() )
				{
					return PointLocationOverPolygon::Vertex;
				}
				else
				{
					return PointLocationOverPolygon::Boundary;
				}
			}

			lineSegmentIndex = i;

			break;
		}

		int const m = ( i + k ) / 2;
		if ( lineSegments[ m ].IsUnderPoint( p ) )
		{
			i = m;
		}
		else
		{
			k = m;
		}
	}
	std::set< QPointF, PointsComparer > const & points = slabSystem.GetSlabBorderVertices( slabIndex + 1 );
	if ( points.find( p ) != points.end() )
	{
		return PointLocationOverPolygon::Vertex;
	}

	if ( lineSegmentIndex % 2 == 0 )
	{
		return PointLocationOverPolygon::Inside;
	}
	else
	{
		return PointLocationOverPolygon::Outside;
	}
}

std::list< GeomShapes::LineSegment > PolygonAlgorithms::GetMotionLineSegmentsForInside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation )
{
	uint16_t const n1 = pol1.GetVerticesCount();
	uint16_t const n2 = pol2.GetVerticesCount();

	std::vector< QPointF > slidingVertices;
	slidingVertices.reserve( n1 );
	std::list< GeomShapes::LineSegment > movingLineSegments;
	for ( int i1 = 0; i1 < n1; ++i1 )
	{
		Math::Vector const normal = Math::Vector( pol1.GetVertex( i1 ), pol1.GetVertex( i1 + 1 ) ).GetNormal( baseOrientation, Math::CircleDirection::Clockwise );
		for ( int i2 = 0; i2 < n2; ++i2 )
		{
			Math::Vector const e1( pol2.GetVertex( i2 ), pol2.GetVertex( i2 - 1 ) );
			Math::Vector const e2( pol2.GetVertex( i2 ), pol2.GetVertex( i2 + 1 ) );
			if ( e1 * normal >= 0 && e2 * normal >= 0 )
			{
				slidingVertices.push_back( pol2.GetVertex( i2 ) );

				break;
			}
		}

		Math::Vector const v( slidingVertices[ i1 ], p );
		movingLineSegments.push_back( GeomShapes::LineSegment( pol1.GetVertex( i1 ) + v, pol1.GetVertex( i1 + 1 ) + v ) );
	}

	std::list< int > const badAngleVertexIndexes = Math::GetBadAngleVertexIndexes( pol1, baseOrientation );
	for ( int i2 = 0; i2 < n2; ++i2 )
	{
		Math::Vector const e( pol2.GetVertex( i2 ), pol2.GetVertex( i2 + 1 ) );
		Math::Vector const normal = e.GetNormal( baseOrientation, Math::CircleDirection::Counterclockwise );
		for ( int index : badAngleVertexIndexes )
		{
			Math::Vector const e1( pol1.GetVertex( index ), pol1.GetVertex( index - 1 ) );
			Math::Vector const e2( pol1.GetVertex( index ), pol1.GetVertex( index + 1 ) );
			if ( e1 * normal > 0 && e2 * normal > 0 )
			{
				Math::Vector const v( pol2.GetVertex( i2 + 1 ), p );
				QPointF const a = pol1.GetVertex( index ) + v;
				movingLineSegments.push_back( GeomShapes::LineSegment( a, a + e ) );
			}
		}
	}

	return movingLineSegments;
}

std::list< GeomShapes::LineSegment > PolygonAlgorithms::GetMotionLineSegmentsForOutside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation )
{
	uint16_t const n1 = pol1.GetVerticesCount();
	uint16_t const n2 = pol2.GetVerticesCount();

	std::list< GeomShapes::LineSegment > result;
	for ( int i1 = 0; i1 < n1; ++i1 )
	{
		Math::Vector const normal = Math::Vector( pol1.GetVertex( i1 ), pol1.GetVertex( i1 + 1 ) ).GetNormal( baseOrientation, Math::CircleDirection::Counterclockwise );
		for ( int i2 = 0; i2 < n2; ++i2 )
		{
			Math::Vector const e1( pol2.GetVertex( i2 ), pol2.GetVertex( i2 - 1 ) );
			Math::Vector const e2( pol2.GetVertex( i2 ), pol2.GetVertex( i2 + 1 ) );
			if ( e1 * normal >= 0 && e2 * normal >= 0 )
			{
				QPointF const slidingVertex = pol2.GetVertex( i2 );
				Math::Vector const v( slidingVertex, p );

				result.push_back( GeomShapes::LineSegment( pol1.GetVertex( i1 ) + v, pol1.GetVertex( i1 + 1 ) + v ) );

				break;
			}
		}
	}

	for ( int i1 : Math::GetGoodAngleVertexIndexes( pol1, baseOrientation ) )
	{
		QPointF const vertex = pol1.GetVertex( i1 );
		Math::Vector const e1( vertex, pol1.GetVertex( i1 + 1 ) );
		Math::Vector const e2( vertex, pol1.GetVertex( i1 - 1 ) );
		for ( int i2 = 0; i2 < n2; ++i2 )
		{
			Math::Vector const e( pol2.GetVertex( i2 ), pol2.GetVertex( i2 + 1 ) );
			Math::Vector const normal = e.GetNormal( baseOrientation, Math::CircleDirection::Counterclockwise );
			if ( e1 * normal >= 0 && e2 * normal >= 0 )
			{
				QPointF const a = vertex + Math::Vector( pol2.GetVertex( i2 ), p );
				result.push_back( GeomShapes::LineSegment( a, a - e ) );

				break;
			}
		}
	}

	return result;
}

Math::CircleDirection PolygonAlgorithms::GetPolygonVerticesDirection( std::vector< QPointF > const & points, Math::CircleDirection baseOrientation )
{
	try
	{
		GeomShapes::Polygon const pol( points );

		uint16_t const n = points.size();

		uint16_t rightEndPointIndex = 0;
		double maxXCoord = points[ 0 ].x();
		for ( uint16_t i = 1; i < n; ++i )
		{
			double const x = points[ i ].x();
			if ( x > maxXCoord )
			{
				maxXCoord = x;
				rightEndPointIndex = i;
			}
		}

		QPointF const rightEndPoint = points[ rightEndPointIndex ];

		QPointF const p1 = Math::GetElement( rightEndPointIndex - 1, points );
		QPointF const p2 = Math::GetElement( rightEndPointIndex + 1, points );
		Math::Vector const v1( rightEndPoint, p1 );
		Math::Vector const v2( rightEndPoint, p2 );
		Math::Angle const a1 = v1.GetRelativeAngle( Math::Vector( 0, -1 ), baseOrientation, Math::CircleDirection::Counterclockwise );
		Math::Angle const a2 = v2.GetRelativeAngle( Math::Vector( 0, -1 ), baseOrientation, Math::CircleDirection::Counterclockwise );

		if ( a1 > a2 )
		{
			return Math::CircleDirection::Clockwise;
		}
		else
		{
			return Math::CircleDirection::Counterclockwise;
		}
	}
	catch ( Exception const & exc )
	{
		qDebug() << exc.GetMessage() ;
	}
}



std::list< GeomShapes::Polygon > PolygonAlgorithms::GetGeneratedRegion( std::list< GeomShapes::LineSegment > const & movingLineSegments, Math::CircleDirection baseOrientation )
{
	std::vector< GeomShapes::LineSegment > const lineSegments( movingLineSegments.begin(), movingLineSegments.end() );
	uint16_t const n = lineSegments.size();
	std::vector< std::list< MLSPointData * > > pointDataLists;
	pointDataLists.reserve( n );
	for ( int i = 0; i < n; ++i )
	{
		std::list< MLSPointData * > pds;
		for ( int k = 0; k < n; ++k )
		{
			if ( k == i )
			{
				continue;
			}

			Math::TwoLineSegmentsIntersectionData const data = Math::GetIntersectionData( lineSegments[ i ], lineSegments[ k ] );
			if ( data.GetLocation() == Math::TwoLineSegmentsLocation::Intersect )
			{
				pds.push_back( new MLSPointData( i, k, data.GetIntersectionPoint() ) );
			}
		}
		SortPointDatasAlongLineSegment( pds, lineSegments[ i ] );
		if ( !pds.empty() )
		{
			pointDataLists.push_back( pds );
		}
	}



	std::list< GeomShapes::Polygon > result;
	std::list< LineSegmentIndexWithPointData > pairs;
	for ( uint16_t i = 0; i < n; ++i )
	{
		qDebug() << "List index : " << i ;

		for ( MLSPointData * start : pointDataLists[ i ] )
		{
			uint16_t lineSegmentIndex = i;

			qDebug() << "Start PD from list " << i << " : " << *start ;

			if ( start == pointDataLists[ i ].back() )
			{
				qDebug() << "Start " << *start << " is list end." ;

				break;
			}

			/*
			if ( GetPointLocationOverPolygon( start->GetPosition(), pol1 ) == PointLocationOverPolygon::Outside )
			{
				qDebug() << "Start " << *start << " is outside the polygon." ;

				continue;
			}
			*/

			MLSPointData * currentPoint = GetNextPointData( start, pointDataLists[ lineSegmentIndex ] );
			qDebug() << "Current PD : " << *currentPoint << currentPoint ;
			/*
			if ( GetPointLocationOverPolygon( currentPoint->GetPosition(), pol1 ) == PointLocationOverPolygon::Outside )
			{
				qDebug() << "Current " << *currentPoint << " is outside the polygon." ;

				continue;
			}
			*/
			std::list< QPointF > vertices;
			vertices.push_back( start->GetPosition() );
			vertices.push_back( currentPoint->GetPosition() );
			LineSegmentIndexWithPointData const pair( lineSegmentIndex, *currentPoint );
			if ( std::find( pairs.begin(), pairs.end(), pair ) == pairs.end() )
			{
				pairs.push_back( pair );
			}
			while ( true )
			{
				uint16_t nextLineSegmentIndex = GetNextLineSegmentIndex( currentPoint, lineSegmentIndex, lineSegments );
				qDebug() << "Next list index : " << nextLineSegmentIndex ;
				MLSPointData * nextPoint = GetNextPointData( currentPoint, pointDataLists[ nextLineSegmentIndex ] );

				if ( !nextPoint )
				{
					qDebug() << "nextpoint == nullptr" ;

					break;
				}
				qDebug() << "Next PD : " << *nextPoint ;

				if ( std::find( pairs.begin(), pairs.end(), LineSegmentIndexWithPointData( nextLineSegmentIndex, *nextPoint ) ) != pairs.end() )
				{
					qDebug() << "Pair is already exist." ;

					break;
				}

				/*
				Shapes::LineSegment const ls = lineSegments[ lineSegmentIndex ];
				Math::Vector const v1( ls.p1(), ls.p2() );
				Math::Vector const normal = v1.GetNormal( Math::CircleDirection::Counterclockwise, Math::CircleDirection::Clockwise );
				Math::Vector const v2( currentPoint->GetPosition(), nextPoint->GetPosition() );
				if ( normal * v2 < 0 )
				{
					qDebug() << "< 0" ;

					break;
				}
				*/

				if ( *nextPoint == *start )
				{
					qDebug() << "\nComplete polygon.\n" ;

					std::vector< QPointF > const v( vertices.begin(), vertices.end() );
					if ( GetPolygonVerticesDirection( v, baseOrientation ) == Math::CircleDirection::Clockwise )
					{
						result.push_back( GeomShapes::Polygon( vertices ) );
					}

					break;
				}
				/*
				else if ( GetPointLocationOverPolygon( nextPoint->GetPosition(), pol1 ) == PointLocationOverPolygon::Outside )
				{
					qDebug() << "nextpoint is outside the polygon." ;

					break;
				}
				*/
				else
				{
					vertices.push_back( nextPoint->GetPosition() );
				}

				qDebug() << "Vertices : " ;
				SHOW_stdlist( vertices );

				qDebug() << "Pairs : " ;
				SHOW_stdlist( pairs );
				qDebug() << "\n" ;

				pairs.push_back( LineSegmentIndexWithPointData( nextLineSegmentIndex, *nextPoint ) );

				lineSegmentIndex = nextLineSegmentIndex;
				currentPoint = nextPoint;
			}
		}
	}

	return result;
}

std::list< GeomShapes::Polygon > PolygonAlgorithms::GetCharacteristicRegionForInside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation )
{
	std::list< GeomShapes::LineSegment > const movingLineSegments = GetMotionLineSegmentsForInside( pol1, pol2, p, baseOrientation );

	return GetGeneratedRegion( movingLineSegments, baseOrientation );
}

std::list< GeomShapes::Polygon > PolygonAlgorithms::GetComplementToCharacteristicRegionForOutside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation )
{
	std::list< GeomShapes::LineSegment > const movingLineSegments = GetMotionLineSegmentsForOutside( pol1, pol2, p, baseOrientation );

	return GetGeneratedRegion( movingLineSegments, baseOrientation );
}



PolygonAlgorithms::SlabSystem::SlabSystem( GeomShapes::Polygon const & polygon )
{
    std::vector< QPointF > vertices = polygon.GetVertices();
    std::sort( vertices.begin(), vertices.end(), []( QPointF const & p1, QPointF const & p2 ) {
        return p1.x() < p2.x();
    } );

	std::list< double > xcoords;
	xcoords.push_back( vertices[ 0 ].x() );
	std::list< Slab > slabs;
	for ( int i = 0; i < vertices.size() - 1; ++i )
	{
		double x1 = vertices[ i ].x();
		double x2 = vertices[ i + 1 ].x();
		if ( x1 != x2 )
		{
			xcoords.push_back( x2 );
			slabs.push_back( Slab( x1, x2 ) );
		}
	}

	m_slabsBorderVerticesSets.reserve( xcoords.size() );
	double x = vertices[ 0 ].x();
	std::set< QPointF, PointsComparer > points;
	points.insert( vertices[ 0 ] );
	for ( int i = 1; i < vertices.size(); ++i )
	{
		double x1 = vertices[ i ].x();
		if ( x1 != x )
		{
			m_slabsBorderVerticesSets.push_back( points );
			points.clear();
			points.insert( vertices[ i ] );
			x = x1;
		}
		else
		{
			points.insert( vertices[ i ] );
		}
	}
	m_slabsBorderVerticesSets.push_back( points );

	m_xcoords.reserve( xcoords.size() );
	for ( double x : xcoords )
	{
		m_xcoords.push_back( x );
	}

	m_slabs.reserve( slabs.size() );
	for ( Slab const & s : slabs  )
	{
		m_slabs.push_back( s );
	}

    for ( Slab & s : m_slabs )
    {
		for ( GeomShapes::LineSegment const & edge : polygon.GetEdges() )
        {
            if ( s.Intersects( edge ) )
            {
                QPointF const p1 = Math::GetPointOnLine( edge.GetQLineF(), s.x1() );
                QPointF const p2 = Math::GetPointOnLine( edge.GetQLineF(), s.x2() );

                s.AddLineSegment( SlabLineSegment( p1, p2 ) );
            }
        }
        s.SortLineSegments();
    }
}

bool PolygonAlgorithms::Slab::Intersects( GeomShapes::LineSegment const & ls )
{
    return ( ( ls.p1().x() <= m_x1 && m_x2 <= ls.p2().x() ) ||
             ( ls.p2().x() <= m_x1 && m_x2 <= ls.p1().x() ) );
}

void PolygonAlgorithms::Slab::SortLineSegments()
{
    std::sort( m_lineSegments.begin(), m_lineSegments.end(), [] ( SlabLineSegment const & sls1, SlabLineSegment const & sls2 ) {
		return sls1 < sls2;
    } );
}

qreal PolygonAlgorithms::SlabLineSegment::GetLeftY() const
{
    if ( p1().x() < p2().x() )
    {
        return p1().y();
    }
    else
    {
        return p2().y();
    }
}

QPointF const & PolygonAlgorithms::SlabLineSegment::GetLeftPoint() const
{
	return ( p1().x() < p2().x() ? p1() : p2() );
}

QPointF const & PolygonAlgorithms::SlabLineSegment::GetRightPoint() const
{
	return ( p1().x() < p2().x() ? p2() : p1() );
}

bool PolygonAlgorithms::SlabLineSegment::IsUnderPoint( QPointF const & p ) const
{
	QPointF const p0 = Math::GetPointOnLine( GetQLineF(), p.x() );

	return ( p0.y() < p.y() );
}

bool PolygonAlgorithms::SlabLineSegment::IsOverPoint( QPointF const & p ) const
{
	QPointF const p0 = Math::GetPointOnLine( GetQLineF(), p.x() );

	return ( p0.y() > p.y() );
}

bool PolygonAlgorithms::SlabLineSegment::Contains( QPointF const & p ) const
{
	QPointF const p0 = Math::GetPointOnLine( GetQLineF(), p.x() );

	return qFuzzyCompare( p0.y(), p.y() );
}

bool PolygonAlgorithms::SlabLineSegment::operator <( PolygonAlgorithms::SlabLineSegment const & other ) const
{
	return ( GetMidpoint().y() < other.GetMidpoint().y() );
}



QDebug operator<<( QDebug debug, PolygonAlgorithms::Slab const & slab )
{
	debug.nospace() << "[ " << slab.x1() << ", " << slab.x2() << " ]" ;

	return debug;
}

QDebug operator<<( QDebug debug, PolygonAlgorithms::SlabLineSegment const & sls )
{
	debug.nospace() << "( " << sls.GetLeftPoint() << ", " << sls.GetRightPoint() << " )" ;

	return debug;
}

QDebug operator<<( QDebug debug, PolygonAlgorithms::PointLocationOverPolygon location )
{
	switch ( location )
	{
	case PolygonAlgorithms::PointLocationOverPolygon::Boundary :
	{
		debug << "Point is boundary." ;

		break;
	}
	case PolygonAlgorithms::PointLocationOverPolygon::Inside :
	{
		debug << "Point is inside the polygon." ;

		break;
	}
	case PolygonAlgorithms::PointLocationOverPolygon::Outside :
	{
		debug << "Point is outside the polygon." ;

		break;
	}
	case PolygonAlgorithms::PointLocationOverPolygon::Vertex :
	{
		debug << "Point is polygon's vertex." ;

		break;
	}
	}

	return debug;
}

QDebug operator<<( QDebug debug, Math::Vector const & v )
{
	debug.nospace() << "Math::Vector( " << v.X() << ", " << v.Y() << " )" ;

	return debug;
}

QDebug operator<<( QDebug debug, GeomShapes::LineSegment const & ls )
{
	debug.nospace() << "Shapes::LineSegment( " << ls.p1() << ", " << ls.p2() << " )" ;

	return debug;
}

QDebug operator<<( QDebug debug, PolygonAlgorithms::MLSPointData const & pd )
{
	debug.nospace() << "MLSPointData : position - " << pd.GetPosition() << ", indexes - " ;
	for ( uint16_t index : pd.GetLineSegmentsIndexes() )
	{
		debug.nospace() << index << " " ;
	}

	return debug;
}

QDebug operator<<( QDebug debug, PolygonAlgorithms::LineSegmentIndexWithPointData const & pair )
{
	debug.nospace() << "Pair : [ " << pair.m_index << ", " << pair.m_pointData << " ]" ;

	return debug;
}

QDebug operator<<( QDebug debug, Math::CircleDirection cd )
{
	switch ( cd )
	{
	case Math::CircleDirection::Clockwise :
	{
		debug.nospace() << "Clockwise." ;

		break;
	}
	case Math::CircleDirection::Counterclockwise :
	{
		debug.nospace() << "Counterclockwise." ;

		break;
	}
	}

	return debug;
}

QDebug operator<<( QDebug debug, PolygonAlgorithms::TwoPolygonsLocation loc )
{
	switch ( loc )
	{
	case PolygonAlgorithms::TwoPolygonsLocation::FirstIsInsideSecond :
	{
		debug.nospace() << "FirstIsInsideSecond" ;

		break;
	}
	case PolygonAlgorithms::TwoPolygonsLocation::Intersection :
	{
		debug.nospace() << "Intersection" ;

		break;
	}
	case PolygonAlgorithms::TwoPolygonsLocation::Outside :
	{
		debug.nospace() << "Outside" ;

		break;
	}
	case PolygonAlgorithms::TwoPolygonsLocation::SecondIsInsideFirst :
	{
		debug.nospace() << "SecondIsInsideFirst" ;

		break;
	}
	}

	return debug;
}




