#pragma once

#include "BSEHandlers/buildingstepseventshandler.h"
#include "BSEHandlers/perpendicularlsbsehandler.h"
#include "BSEHandlers/parallellsbsehandler.h"
#include "BSEHandlers/tangentlsbsehandler.h"

class GraphicsLineSegment;



namespace GraphicsShapesBSEHandlers
{
	class HandlerForLineSegment :
			public GraphicsShapesBSEHandler,
			public PerpendicularLSBSEHandler,
			public ParallelLSBSEHandler,
			public TangentLSBSEHandler
	{
	public:
		HandlerForLineSegment() : m_lineSegment( nullptr ) {}

		virtual void HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent *, GB::GraphicsShapeType ) const override;
		virtual void HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleParallelLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleTangentLSAdding( QGraphicsSceneMouseEvent * ) const override;
		void HandleLineSegmentMidpointAdding( QGraphicsSceneMouseEvent *, GB::GraphicsLineSegmentLocationType );

		void SetLineSegment( GraphicsLineSegment * ls ) { m_lineSegment = ls; }
	private:
		GraphicsLineSegment * m_lineSegment;
	};
}
