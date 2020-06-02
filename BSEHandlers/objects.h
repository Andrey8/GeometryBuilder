#pragma once

namespace GraphicsShapesBSEHandlers { class HandlerForPoint; }
namespace GraphicsShapesBSEHandlers { class HandlerForLineSegment; }
namespace GraphicsShapesBSEHandlers { class HandlerForLine; }
namespace GraphicsShapesBSEHandlers { class HandlerForCircle; }
//namespace BSEHandlers { class HandlerForPolygon; }



namespace GraphicsShapesBSEHandlers
{
	namespace Objects
	{
		HandlerForPoint & GetHandlerForPoint();
		HandlerForLineSegment & GetHandlerForLineSegment();
		HandlerForLine & GetHandlerForLine();
		HandlerForCircle & GetHandlerForCircle();

		//HandlerForPolygon & GetHandlerForPolygon();
	}
}
