#pragma once

class GraphicsPoint;
//class GraphicsLineSegment;



namespace GSBuilders
{
	namespace Additional
	{
		class LineSegmentMidpointBuilder
		{
		public:
			LineSegmentMidpointBuilder()
				: m_firstPointIsSelected( false ),
				  m_p1( nullptr ),
				  m_p2( nullptr ),
				  m_midpoint( nullptr )
			{}

			// questions
			bool FirstPointIsSelected() const { return m_firstPointIsSelected; }

			// setters
			void SetFirstPoint( GraphicsPoint * );
			void SetSecondPoint( GraphicsPoint * );

			// getters
			GraphicsPoint * GetMidpoint() const { return m_midpoint; }
			GraphicsPoint * GetFirstPoint() const { return m_p1; }
			GraphicsPoint * GetSecondPoint() const { return m_p2; }

			void CreateMidpoint();

			void Reset();
		private:
			bool m_firstPointIsSelected;
			GraphicsPoint * m_p1;
			GraphicsPoint * m_p2;
			GraphicsPoint * m_midpoint;

		};
	}
}
