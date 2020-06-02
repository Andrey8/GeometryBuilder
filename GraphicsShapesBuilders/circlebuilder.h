#pragma once

#include "gbuildernamespace.h"

class GraphicsPoint;
class GraphicsCircle;
class GraphicsLineSegment;

class QPointF;



namespace GSBuilders
{
	class CircleBuilder
	{
	public:
		CircleBuilder();

		// questions
		bool CenterIsAdded() const { return m_centerIsAdded; }
		bool FirstPointIsSelected() const { return m_p1 != nullptr; }
		bool SecondPointIsSelected() const { return m_p2 != nullptr; }
		bool ThirdPointIsSelected() const { return m_p3 != nullptr; }

		// setters
		void SetCenterPos( QPointF const & c );
		void SetCenter( GraphicsPoint * );
		void SetRadius( qreal r ) { m_radius = r; }
		void SetTempRadius( qreal r );
		void SetFirstPoint( GraphicsPoint * p ) { m_p1 = p; }
		void SetSecondPoint( GraphicsPoint * p ) { m_p2 = p; }
		void SetThirdPoint( GraphicsPoint * p ) { m_p3 = p; }

		// getters
		GraphicsCircle * GetTempCircle() const { return m_tempCircle; }
		GraphicsCircle * GetCircle() const { return m_circle; }
		QPointF const & GetCenterPos() const { return m_centerPos; }
		GraphicsPoint * GetCenter() const { return m_center; }
		GraphicsPoint * GetFirstPointFromTriple() const { return m_p1; }
		GraphicsPoint * GetSecondPointFromTriple() const { return m_p2; }

		// creators
		void CreateTempCircle();
		void CreateCircle();
		void CreateCircle( GraphicsPoint * pointOnCircle );
		void CreateCircle( GB::CircleBuildingType type );
		void CreateCenter();
		void CreateCenterOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * );
		void CreateCenterOnOtherCircle( QPointF const & scenePos, GraphicsCircle * c );

		void DestroyTempCircle();

		void Reset();
	private:
		QPointF m_centerPos;
		qreal m_radius;
		GraphicsCircle * m_tempCircle;
		GraphicsCircle * m_circle;
		GraphicsPoint * m_center;
		bool m_centerIsAdded;

		GraphicsPoint * m_p1;
		GraphicsPoint * m_p2;
		GraphicsPoint * m_p3;
	};
}
