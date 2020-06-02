#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"



Helpers::ConstructionData & Helpers::Objects::GetConstructionData()
{
	static ConstructionData helper;

	return helper;
}
