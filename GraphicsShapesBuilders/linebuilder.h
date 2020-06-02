#pragma once

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsLine;
class GraphicsCircle;

class QPointF;


namespace GSBuilders
{
	class LineBuilder
	{
	public:
		LineBuilder()
			: m_line( nullptr ),
			  m_tempLine( nullptr ),
			  m_p1( nullptr ),
			  m_p2( nullptr ),
			  m_firstPointIsAdded( false ),
			  m_tempPoint( nullptr ),
			  m_firstPointOwnerLine( nullptr ),
			  m_firstPointOwnerLineSegment( nullptr )
		{}

		// creators
		void CreateLine();
		void CreateTempLine();
		void CreateFirstPoint( QPointF const & center );
		void CreateFirstPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * s );
		void CreateFirstPointOnCircle( QPointF const & scenePos, GraphicsCircle * c );
		void CreateSecondPoint( QPointF const & center );
		void CreateSecondPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * s );
		void CreateSecondPointOnCircle( QPointF const & scenePos, GraphicsCircle * c );
		void CreateTempPoint( QPointF const & center );

		// setters
		void SetFirstPoint( GraphicsPoint * p );
		void SetSecondPoint( GraphicsPoint * p );
		void SetTempPointCenter( QPointF const & );
		void SetLineAsFirstPointOwner( GraphicsLine * l );
		void SetLineSegmentAsFirstPointOwner( GraphicsLineSegment * ls );

		void RecalculateTempLine();

		void DestroyTempPoint();
		void DestroyTempLine();

		void Reset();

		// questions
		bool FirstPointIsAdded() { return m_firstPointIsAdded; }
		bool FirstPointIsOnLine( GraphicsLine * l );
		bool FirstPointIsOnLineSegment( GraphicsLineSegment * ls );

		// getters
		GraphicsLine * GetLine();
		GraphicsLine * GetTempLine();
		GraphicsPoint * GetFirstPoint();
		GraphicsPoint * GetSecondPoint();
		GraphicsPoint * GetTempPoint();

		int ShowMessageAboutBuildingOnLineSegment();
	private:
		GraphicsLine * m_line;
		GraphicsLine * m_tempLine;
		GraphicsPoint * m_p1;
		GraphicsPoint * m_p2;

		bool m_firstPointIsAdded;
		GraphicsPoint * m_tempPoint;

		GraphicsLine * m_firstPointOwnerLine;
		GraphicsLineSegment * m_firstPointOwnerLineSegment;
	};
}
