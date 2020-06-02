#pragma once

class QGraphicsSceneMouseEvent;



namespace GraphicsShapesBSEHandlers
{
	class PerpendicularLSBSEHandler
	{
	public:
		virtual void HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * ) const = 0;
	};
}
