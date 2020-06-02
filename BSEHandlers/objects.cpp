#include "BSEHandlers/objects.h"
#include "BSEHandlers/bsehandlerforpoint.h"
#include "BSEHandlers/bsehandlerforlinesegment.h"
#include "BSEHandlers/bsehandlerforline.h"
#include "BSEHandlers/bsehandlerforcircle.h"



GraphicsShapesBSEHandlers::HandlerForPoint & GraphicsShapesBSEHandlers::Objects::GetHandlerForPoint()
{
	static HandlerForPoint hfp;

	return hfp;
}

GraphicsShapesBSEHandlers::HandlerForLineSegment & GraphicsShapesBSEHandlers::Objects::GetHandlerForLineSegment()
{
	static HandlerForLineSegment hfls;

	return hfls;
}

GraphicsShapesBSEHandlers::HandlerForLine & GraphicsShapesBSEHandlers::Objects::GetHandlerForLine()
{
	static HandlerForLine hfl;

	return hfl;
}

GraphicsShapesBSEHandlers::HandlerForCircle & GraphicsShapesBSEHandlers::Objects::GetHandlerForCircle()
{
	static HandlerForCircle hfc;

	return hfc;
}
