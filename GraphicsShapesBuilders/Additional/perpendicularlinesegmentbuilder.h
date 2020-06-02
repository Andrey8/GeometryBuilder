#pragma once

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsLine;

class QPointF;



namespace GSBuilders
{
	namespace Additional
	{
		class PerpendicularLineSegmentBuilder
		{
		public:
			PerpendicularLineSegmentBuilder()
				: m_sourcePoint( nullptr ),
				  m_basePoint( nullptr ),
				  m_lineSegment( nullptr ),
				  m_line( nullptr ),

				  m_perpendicular( nullptr ),

				  m_sourcePointIsSelected( false ),

				  m_tempPerpendicular( nullptr ),
				  m_tempPoint( nullptr ),
				  m_doesItNeedToShowTempPerpendicular( false ),
				  m_secondEnd( nullptr )
			{}

			// creators
			void CreatePerpendicularToLine();
			void CreatePerpendicularToLineSegment();
			void CreateRisingPerpendicular();
			//void CreatePerpendicularFromLineSegment();
			void CreateTempPoint();
			void CreateTempPerpendicular();
			void CreateSecondEnd( QPointF const & pos );

			// getters
			GraphicsPoint * GetSourcePoint() const { return m_sourcePoint; }
			GraphicsLineSegment * GetPerpendicular() const { return m_perpendicular; }
			GraphicsPoint * GetBasePoint() const { return m_basePoint; }
			GraphicsLine * GetTargetLine() const { return m_line; }
			GraphicsLineSegment * GetTargetLineSegment() const { return m_lineSegment; }
			GraphicsLineSegment * GetTempPerpendicular() const { return m_tempPerpendicular; }
			GraphicsPoint * GetTempPoint() const { return m_tempPoint; }
			GraphicsPoint * GetSecondEnd() const { return m_secondEnd; }

			// questions
			bool SourcePointIsSelected() const { return m_sourcePointIsSelected; }
			bool DoesItNeedToShowTempPerpendicular() const { return m_doesItNeedToShowTempPerpendicular; }

			// setters
			void SetSourcePoint( GraphicsPoint * p )
			{
				m_sourcePoint = p;
				m_sourcePointIsSelected = true;
			}
			void SetLineSegment( GraphicsLineSegment * ls ) { m_lineSegment = ls; }
			void SetLine( GraphicsLine * line ) { m_line = line; }
			void SetTempPointPosition( QPointF const & pos );
			void NeedToShowTempPerpendicular() { m_doesItNeedToShowTempPerpendicular = true; }
			void NoNeedToShowTempPerpendicular() { m_doesItNeedToShowTempPerpendicular = false; }

			void DestroyTempPerpendicular();
			void DestroyTempPoint();

			void Reset();
		private:
			GraphicsPoint * m_sourcePoint;
			GraphicsPoint * m_basePoint;

			GraphicsLineSegment * m_lineSegment;
			GraphicsLine * m_line;

			GraphicsLineSegment * m_perpendicular;

			bool m_sourcePointIsSelected;

			GraphicsLineSegment * m_tempPerpendicular;
			GraphicsPoint * m_tempPoint;
			bool m_doesItNeedToShowTempPerpendicular;
			GraphicsPoint * m_secondEnd;
		};
	}
}
