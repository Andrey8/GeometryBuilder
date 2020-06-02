#pragma once

#include "BSEHandlers/buildingstepseventshandler.h"
#include "BSEHandlers/parallellsbsehandler.h"
#include "BSEHandlers/tangentlsbsehandler.h"

class GraphicsCircle;



namespace GraphicsShapesBSEHandlers
{
	class HandlerForCircle :
			public GraphicsShapesBSEHandler,
			public ParallelLSBSEHandler,
			public TangentLSBSEHandler
	{
	public:
		HandlerForCircle() : m_circle( nullptr ) {}

		virtual void HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent *, GB::GraphicsShapeType ) const;
		virtual void HandleParallelLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleTangentLSAdding( QGraphicsSceneMouseEvent * ) const override;
		//void HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * );

		void SetCircle( GraphicsCircle * circle ) { m_circle = circle; }
	private:
		GraphicsCircle * m_circle;
	};
}
