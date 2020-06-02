#pragma once

#include <vector>

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsCircle;
class GraphicsPolygon;

class QPointF;



namespace GSBuilders
{
	class PolygonBuilder
	{
	public:
		PolygonBuilder()
			: m_polygon( nullptr ),
			  m_startPoint( nullptr ),
			  m_currentVertex( nullptr ),
			  m_lastVertex( nullptr ),
			  m_currentSide( nullptr ),
			  m_tempSideFromLastVertex( nullptr ),
			  m_tempSideFromStartPoint( nullptr ),
			  m_polygonStartPointIsAdded( false ),
			  m_tempPoint( nullptr )
		{}

		// getters
		GraphicsPoint * GetStartPoint();
		GraphicsLineSegment * GetSide();
		GraphicsPoint * GetVertex();
		GraphicsLineSegment * GetTempSideFromLastVertex();
		GraphicsLineSegment * GetTempSideFromStartPoint();
		int GetVerticesCount() { return m_vertices.size(); }
		GraphicsPolygon * GetGraphicsPolygon() { return m_polygon; }

		// setters
		void SetTempPointPosition( QPointF const & c );

		// questions
		bool StartPointIsAdded() { return m_polygonStartPointIsAdded; }
		bool PolygonStartContainsPos( QPointF const & pos );
		bool IsItStartPoint( GraphicsPoint * p );

		void SelectPolygonStart();
		void DesselectPolygonStart();

		// destroyers
		void DestroyTempSideFromLastVertex();
		void DestroyTempSideFromStartPoint();
		void DestroyTempPoint();

		// creators
		void CreateStartPoint( QPointF const & center );
		void CreateTempPoint( QPointF const & center );
		void CreateFirstTempSide();
		void CreateSide();
		void CreateTempSideFromLastVertex();
		void CreateTempSideFromStartPoint();
		void CreateVertex( QPointF const & center );
		void CreateLastSide();
		void CreateGraphicsPolygon();

		void Reset();

		void ShowMessageAboutPolygonVerticesCount();
	private:
		GraphicsPolygon * m_polygon;
		std::vector< GraphicsPoint * > m_vertices;
		std::vector< GraphicsLineSegment * > m_edges;

		GraphicsPoint * m_startPoint;
		GraphicsPoint * m_currentVertex;
		GraphicsPoint * m_lastVertex;
		GraphicsLineSegment * m_currentSide;
		GraphicsLineSegment * m_tempSideFromLastVertex;
		GraphicsLineSegment * m_tempSideFromStartPoint;
		bool m_polygonStartPointIsAdded;
		GraphicsPoint * m_tempPoint;
	};
}
