#pragma once

#include "BSEHandlers/buildingstepseventshandler.h"
#include "BSEHandlers/perpendicularlsbsehandler.h"
#include "BSEHandlers/parallellsbsehandler.h"
#include "BSEHandlers/tangentlsbsehandler.h"

class GraphicsPoint;



namespace GraphicsShapesBSEHandlers
{
	class HandlerForPoint :
			public GraphicsShapesBSEHandler,
			public PerpendicularLSBSEHandler,
			public ParallelLSBSEHandler,
			public TangentLSBSEHandler
	{
	public:
		HandlerForPoint() : m_point( nullptr ) {}

		virtual void HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent *, GB::GraphicsShapeType ) const override;
		void HandleLineSegmentMidpointAdding( QGraphicsSceneMouseEvent * );
		virtual void HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleParallelLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleTangentLSAdding( QGraphicsSceneMouseEvent * ) const override;

		void SetPoint( GraphicsPoint * point ) { m_point = point; }
	private:
		GraphicsPoint * m_point;
	};
}
