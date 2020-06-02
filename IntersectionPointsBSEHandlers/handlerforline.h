#pragma once

#include "IntersectionPointsBSEHandlers/intersectionpointsbuildingstepseventshandler.h"

class GraphicsLine;



namespace IntersectionPointsBSEHandlers
{
	class HandlerForLine : public IntersectionPointsBuildingStepsEventsHandler
	{
	public:
		HandlerForLine() : m_line( nullptr ) {}

		virtual void HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * );

		void SetLine( GraphicsLine * line ) { m_line = line; }
	private:
		GraphicsLine * m_line;
	};
}
