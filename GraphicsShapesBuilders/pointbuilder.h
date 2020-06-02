#pragma once

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsCircle;

class QPointF;



namespace GSBuilders
{
	class PointBuilder
	{
	public:
		PointBuilder()
			: m_point( nullptr )
		{}

		// creators
		void CreatePointOnFreePlace( QPointF const & center );
		void CreatePointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * segment );
		void CreatePointOnCircle( QPointF const & scenePos, GraphicsCircle * c );

		// getters
		GraphicsPoint * GetPoint();

		void Reset();
	private:
		GraphicsPoint * m_point;
	};
}
