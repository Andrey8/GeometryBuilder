#pragma once

class QGraphicsSceneMouseEvent;



namespace GraphicsShapesBSEHandlers
{
	class TangentLSBSEHandler
	{
	public:
		virtual void HandleTangentLSAdding( QGraphicsSceneMouseEvent * ) const = 0;
	};
}


