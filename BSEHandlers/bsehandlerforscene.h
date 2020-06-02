#pragma once

#include "BSEHandlers/buildingstepseventshandler.h"
#include "BSEHandlers/perpendicularlsbsehandler.h"
#include "BSEHandlers/parallellsbsehandler.h"
#include "BSEHandlers/tangentlsbsehandler.h"

class PlaneScene;



namespace GraphicsShapesBSEHandlers
{
	class HandlerForScene :
			public GraphicsShapesBSEHandler,
			public PerpendicularLSBSEHandler,
			public ParallelLSBSEHandler,
			public TangentLSBSEHandler
	{
	public:
		HandlerForScene( PlaneScene * scene )
			: m_scene( scene )
		{}

		virtual void HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent *, GB::GraphicsShapeType ) const override;
		virtual void HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleParallelLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleTangentLSAdding( QGraphicsSceneMouseEvent * ) const override;
	private:
		PlaneScene * const m_scene;
	};
}
