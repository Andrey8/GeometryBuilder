#include "IntersectionPointsBSEHandlers/objects.h"
#include "IntersectionPointsBSEHandlers/handlerforcircle.h"
#include "IntersectionPointsBSEHandlers/handlerforline.h"
#include "IntersectionPointsBSEHandlers/handlerforlinesegment.h"



IntersectionPointsBSEHandlers::HandlerForCircle & IntersectionPointsBSEHandlers::Objects::GetHandlerForCircle()
{
	static HandlerForCircle handler;

	return handler;
}

IntersectionPointsBSEHandlers::HandlerForLine & IntersectionPointsBSEHandlers::Objects::GetHandlerForLine()
{
	static HandlerForLine handler;

	return handler;
}

IntersectionPointsBSEHandlers::HandlerForLineSegment & IntersectionPointsBSEHandlers::Objects::GetHandlerForLineSegment()
{
	static HandlerForLineSegment handler;

	return handler;
}
