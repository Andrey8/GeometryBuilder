#pragma once

//#include "gbuildernamespace.h"

class QGraphicsSceneMouseEvent;



namespace IntersectionPointsBSEHandlers
{
	class IntersectionPointsBuildingStepsEventsHandler
	{
	public:
		virtual void HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * ) = 0;
	};
}
