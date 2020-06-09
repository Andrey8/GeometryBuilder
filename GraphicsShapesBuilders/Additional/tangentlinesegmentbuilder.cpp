#include "GraphicsShapesBuilders/Additional/tangentlinesegmentbuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/helpers.h"
#include "DebugHelpers/debughelper.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"



void GSBuilders::Additional::TangentLineSegmentBuilder::CreateSourcePoint( QPointF const & scenePos )
{
    m_sourcePoint = new GraphicsPoint( scenePos );
    m_sourcePointIsSelected = true;

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_sourcePoint );
}

void GSBuilders::Additional::TangentLineSegmentBuilder::CreateSourcePointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * ls )
{
    m_sourcePoint = new GraphicsPoint(
        GSHelpers::GetPerpendicularBasePoint(
            scenePos, ls ) );
    m_sourcePointIsSelected = true;

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_sourcePoint );
}

void GSBuilders::Additional::TangentLineSegmentBuilder::CreateSourcePointOnLine( QPointF const & scenePos, GraphicsLine * line )
{
    m_sourcePoint = new GraphicsPoint(
        GSHelpers::GetPerpendicularBasePoint(
            scenePos, line ) );
    m_sourcePointIsSelected = true;

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_sourcePoint );
}

void GSBuilders::Additional::TangentLineSegmentBuilder::CreateSourcePointOnCircle( QPointF const & scenePos, GraphicsCircle * circle )
{
    m_sourcePoint = new GraphicsPoint(
        GSHelpers::GetPointOnCircle(
            scenePos, circle ) );
    m_sourcePointIsSelected = true;

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_sourcePoint );
}



void GSBuilders::Additional::TangentLineSegmentBuilder::CreateTangentPoints()
{
    std::pair< QPointF const, QPointF const > const points =
        GSHelpers::GetTangentPoints(
            m_sourcePoint->GetPosition(),
            m_circle );

    m_tangentPoint1 = new GraphicsPoint( points.first );
    m_tangentPoint2 = new GraphicsPoint( points.second );
}

void GSBuilders::Additional::TangentLineSegmentBuilder::CreateTangentLineSegments()
{
    m_tangentLS1 = new GraphicsLineSegment( m_sourcePoint, m_tangentPoint1 );
    m_tangentLS2 = new GraphicsLineSegment( m_sourcePoint, m_tangentPoint2 );
}

void GSBuilders::Additional::TangentLineSegmentBuilder::SetSourcePoint( GraphicsPoint * point )
{
    m_sourcePoint = point;
    m_sourcePointIsSelected = true;
}



void GSBuilders::Additional::TangentLineSegmentBuilder::Reset()
{
    m_sourcePoint = nullptr;
    m_sourcePointIsSelected = false;
    m_circle = nullptr;
    m_tangentPoint1 = nullptr;
    m_tangentPoint2 = nullptr;
    m_tangentLS1 = nullptr;
    m_tangentLS2 = nullptr;
}



void GSBuilders::Additional::TangentLineSegmentBuilder::ShowMessageAboutImpossibleBuilding()
{
    QMessageBox::warning( 0,
                          QObject::tr( "Message" ),
                          QObject::tr( "It is impossible!" ) );
}
