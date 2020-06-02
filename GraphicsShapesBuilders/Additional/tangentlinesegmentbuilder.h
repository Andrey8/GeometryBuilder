#pragma once

#include <utility>

class GraphicsPoint;
class GraphicsCircle;
class GraphicsLineSegment;
class GraphicsLine;

class QPointF;



namespace GSBuilders
{
	namespace Additional
	{
		class TangentLineSegmentBuilder
		{
		public:
			TangentLineSegmentBuilder()
				: m_sourcePoint( nullptr ),
				  m_sourcePointIsSelected( false ),
				  m_circle( nullptr ),
				  m_tangentPoint1( nullptr ),
				  m_tangentPoint2( nullptr ),
				  m_tangentLS1( nullptr ),
				  m_tangentLS2( nullptr )
			{}

			// questions
			bool SourcePointIsSelected() { return m_sourcePointIsSelected; }

			// creators
			void CreateSourcePoint( QPointF const & scenePos );
			void CreateSourcePointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * );
			void CreateSourcePointOnLine( QPointF const & scenePos, GraphicsLine * );
			void CreateSourcePointOnCircle( QPointF const & scenePos, GraphicsCircle * );
			void CreateTangentPoints();
			void CreateTangentLineSegments();

			// getters
			GraphicsPoint * GetSourcePoint() { return m_sourcePoint; }
			std::pair< GraphicsPoint *, GraphicsPoint * > GetTangentPoints()
			{
				return std::pair< GraphicsPoint *, GraphicsPoint * >( m_tangentPoint1, m_tangentPoint2 );
			}
			std::pair< GraphicsLineSegment *, GraphicsLineSegment * > GetTangentLineSegments()
			{
				return std::pair< GraphicsLineSegment *, GraphicsLineSegment * >( m_tangentLS1, m_tangentLS2 );
			}

			// setters
			void SetCircle( GraphicsCircle * circle ) { m_circle = circle; }
			void SetSourcePoint( GraphicsPoint * point );

			void Reset();

			void ShowMessageAboutImpossibleBuilding();
		private:
			GraphicsPoint * m_sourcePoint;

			bool m_sourcePointIsSelected;

			GraphicsCircle * m_circle;

			GraphicsPoint * m_tangentPoint1;
			GraphicsPoint * m_tangentPoint2;
			GraphicsLineSegment * m_tangentLS1;
			GraphicsLineSegment * m_tangentLS2;
		};
	}
}
