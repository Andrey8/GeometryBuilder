#pragma once

#include "mathnamespace.h"

#include <list>
#include <map>
#include <cstdint>
#include <QList>

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsLine;
class GraphicsCircle;
class GraphicsPolygon;
namespace GeomConstr { struct GraphicsLineSegmentData; }
namespace GPM { class CircleWithGuidePointType; }
namespace GPM { enum class CircleMovingGuidePointType; }



namespace GeomConstr
{
    struct CircleWithAngle
    {
        CircleWithAngle( GraphicsCircle * c, Math::Angle const & a )
            : m_circle( c ),
              m_angle( a )
        {}

        GraphicsCircle * m_circle;
        Math::Angle m_angle;
    };

    struct GraphicsPointData
    {
        std::list< GraphicsLineSegment * > m_definedLineSegments;
        std::list< GraphicsLineSegment * > m_nondefinedLineSegments;
        std::list< GraphicsLine * > m_definedLines;
        std::list< GraphicsLine * > m_nondefinedLines;
        std::list< GraphicsCircle * > m_nondefinedCircles;
        std::list< CircleWithAngle > m_nondefinedCirclesWithAngles;
        std::list< GPM::CircleWithGuidePointType * > m_definedCirclesWithData;
        std::list< GraphicsPolygon * > m_polygons;
    };



	class ConstructionData
	{
		typedef std::map< GraphicsPoint *, GraphicsPointData * >::iterator PointsWithDatasMapIterator;
		typedef std::map< GraphicsLineSegment *, GraphicsLineSegmentData * >::iterator LSWithDatasMapIterator;
	public:
		// adders
		void AddLineSegmentAsDefined( GraphicsPoint *, GraphicsLineSegment * );
		void AddLineSegmentAsNondefined( GraphicsPoint *, GraphicsLineSegment * );
		void AddLineAsDefined( GraphicsPoint *, GraphicsLine * );
		void AddLineAsNondefined( GraphicsPoint *, GraphicsLine * );
		void AddCircleAsDefined( GraphicsPoint *, GraphicsCircle *, GPM::CircleMovingGuidePointType type );
		void AddCircleAsNondefined( GraphicsPoint *, GraphicsCircle * );
		void AddPolygon( GraphicsPoint *, GraphicsPolygon * pol );
		void AddPointAsLineSegmentInterior( GraphicsPoint *, GraphicsLineSegment * );

		void RemoveDefinedLineSegment( GraphicsPoint *, GraphicsLineSegment * );
		void RemoveNondefinedLineSegment( GraphicsPoint *, GraphicsLineSegment * );

		void CalculateAngleInNondefinedCirclesOf( GraphicsPoint * p );

		// getters
		std::list< GraphicsPoint * > const & GetGraphicsPoints() const { return m_graphicsPoints; }
		std::list< GraphicsLineSegment * > const & GetGraphicsLineSegments() const { return m_graphicsLineSegments; }
		std::list< GraphicsLine * > const & GetGraphicsLines() const { return m_graphicsLines; }
		std::list< GraphicsCircle * > const & GetGraphicsCircles() const { return m_graphicsCircles; }
		std::list< GraphicsPolygon * > const & GetGraphicsPolygons() const { return m_graphicsPolygons; }
		Math::Angle const & GetAngleInNondefinedCircle( GraphicsPoint *, GraphicsCircle * );
		std::list< GraphicsLineSegment * > const & GetDefinedLineSegments( GraphicsPoint * ) const;
		std::list< GraphicsLineSegment * > const & GetNondefinedLineSegments( GraphicsPoint * ) const;
		std::list< GraphicsLine * > const & GetDefinedLines( GraphicsPoint * ) const;
		std::list< GraphicsLine * > const & GetNondefinedLines( GraphicsPoint * ) const;
		std::list< GPM::CircleWithGuidePointType * > const & GetDefinedCirclesWithData( GraphicsPoint * ) const;
		std::list< GraphicsCircle * > const & GetNondefinedCircles( GraphicsPoint * ) const;
		uint16_t GetNondefinedShapesCount( GraphicsPoint * ) const;
		std::list< GraphicsPolygon * > const & GetPolygons( GraphicsPoint * ) const;
		QList< GraphicsPoint * > const & GetInteriorPoints( GraphicsLineSegment * ) const;

		// setters
		void SetGLSAsLinePart( GraphicsLineSegment *, bool isLinePart );
		void SetGLSAsLineSegmentPart( GraphicsLineSegment *, bool isLineSegmentPart );
		void SetGLSAsContaining( GraphicsLineSegment *, bool isContaining );
		void SetGLSContainingLine( GraphicsLineSegment *, GraphicsLine * line );
		void SetGLSContainingLineSegment( GraphicsLineSegment * dest, GraphicsLineSegment * ls );

		// questions
		bool PointIsPolygonVertex( GraphicsPoint * ) const;

		// storers
		void StoreGraphicsPoint( GraphicsPoint * );
		void StoreGraphicsLineSegment( GraphicsLineSegment * );
		void StoreGraphicsLine( GraphicsLine * line ) { m_graphicsLines.push_back( line ); }
		void StoreGraphicsCircle( GraphicsCircle * circle ) { m_graphicsCircles.push_back( circle ); }
		void StoreGraphicsPolygon( GraphicsPolygon * pol ) { m_graphicsPolygons.push_back( pol ); }
	private:
		std::list< GraphicsPoint * > m_graphicsPoints;
		std::list< GraphicsLineSegment * > m_graphicsLineSegments;
		std::list< GraphicsLine * > m_graphicsLines;
		std::list< GraphicsCircle * > m_graphicsCircles;
		std::list< GraphicsPolygon * > m_graphicsPolygons;

		std::map< GraphicsPoint *, GraphicsPointData * > m_pointsWithDatas;
		std::map< GraphicsLineSegment *, GraphicsLineSegmentData * > m_lineSegmentsWithDatas;
	};
}
