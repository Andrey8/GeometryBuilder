#pragma once

#include "Shapes/polygon.h"
#include "Shapes/line.h"
#include "Shapes/circle.h"

#include <list>
#include <cmath>

#include <QPointF>
#include <QLineF>
#include <QDebug>

class GVector;
class CoordSystem;



namespace Math
{
    enum CoordsType
    {
        Rect, Polar
    };

    enum class AngleMeasure
    {
        Degrees, Radians
    };

	enum class CircleDirection
	{
		Clockwise,
		Counterclockwise
	};

    enum PolarAngleOfVectorAlgorithm
    {
        WithQLineFAngles, WithArccosine
    };

	enum class TwoLinesLocation
	{
		Intersect, Parallel
	};

	enum class TwoLineSegmentsLocation
	{
		Intersect, ExtensionIntersect, Parallel
	};

	enum class TwoCirclesLocation
	{
		IntersectionInTwoPoints, Touching, NoIntersection
	};

	enum class LineWithLineSegmentLocation
	{
		Intersect, ExtensionIntersect, Parallel
	};

	enum class CircleWithLineLocation
	{
		IntersectionInTwoPoints, Touching, NoIntersection
	};

	enum class CircleWithLineSegmentLocation
	{
		IntersectionInTwoPoints, IntersectionInOnePoint, Touching, NoIntersection
	};



	int GetSignum( double );

	template < typename T > class ValuesTriple
	{
	public:
		ValuesTriple( T const & v1, T const & v2, T const & v3 )
			: m_v1( v1 ),
			  m_v2( v2 ),
			  m_v3( v3 )
		{}

		T const & GetFirst() const { return m_v1; }
		T const & GetSecond() const { return m_v2; }
		T const & GetThird() const { return m_v3; }

		friend QDebug operator<<( QDebug debug, ValuesTriple< T > const & triple )
		{
			debug.nospace() << "ValuesTriple< double > : [ " << triple.m_v1 << ", " << triple.m_v2 << ", " << triple.m_v3 << " ]" ;

			return debug;
		}
	private:
		T const m_v1;
		T const m_v2;
		T const m_v3;
	};

    class Interval
    {
    public:
        // questions
        virtual bool Contains( qreal value ) const = 0;

        Interval( qreal lowValue, qreal highValue )
        {
            if ( lowValue > highValue )
            {
                // throw
            }
            else
            {
                m_lowValue = lowValue;
                m_highValue = highValue;
            }
        }

        virtual ~Interval() {}
    protected:
        qreal m_lowValue;
        qreal m_highValue;
    };

    class OpenInterval : public Interval
    {
    public:
        // questions
        bool Contains( qreal value ) const override { return ( m_lowValue < value && value < m_highValue ); }
        bool IsEmpty() const { return ( m_lowValue == m_highValue ); }

    };

    class ClosedInterval : public Interval
    {
    public:
        // questions
        bool Contains( qreal value ) const override { return ( m_lowValue <= value && value <= m_highValue ); }

    private:
    };

    class LeftClosedInterval : public Interval
    {
    public:
        bool Contains( qreal value ) const override { return ( m_lowValue <= value && value < m_highValue ); }

    };

    class RightClosedInterval : public Interval
    {
    public:
        bool Contains( qreal value ) const override { return ( m_lowValue < value && value <= m_highValue ); }

    };

    class Angle
    {
    public:
        qreal GetRadians() const { return m_radians; }
        qreal GetDegrees() const { return ( m_radians * 180 * M_1_PI ); }

		bool operator<( Angle const & other ) const { return m_radians < other.m_radians; }
		bool operator>( Angle const & other ) const { return m_radians > other.m_radians; }

        explicit Angle( qreal value, AngleMeasure measure = AngleMeasure::Radians )
        {
            switch ( measure )
            {
            case AngleMeasure::Radians :
            {
                m_radians = value;

                break;
            }
            case AngleMeasure::Degrees :
            {
                m_radians = M_PI * ( value / 180 );

                break;
            }
            }
        }

    private:
        qreal m_radians;
    };

	class Vector
	{
	public:
		Vector( double x, double y ) : m_x( x ), m_y( y ) {}
		Vector( QPointF const & beg, QPointF const & end ) : Vector( end.x() - beg.x(), end.y() - beg.y() ) {}

		// getters
		double X() const { return m_x; }
		double Y() const { return m_y; }
		double GetNorm() const { return sqrt( m_x * m_x + m_y * m_y ); }
		//Vector GetNormalized() const;
		Vector GetNormal( CircleDirection baseOrientation, CircleDirection cd ) const
		{
			switch ( baseOrientation )
			{
			case CircleDirection::Clockwise :
			{
				switch ( cd )
				{
				case CircleDirection::Clockwise :
				{
					return Vector( -m_y, m_x );

					break;
				}
				case CircleDirection::Counterclockwise :
				{
					return Vector( m_y, -m_x );

					break;
				}
				}

				break;
			}
			case CircleDirection::Counterclockwise :
			{
				switch ( cd )
				{
				case CircleDirection::Clockwise :
				{
					return Vector( m_y, -m_x );

					break;
				}
				case CircleDirection::Counterclockwise :
				{
					return Vector( -m_y, m_x );

					break;
				}
				}

				break;
			}
			}
		}
		Angle GetRelativeAngle( Vector const & fixed, CircleDirection baseOrientation, CircleDirection ) const;

		// operators
		Vector const operator +( Vector const & other ) const { return Vector( m_x + other.m_x, m_y + other.m_y ); }
		Vector const operator *( double k ) const { return Vector( k * m_x, k * m_y ); }
		double operator *( Vector const & other ) const { return ( m_x * other.m_x + m_y * other.m_y ); }
		Vector operator -() const { return Vector( -m_x, -m_y ); }
		friend QPointF operator +( QPointF const & p, Math::Vector const & v )
		{
			return QPointF( p.x() + v.X(), p.y() + v.Y() );
		}
		friend QPointF operator -( QPointF const & p, Math::Vector const & v )
		{
			return QPointF( p.x() - v.X(), p.y() - v.Y() );
		}

		// questions
		bool IsNull() const { return ( m_x == 0 && m_y == 0 ); }
	private:
		double m_x;
		double m_y;
	};

    class PointInPolar
    {
    public:
        // getters
        Angle const & GetAngle() const { return m_angle; }
        qreal GetRadius() const { return m_radius; }

        PointInPolar( Angle angle, qreal radius )
            : m_angle( angle ),
              m_radius( radius )
        {}
    private:
        Angle m_angle;
        qreal m_radius;
    };



	class TwoLinesIntersectionData
	{
	public:
		TwoLinesLocation GetLocation() const { return m_location; }
		QPointF const & GetIntersectionPoint() const { return *m_intersectionPoint; }

		TwoLinesIntersectionData( TwoLinesLocation l, QPointF * p )
			: m_location( l ),
			  m_intersectionPoint( p )
		{}
	private:
		TwoLinesLocation m_location;
		QPointF * m_intersectionPoint;
	};

	class TwoLineSegmentsIntersectionData
	{
	public:
		TwoLineSegmentsLocation GetLocation() const { return m_location; }
		QPointF const & GetIntersectionPoint() const { return *m_intersectionPoint; }

		TwoLineSegmentsIntersectionData( TwoLineSegmentsLocation l, QPointF * p )
			: m_location( l ),
			  m_intersectionPoint( p )
		{}
	private:
		TwoLineSegmentsLocation m_location;
		QPointF * m_intersectionPoint;
	};

	class TwoCirclesIntersectionData
	{
	public:
		TwoCirclesIntersectionData( TwoCirclesLocation location )
			: m_location( location ),
			  m_intersectionPoint_1( nullptr ),
			  m_intersectionPoint_2( nullptr )
		{}

		TwoCirclesIntersectionData( TwoCirclesLocation location, QPointF * p )
			: m_location( location ),
			  m_intersectionPoint_1( p ),
			  m_intersectionPoint_2( nullptr )
		{}

		TwoCirclesIntersectionData( TwoCirclesLocation location, QPointF * p1, QPointF * p2 )
			: m_location( location ),
			  m_intersectionPoint_1( p1 ),
			  m_intersectionPoint_2( p2 )
		{}

		std::pair< QPointF const, QPointF const > GetIntersectionPointsPair() const
		{
			return std::pair< QPointF const, QPointF const >(
						*m_intersectionPoint_1,
						*m_intersectionPoint_2 );
		}

		QPointF GetOneIntersectionPoint() const
		{
			if ( m_intersectionPoint_1 )
			{
				return *m_intersectionPoint_1;
			}
			else if ( m_intersectionPoint_2 )
			{
				return *m_intersectionPoint_2;
			}

			// throw Exception
		}

		TwoCirclesLocation GetLocation() const { return m_location; }

		~TwoCirclesIntersectionData()
		{
			if ( m_intersectionPoint_1 )
			{
				delete m_intersectionPoint_1;
			}
			if ( m_intersectionPoint_2 )
			{
				delete m_intersectionPoint_2;
			}
		}

	private:
		TwoCirclesLocation m_location;
		QPointF * m_intersectionPoint_1;
		QPointF * m_intersectionPoint_2;
	};

	class LineWithLineSegmentIntersectionData
	{
	public:
		LineWithLineSegmentLocation GetLocation() const { return m_location; }
		QPointF const & GetIntersectionPoint() const { return *m_intersectionPoint; }

		LineWithLineSegmentIntersectionData( LineWithLineSegmentLocation l, QPointF * p )
			: m_location( l ),
			  m_intersectionPoint( p )
		{}
	private:
		LineWithLineSegmentLocation m_location;
		QPointF * m_intersectionPoint;
	};

	class CircleWithLineIntersectionData
	{
	public:
		CircleWithLineIntersectionData( CircleWithLineLocation loc, QPointF const & p1, QPointF const & p2 )
			: m_location( loc ),
			  m_firstIntersectionPoint( new QPointF( p1 ) ),
			  m_secondIntersectionPoint( new QPointF( p2 ) )
		{
			if ( loc != CircleWithLineLocation::IntersectionInTwoPoints )
			{
				throw Exception( "CircleWithLineIntersectionData constructor : ... ." );
			}
		}
		CircleWithLineIntersectionData( CircleWithLineLocation loc, QPointF const & p )
			: m_location( loc ),
			  m_firstIntersectionPoint( new QPointF( p ) ),
			  m_secondIntersectionPoint( nullptr )
		{
			if ( loc != CircleWithLineLocation::Touching )
			{
				throw Exception( "CircleWithLineIntersectionData constructor : ... ." );
			}
		}
		CircleWithLineIntersectionData( CircleWithLineLocation loc )
			: m_location( loc ),
			  m_firstIntersectionPoint( nullptr ),
			  m_secondIntersectionPoint( nullptr )
		{
			if ( loc != CircleWithLineLocation::NoIntersection )
			{
				throw Exception( "CircleWithLineIntersectionData constructor : ... ." );
			}
		}

		CircleWithLineLocation GetLocation() const { return m_location; }
		std::pair< QPointF const, QPointF const > GetIntersectionPointsPair() const
		{
			if ( m_firstIntersectionPoint && m_secondIntersectionPoint )
			{
				return std::pair< QPointF const, QPointF const >( *m_firstIntersectionPoint, *m_secondIntersectionPoint );
			}
		}
		QPointF const & GetOnlyIntersectionPoint() const
		{
			if ( m_firstIntersectionPoint && !m_secondIntersectionPoint )
			{
				return *m_firstIntersectionPoint;
			}
			else if ( !m_firstIntersectionPoint && m_secondIntersectionPoint )
			{
				return *m_secondIntersectionPoint;
			}

			throw Exception( "CircleWithLineIntersectionData::GetOnlyIntersectionPoint : ... ." );
		}

		~CircleWithLineIntersectionData()
		{
			if ( m_firstIntersectionPoint )
			{
				delete m_firstIntersectionPoint;
			}
			if ( m_secondIntersectionPoint )
			{
				delete m_secondIntersectionPoint;
			}
		}
	private:
		CircleWithLineLocation m_location;
		QPointF * m_firstIntersectionPoint;
		QPointF * m_secondIntersectionPoint;
	};

	class CircleWithLineSegmentIntersectionData
	{
	public:
		CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation loc, QPointF const & p1, QPointF const & p2 )
			: m_location( loc ),
			  m_firstIntersectionPoint( new QPointF( p1 ) ),
			  m_secondIntersectionPoint( new QPointF( p2 ) )
		{
			if ( loc != CircleWithLineSegmentLocation::IntersectionInTwoPoints )
			{
				throw Exception( "CircleWithLineSegmentIntersectionData constructor : ... ." );
			}
		}
		CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation loc, QPointF const & p )
			: m_location( loc ),
			  m_firstIntersectionPoint( new QPointF( p ) ),
			  m_secondIntersectionPoint( nullptr )
		{
			if ( loc != CircleWithLineSegmentLocation::Touching && loc != CircleWithLineSegmentLocation::IntersectionInOnePoint )
			{
				throw Exception( "CircleWithLineSegmentIntersectionData constructor : ... ." );
			}
		}
		CircleWithLineSegmentIntersectionData( CircleWithLineSegmentLocation loc )
			: m_location( loc ),
			  m_firstIntersectionPoint( nullptr ),
			  m_secondIntersectionPoint( nullptr )
		{
			if ( loc != CircleWithLineSegmentLocation::NoIntersection )
			{
				throw Exception( "CircleWithLineSegmentIntersectionData constructor : ... ." );
			}
		}

		CircleWithLineSegmentLocation GetLocation() const { return m_location; }
		std::pair< QPointF const, QPointF const > GetIntersectionPointsPair() const
		{
			if ( m_firstIntersectionPoint && m_secondIntersectionPoint )
			{
				return std::pair< QPointF const, QPointF const >( *m_firstIntersectionPoint, *m_secondIntersectionPoint );
			}
		}
		QPointF const & GetOnlyIntersectionPoint() const
		{
			if ( m_firstIntersectionPoint && !m_secondIntersectionPoint )
			{
				return *m_firstIntersectionPoint;
			}
			else if ( !m_firstIntersectionPoint && m_secondIntersectionPoint )
			{
				return *m_secondIntersectionPoint;
			}

			throw Exception( "CircleWithLineIntersectionData::GetOnlyIntersectionPoint : ... ." );
		}

		~CircleWithLineSegmentIntersectionData()
		{
			if ( m_firstIntersectionPoint )
			{
				delete m_firstIntersectionPoint;
			}
			if ( m_secondIntersectionPoint )
			{
				delete m_secondIntersectionPoint;
			}
		}
	private:
		CircleWithLineSegmentLocation m_location;
		QPointF * m_firstIntersectionPoint;
		QPointF * m_secondIntersectionPoint;
	};



	bool operator<( QPointF const &, QPointF const & );

	uint16_t GetRemainder( int a, uint16_t b );

    qreal GetDistance( QPointF const & first, QPointF const & second );

	bool PointsAreNearly( QPointF const & p1, QPointF const & p2, double d );

	QPointF GetPointOnLine( QLineF const & line, qreal x );

    qreal GetScalarProduct( QLineF const & v1, QLineF const & v2 );
    qreal GetScalarProduct( GVector const & first, GVector const & second );

    Angle GetPolarAngleOfVector( QLineF const & v );
    Angle GetPolarAngleOfVector( GVector const & vector, CoordSystem const & system, PolarAngleOfVectorAlgorithm a );

    GVector const operator *( qreal k, GVector const & );

    QPointF GetPointFromPolarToRect( PointInPolar const & point );
    PointInPolar GetPointFromRectToPolar( QPointF const & point );

    PointInPolar GetMappedPointInPolarCoords( QPointF const & point, CoordSystem const & system );
    QPointF GetMappedPointFromMain( QPointF const & point, CoordSystem const & newSystem );
    QPointF GetMappedPointToMain( QPointF const & point, CoordSystem const & oldSystem );
    QPointF GetMappedPoint( QPointF const & point, CoordSystem const & oldSystem, CoordSystem const & newSystem );

    QPointF GetPointOnLineSegment( QPointF const & p1, QPointF const & p2, qreal distanceToFirstPoint );
    QPointF GetPointOutOfLineSegment( QPointF const & nearestEndPoint, QPointF const & p2, qreal distanceToFirstPoint );

	QPointF GetCircumcircleCenter( QPointF const & pos1, QPointF const & pos2, QPointF const & pos3 );
    qreal GetCircumcircleRadius( QPointF const & pos1, QPointF const & pos2, QPointF const & pos3 );
	ValuesTriple< double > GetGeneralCoefficients( GeomShapes::Line const & l );
	//ValuesTriple< double > GetGeneralCoefficients( GraphicsLine * l );

	TwoLineSegmentsIntersectionData GetIntersectionData( GeomShapes::LineSegment const &, GeomShapes::LineSegment const & );
	CircleWithLineIntersectionData GetIntersectionData( GeomShapes::Circle const &, GeomShapes::Line const & );
	CircleWithLineSegmentIntersectionData GetIntersectionData( GeomShapes::Circle const &, GeomShapes::LineSegment const & );
	TwoCirclesIntersectionData GetIntersectionData( GeomShapes::Circle const & c1, GeomShapes::Circle const & c2 );

	QPointF GetPerpendicularBasePoint( QPointF const & from, QLineF const & ls );

	std::pair< QPointF const, QPointF const > GetTangentPoints( QPointF const & sourcePoint, GeomShapes::Circle const & circle );

	bool PointIsOutsideCircle( QPointF const & scenePos, GeomShapes::Circle const & );



	std::list< int > GetBadAngleVertexIndexes( GeomShapes::Polygon const &, CircleDirection baseOrientation );
	std::list< int > GetGoodAngleVertexIndexes( GeomShapes::Polygon const &, CircleDirection baseOrientation );

	template < typename T > T const & GetElement( int index, std::vector< T > const & v );

	void Tests();
	void Test_VectorGetRelativeAngle();
	void Test_GetBadAngleVertexIndexes();
	void Test_GetGeneralCoefficients();
}



template < typename T > T const & Math::GetElement( int index, std::vector< T > const & v )
{
	uint16_t const r = Math::GetRemainder( index, v.size() );

	return v[ r ];
}
