#include "GraphicsShapesBuilders/pointbuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"
#include "GraphicsShapes/helpers.h"
#include "DebugHelpers/constructioninfowriter.h"
#include "DebugHelpers/objects.h"



GraphicsPoint * GSBuilders::PointBuilder::GetPoint()
{
    if ( m_point )
    {
        return m_point;
    }
    else
    {
        // throw exception
    }
}

void GSBuilders::PointBuilder::CreatePointOnFreePlace( QPointF const & center )
{
    if ( !m_point )
    {
        m_point = new GraphicsPoint( center );

        GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_point );
        DebugHelper::Objects::GetConstructionInfoWriter().WriteToFile( m_point );
    }
    else
    {
        // throw exception
    }
}

void GSBuilders::PointBuilder::CreatePointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * segment )
{
    GraphicsLine * line = new GraphicsLine( segment->p1(), segment->p2() );
    QPointF const center = GSHelpers::GetPerpendicularBasePoint( scenePos, line );

    m_point = new GraphicsPoint( center );

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_point );
}

void GSBuilders::PointBuilder::CreatePointOnCircle( QPointF const & scenePos, GraphicsCircle * c )
{
    QLineF const l( c->GetCenterPosition(), scenePos );

    m_point = new GraphicsPoint( l.pointAt( c->GetRadius() / l.length() ) );

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_point );
}

void GSBuilders::PointBuilder::Reset()
{
    m_point = nullptr;
}

