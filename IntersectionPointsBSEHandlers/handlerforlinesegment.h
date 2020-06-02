#pragma once

#include "IntersectionPointsBSEHandlers/intersectionpointsbuildingstepseventshandler.h"

class GraphicsLineSegment;



namespace IntersectionPointsBSEHandlers
{
	class HandlerForLineSegment : public IntersectionPointsBuildingStepsEventsHandler
	{
	public:
		HandlerForLineSegment() : m_lineSegment( nullptr ) {}

		virtual void HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * );

		void SetLineSegment( GraphicsLineSegment * ls ) { m_lineSegment = ls; }
	private:
		GraphicsLineSegment * m_lineSegment;
	};
}


