#pragma once

#include "gbuildernamespace.h"

class QGraphicsSceneMouseEvent;



namespace GraphicsShapesBSEHandlers
{
	class GraphicsShapesBSEHandler
	{
	public:
		virtual void HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent *, GB::GraphicsShapeType ) const = 0;
	};
}
