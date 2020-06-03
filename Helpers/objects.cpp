#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"



GeomConstr::ConstructionData & GeomConstr::Objects::GetConstructionData()
{
	static ConstructionData helper;

	return helper;
}
