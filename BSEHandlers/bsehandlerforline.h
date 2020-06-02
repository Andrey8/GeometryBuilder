#pragma once

#include "BSEHandlers/buildingstepseventshandler.h"
#include "BSEHandlers/perpendicularlsbsehandler.h"
#include "BSEHandlers/parallellsbsehandler.h"
#include "BSEHandlers/tangentlsbsehandler.h"

class GraphicsLine;



namespace GraphicsShapesBSEHandlers
{
	class HandlerForLine :
			public GraphicsShapesBSEHandler,
			public PerpendicularLSBSEHandler,
			public ParallelLSBSEHandler,
			public TangentLSBSEHandler
	{
	public:
		HandlerForLine() : m_line( nullptr ) {}

		virtual void HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent *, GB::GraphicsShapeType ) const override;
		virtual void HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleParallelLSAdding( QGraphicsSceneMouseEvent * ) const override;
		virtual void HandleTangentLSAdding( QGraphicsSceneMouseEvent * ) const override;
		void HandleLineSegmentMidpointAdding( QGraphicsSceneMouseEvent * );

		void SetLine( GraphicsLine * line ) { m_line = line; }
	private:
		GraphicsLine * m_line;
	};
}
