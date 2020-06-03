#include "GraphicsPointMoving/movingdata.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsPointMoving/movingpointsgroup.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"



GPM::MovingData::MovingData()
    : m_movingIsAllowed( true ),

      m_cdata( GeomConstr::Objects::GetConstructionData() )
{}



void GPM::MovingData::SetStartPositionData()
{
	for ( MovingPointGroup & group : m_pointGroups )
	{
		group.SetStartPositionData();

		for ( GraphicsPoint * p : group.GetIntersectionPoints() )
		{
			switch ( group.GetJointShape() )
			{
			case MovingPointGroup::JointShape::Line :
			{
				if ( m_cdata.GetNondefinedLineSegments( p ).size() == 1 )
				{
					m_transportLineSegments.push_back(
								m_cdata.GetNondefinedLineSegments( p ).front() );
				}

				break;
			}
			case MovingPointGroup::JointShape::LineSegment :
			{
				if ( m_cdata.GetNondefinedLineSegments( p ).size() == 2 )
				{
					if ( m_cdata.GetNondefinedLineSegments( p ).front() == group.GetJointLineSegment() )
					{
						m_transportLineSegments.push_back(
									m_cdata.GetNondefinedLineSegments( p ).back() );
					}
					if ( m_cdata.GetNondefinedLineSegments( p ).back() == group.GetJointLineSegment() )
					{
						m_transportLineSegments.push_back(
									m_cdata.GetNondefinedLineSegments( p ).front() );
					}
				}

				break;
			}
			}
		}
	}
}

void GPM::MovingData::Clear()
{
    m_pointGroups.clear();
    m_movingIsAllowed = true;
}

