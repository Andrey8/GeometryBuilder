#pragma once

class QGraphicsSceneMouseEvent;



namespace GraphicsShapesBSEHandlers
{
	class ParallelLSBSEHandler
	{
	public:
		virtual void HandleParallelLSAdding( QGraphicsSceneMouseEvent * ) const = 0;
	};
}
