#pragma once

#include "Shapes/linesegment.h"
#include "Shapes/polygon.h"
#include "mathnamespace.h"

#include <set>

#include <QDebug>

class QPointF;



namespace PolygonAlgorithms
{
    void Tests();
	void Test_GetLocationsInCaseOfTranslations();
	void Test_GetLocationsInCaseOfTranslationsAndRotations();
	void Test_CharacteristicRegionAlgorithm();
	void Test_GetPolygonVerticesDirection();
	void Test_MotionLineSegmentsAlgorithm();
	void Test_PointLocationAlgorithm();

	void SKIP();
	void SHOW_ExpressionValue( bool expression );
	template < typename T > void SHOW_stdvector( std::vector< T > const & );
	template < typename T > void SHOW_stdlist( std::list< T > const & );
	template < typename T > void SHOW_stdset( std::set< T > const & );
	class PointsComparer;
	void SHOW_pointsstdset( std::set< QPointF, PointsComparer > const & );



	enum class PointLocationOverPolygon
	{
		Outside,
		Inside,
		Boundary,
		Vertex
	};

	enum class TwoPolygonsLocation
	{
		FirstIsInsideSecond,
		SecondIsInsideFirst,
		Intersection,
		Outside
	};

	class PointsComparer
	{
	public:
		bool operator() ( QPointF const & p1, QPointF const & p2 ) const
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
	};

	class SlabLineSegment : public GeomShapes::LineSegment
    {
    public:
        SlabLineSegment( QPointF const & p1, QPointF const & p2 )
			: GeomShapes::LineSegment( p1, p2 )
        {}

		bool operator<( SlabLineSegment const & other ) const;
		bool IsUnderPoint( QPointF const & ) const;
		bool IsOverPoint( QPointF const & ) const;
		bool Contains( QPointF const & ) const;

		// getters
        qreal GetLeftY() const;
		QPointF const & GetLeftPoint() const;
		QPointF const & GetRightPoint() const;
    };

    class Slab
    {
    public:
        Slab( double x1, double x2 )
            : m_x1( x1 ),
              m_x2( x2 )
        {}

        // getters
        double x1() const { return m_x1; }
        double x2() const { return m_x2; }
        std::vector< SlabLineSegment > const & GetLineSegments() const { return m_lineSegments; }

        void AddLineSegment( SlabLineSegment const & ls ) { m_lineSegments.push_back( ls ); }
        void SortLineSegments();

        bool Contains( QPointF const & p ) const;
		bool Intersects( GeomShapes::LineSegment const & );

		//friend QDebug & operator<<( QDebug &, Slab const & );
    private:
        double m_x1;
        double m_x2;

        std::vector< SlabLineSegment > m_lineSegments;
	};

    class SlabSystem
    {
    public:
		SlabSystem( GeomShapes::Polygon const & );

        // getters
        std::vector< Slab > const & GetSlabs() const { return m_slabs; }
		std::vector< double > const & GetXCoords() const { return m_xcoords; }
		std::set< QPointF, PointsComparer > const & GetSlabBorderVertices( int index ) const { return m_slabsBorderVerticesSets[ index ]; }

        bool Contains( QPointF const & p ) const;
    private:
        std::vector< double > m_xcoords;
        std::vector< Slab > m_slabs;
		std::vector< std::set< QPointF, PointsComparer > > m_slabsBorderVerticesSets;
    };

	class MLSPointData
	{
	public:
		enum class Place
		{
			Begin, End
		};

		MLSPointData( uint16_t lsIndex1, uint16_t lsIndex2, QPointF const & pos )
			: m_position( pos )
		{
			m_lineSegmentsIndexes.insert( lsIndex1 );
			m_lineSegmentsIndexes.insert( lsIndex2 );
		}

		// getters
		std::set< uint16_t > const & GetLineSegmentsIndexes() const { return m_lineSegmentsIndexes; }
		QPointF const & GetPosition() const { return m_position; }

		bool operator==( MLSPointData const & other ) const { return m_position == other.m_position; }

		bool IsLineSegmentBegin() const;
		bool IsLineSegmentEnd() const;
	private:
		QPointF m_position;
		std::set< uint16_t > m_lineSegmentsIndexes;
		Place m_place;
	};
	class MLSPointDataComparer
	{
	public:
		MLSPointDataComparer( GeomShapes::LineSegment const & ls )
			: m_lineSegment( ls )
		{}

		bool operator()( MLSPointData * pd1, MLSPointData * pd2 ) const;
	private:
		GeomShapes::LineSegment const m_lineSegment;
	};
	struct LineSegmentIndexWithPointData
	{
		LineSegmentIndexWithPointData( uint16_t i, MLSPointData const & pd )
			: m_index( i ),
			  m_pointData( pd )
		{}

		bool operator ==( LineSegmentIndexWithPointData const & other ) const { return ( m_index == other.m_index && m_pointData == other.m_pointData ); }

		uint16_t m_index;
		MLSPointData m_pointData;
	};

	void SortPointDatasAlongLineSegment( std::list< MLSPointData * > & pdsList, GeomShapes::LineSegment const & ls );
	uint16_t GetNextLineSegmentIndex( MLSPointData * pd, uint16_t currentLineSegmentIndex, std::vector< GeomShapes::LineSegment > const & );
	MLSPointData * GetNextPointData( MLSPointData * pd, std::list< MLSPointData * > const & pointDataList );



	PointLocationOverPolygon GetPointLocationOverPolygon( QPointF const &, GeomShapes::Polygon const & );
	PointLocationOverPolygon GetPointLocationOverPolygon( QPointF const &, SlabSystem const & );

	std::list< GeomShapes::LineSegment > GetMotionLineSegmentsForInside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation = Math::CircleDirection::Counterclockwise );
	std::list< GeomShapes::LineSegment > GetMotionLineSegmentsForOutside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation = Math::CircleDirection::Counterclockwise );

	Math::CircleDirection GetPolygonVerticesDirection( std::vector< QPointF > const & points, Math::CircleDirection baseOrientation );

	std::list< GeomShapes::Polygon > GetGeneratedRegion( std::list< GeomShapes::LineSegment > const & movingLineSegments, Math::CircleDirection baseOrientation );
	std::list< GeomShapes::Polygon > GetCharacteristicRegionForInside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation = Math::CircleDirection::Counterclockwise );
	std::list< GeomShapes::Polygon > GetComplementToCharacteristicRegionForOutside( GeomShapes::Polygon const & pol1, GeomShapes::Polygon const & pol2, QPointF const & p, Math::CircleDirection baseOrientation = Math::CircleDirection::Counterclockwise );

	std::list< std::pair< Math::Vector, TwoPolygonsLocation > > GetLocations( GeomShapes::Polygon const & fixed, GeomShapes::Polygon const & moving, std::vector< Math::Vector > const & translations );
	std::list< std::pair< std::pair< Math::Vector, Math::Angle >, TwoPolygonsLocation > > GetLocations( GeomShapes::Polygon const & fixed, GeomShapes::Polygon const & moving, std::vector< std::pair< Math::Vector, Math::Angle > > const & translationsWithAngles );
}
