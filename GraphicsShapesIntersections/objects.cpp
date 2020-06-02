#include "GraphicsShapesIntersections/objects.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"



GSI::IntersectionManager & GSI::GetIntersectionManager()
{
    static IntersectionManager manager;

    return manager;
}
