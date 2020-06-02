#pragma once

#include "IntersectionPointsBSEHandlers/intersectionpointsbuildingstepseventshandler.h"

class GraphicsCircle;



namespace IntersectionPointsBSEHandlers
{
	class HandlerForCircle : public IntersectionPointsBuildingStepsEventsHandler
	{
	public:
		HandlerForCircle() : m_circle( nullptr ) {}

		virtual void HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * );

		void SetCircle( GraphicsCircle * circle ) { m_circle = circle; }
	private:
		GraphicsCircle * m_circle;
	};
}
