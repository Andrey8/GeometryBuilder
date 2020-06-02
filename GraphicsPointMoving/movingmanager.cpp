#include "GraphicsPointMoving/movingmanager.h"
#include "GraphicsPointMoving/movingdata.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicspolygon.h"
#include "GraphicsShapes/helpers.h"
#include "GraphicsPointMoving/movingpointsgroup.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"

#include <QGraphicsSceneMouseEvent>



GPM::MovingManager::MovingManager()
    : m_cdata( Helpers::Objects::GetConstructionData() )
{}



void GPM::MovingManager::AddNewPoint( GraphicsPoint * p )
{
    m_pointsWithDatas.insert( std::make_pair( p, new MovingData() ) );
}



void GPM::MovingManager::CalculateMovingData( GraphicsPoint * p )
{
    MapIterator it = m_pointsWithDatas.find( p );

    if ( it == m_pointsWithDatas.end() )
    {
        throw Exception( "ERROR" );
    }

    MovingData * mdata = it->second;

    if ( m_cdata.GetNondefinedShapesCount( p ) > 1 )
    {
        mdata->DoNotAllowMoving();

        return;
    }

	QList< GPM::MovingPointGroup > groups_1;
	foreach ( GraphicsLine * line, m_cdata.GetDefinedLines( p ) )
	{
		GPM::MovingPointGroup group( line, p );
		group.SetPoints( line->GetNondefiningPoints() );

		groups_1.push_back( group );
	}
	foreach ( GraphicsLineSegment * ls, m_cdata.GetDefinedLineSegments( p ) )
	{
		GPM::MovingPointGroup group( ls, p );
		group.SetPoints( Helpers::Objects::GetConstructionData().GetInteriorPoints( ls ) );

		groups_1.push_back( group );
	}
	foreach ( GPM::CircleWithGuidePointType * pair, m_cdata.GetDefinedCirclesWithData( p ) )
	{
		GraphicsCircle * c = pair->m_circle;
		GPM::MovingPointGroup group( c, pair->m_guideType, p );
		group.SetPoints( c->GetNondefiningPoints() );

        groups_1.push_back( group );
    }

    QList< GPM::MovingPointGroup > allGroups;
    QList< GPM::MovingPointGroup > groups_2;
    bool allMovingPointsAreFound = false;
    while ( !allMovingPointsAreFound )
    {
        uint const n = groups_1.size();

		uint i = 0;
		foreach ( GPM::MovingPointGroup const & group, groups_1 )
		{
			if ( group.IsEmpty() )
			{
				++i;
			}

			foreach ( GraphicsPoint * point, group.GetPoints() )
			{
				foreach ( GraphicsLine * line, m_cdata.GetDefinedLines( point ) )
				{
					GPM::MovingPointGroup group( line, point );
					group.SetPoints( line->GetNondefiningPoints() );

					groups_2.push_back( group );
				}
				foreach ( GraphicsLineSegment * s, m_cdata.GetDefinedLineSegments( point ) )
				{
					GPM::MovingPointGroup group( s, point );
					group.SetPoints( Helpers::Objects::GetConstructionData().GetInteriorPoints( s ) );

					groups_2.push_back( group );
				}
				foreach ( GPM::CircleWithGuidePointType * pair, m_cdata.GetDefinedCirclesWithData( point ) )
				{
					GraphicsCircle * c = pair->m_circle;
					GPM::MovingPointGroup group( c, pair->m_guideType, point );
					group.SetPoints( c->GetNondefiningPoints() );

                    groups_1.push_back( group );
                }
            }
        }

		allGroups.append( groups_1 );
		groups_1 = groups_2;
		groups_2.clear();

        if ( i == n )
        {
            allMovingPointsAreFound = true;
        }
    }
    mdata->SetGroups( allGroups );

    mdata->SetStartPositionData();
}

void GPM::MovingManager::HandleMoveEvent( GraphicsPoint * p, QGraphicsSceneMouseEvent * event )
{
	switch ( m_cdata.GetNondefinedShapesCount( p ) )
	{
	case 0 :
	{
		p->SetPosition( event->scenePos() - p->m_pressPlace );

		break;
	}
	case 1 :
	{
		if ( m_cdata.GetNondefinedLines( p ).size() == 1 )
		{
			GraphicsLine * nondefinedLine = m_cdata.GetNondefinedLines( p ).front();
			QPointF const pos = GSHelpers::GetPerpendicularBasePoint( event->scenePos(), nondefinedLine );

			p->SetPosition( pos );
		}

		if ( m_cdata.GetNondefinedLineSegments( p ).size() == 1 )
		{
			GraphicsLineSegment * nondefinedLineSegment = m_cdata.GetNondefinedLineSegments( p ).front();
			GraphicsLine l( nondefinedLineSegment->p1(), nondefinedLineSegment->p2() );
			QPointF const pos = GSHelpers::GetPerpendicularBasePoint( event->scenePos(), &l );

			p->SetPosition( pos );

			GraphicsLineSegment * s = nondefinedLineSegment;
			s->CalculateExtensionLineSegments();
			if ( s->HasPointOnExtension() )
			{
				s->SetPaintMode( GB::LineSegmentPaintMode::WithExtension );
				s->update();
			}
			else
			{
				s->SetPaintMode( GB::LineSegmentPaintMode::Usual );
			}
		}

        if ( m_cdata.GetNondefinedCircles( p ).size() == 1 )
        {
            GraphicsCircle * c = m_cdata.GetNondefinedCircles( p ).front();

            p->SetPosition( GSHelpers::GetPointOnCircle( event->scenePos(), c ) );
        }

		break;
	}
	}

    MapIterator it = m_pointsWithDatas.find( p );
    MovingData * mdata = it->second;

    p->SetOldPositionAsCurrent();
    foreach ( GPM::MovingPointGroup const & group, mdata->GetPointGroups() )
    {
        foreach ( GraphicsPoint * point, group.GetPoints() )
        {
            point->SetOldPositionAsCurrent();
        }
    }

	foreach ( GPM::MovingPointGroup const & group, mdata->GetPointGroups() )
	{
		switch ( group.GetJointShape() )
		{
		case GPM::MovingPointGroup::JointShape::LineSegment :
		{
			GraphicsLineSegment * ls = group.GetJointLineSegment();
			ls->CalculatePaintData();

			if ( ls->HasPointOnExtension() )
			{
				ls->SetPaintMode( GB::LineSegmentPaintMode::WithExtension );
				ls->update();
			}
			else
			{
				ls->SetPaintMode( GB::LineSegmentPaintMode::Usual );
			}

			break;
		}
		case GPM::MovingPointGroup::JointShape::Line :
		{
			GraphicsLine * l = group.GetJointLine();
			l->CalculatePaintData();

			break;
		}
		}
	}
	for ( GraphicsPolygon * pol : m_cdata.GetPolygons( p ) )
	{
		pol->CalculateSelectionAreaPath();
		pol->CalculateVerticesPositions();
	}

	foreach ( GraphicsLineSegment * s, mdata->GetTransportLineSegments() )
	{
		s->CalculateExtensionLineSegments();
		if ( s->HasPointOnExtension() )
		{
			s->SetPaintMode( GB::LineSegmentPaintMode::WithExtension );
			s->update();
		}
		else
		{
			s->SetPaintMode( GB::LineSegmentPaintMode::Usual );
		}
	}

	foreach ( GPM::MovingPointGroup group, mdata->GetPointGroups() )
	{
		group.Move();

		for ( GraphicsPoint * p : group.GetPoints() )
		{
			if ( m_cdata.PointIsPolygonVertex( p ) )
			{
				for ( GraphicsPolygon * pol : m_cdata.GetPolygons( p ) )
				{
					pol->CalculateSelectionAreaPath();
					pol->CalculateVerticesPositions();
				}
			}
		}
	}
}

void GPM::MovingManager::HandleReleaseEvent( GraphicsPoint * p, QGraphicsSceneMouseEvent * event )
{
    MapIterator it = m_pointsWithDatas.find( p );
    MovingData * mdata = it->second;

    foreach ( GPM::MovingPointGroup const & group, mdata->GetPointGroups() )
    {
        if ( group.GetJointShape() == GPM::MovingPointGroup::JointShape::Circle )
        {
            foreach ( GraphicsPoint * p, group.GetPoints() )
            {
                //p->CalculateAngleInNondefinedCircles();
                m_cdata.CalculateAngleInNondefinedCirclesOf( p );
            }
        }
    }
    //CalculateAngleInNondefinedCircles();
    m_cdata.CalculateAngleInNondefinedCirclesOf( p );

    mdata->Clear();
}



GPM::MovingData * GPM::MovingManager::GetMovingData( GraphicsPoint * p ) const
{
    return m_pointsWithDatas.at( p );
}








