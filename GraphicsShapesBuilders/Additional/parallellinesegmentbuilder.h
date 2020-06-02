#pragma once

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsLine;
class GraphicsCircle;

class QPointF;



namespace GSBuilders
{
	namespace Additional
	{
		class ParallelLineSegmentBuilder
		{
		public:
			ParallelLineSegmentBuilder()
				: m_targetLine( nullptr ),
				  m_targetLineSegment( nullptr ),

				  m_targetIsSelected( false ),
				  m_firstEndIsSelected( false ),

				  m_firstEndPoint( nullptr ),
				  m_secondEndPoint( nullptr ),

				  m_needToShowTempLineSegment( false ),

				  m_tempPoint( nullptr ),
				  m_tempLineSegment( nullptr ),

				  m_parallelLineSegment( nullptr )
			{}

			// setters
			void SetTargetLine( GraphicsLine * line )
			{
				m_targetLine = line;
				m_targetIsSelected = true;
			}
			void SetTargetLineSegment( GraphicsLineSegment * ls )
			{
				m_targetLineSegment = ls;
				m_targetIsSelected = true;
			}
			void SetFirstEndPoint( GraphicsPoint * p )
			{
				m_firstEndPoint = p;
				m_firstEndIsSelected = true;
			}
			void SetSecondEndPoint( GraphicsPoint * p ) { m_secondEndPoint = p; }
			void SetTempPointPosition( QPointF const & pos );

			// questions
			bool TargetIsSelected() const { return m_targetIsSelected; }
			bool FirstEndIsSelected() const { return m_firstEndIsSelected; }
			bool DoesItNeedToShowTempLineSegment() const { return m_needToShowTempLineSegment; }

			// creators
			void CreateFirstEndPoint( QPointF const & pos );
			void CreateSecondEndPoint( QPointF const & pos );
			void CreateTempPoint();
			void CreateTempLineSegment();
			void CreateParallelLineSegment();
			void CreateFirstEndPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * );
			void CreateFirstEndPointOnCircle( QPointF const & scenePos, GraphicsCircle * );

			// getters
			GraphicsPoint * GetFirstEndPoint() const { return m_firstEndPoint; }
			GraphicsPoint * GetSecondEndPoint() const { return m_secondEndPoint; }
			GraphicsLine * GetTargetLine() const { return m_targetLine; }
			GraphicsLineSegment * GetTargetLineSegment() const { return m_targetLineSegment; }
			GraphicsLineSegment * GetTempLineSegment() const { return m_tempLineSegment; }
			GraphicsPoint * GetTempPoint() const { return m_tempPoint; }
			GraphicsLineSegment * GetParallelLineSegment() const { return m_parallelLineSegment; }

			void NeedToShowTempLineSegment() { m_needToShowTempLineSegment = true; }
			void BanTempLineSegmentShowing() { m_needToShowTempLineSegment = false; }

			// destroyers
			void DestroyTempLineSegment();
			void DestroyTempPoint();

			void Reset();
		private:
			GraphicsLine * m_targetLine;
			GraphicsLineSegment * m_targetLineSegment;

			bool m_targetIsSelected;
			bool m_firstEndIsSelected;

			GraphicsPoint * m_firstEndPoint;
			GraphicsPoint * m_secondEndPoint;

			bool m_needToShowTempLineSegment;

			GraphicsPoint * m_tempPoint;
			GraphicsLineSegment * m_tempLineSegment;

			GraphicsLineSegment * m_parallelLineSegment;
		};
	}
}
