#include "objects.h"
#include "DebugHelpers/constructioninfowriter.h"



DebugHelper::ConstructionInfoWriter & DebugHelper::Objects::GetConstructionInfoWriter()
{
    static ConstructionInfoWriter w;

    return w;
}
