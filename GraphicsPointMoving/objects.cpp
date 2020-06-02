#include "GraphicsPointMoving/objects.h"
#include "GraphicsPointMoving/movingmanager.h"



GPM::MovingManager & GPM::Objects::GetMovingManager()
{
    static MovingManager mm;

    return mm;
}

