#pragma once

namespace IntersectionPointsBSEHandlers { class HandlerForCircle; }
namespace IntersectionPointsBSEHandlers { class HandlerForLine; }
namespace IntersectionPointsBSEHandlers { class HandlerForLineSegment; }



namespace IntersectionPointsBSEHandlers
{
	namespace Objects
	{
		HandlerForCircle & GetHandlerForCircle();
		HandlerForLine & GetHandlerForLine();
		HandlerForLineSegment & GetHandlerForLineSegment();
	}
}
