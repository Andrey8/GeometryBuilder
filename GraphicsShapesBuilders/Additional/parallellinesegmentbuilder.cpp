#include "GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/helpers.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"



void GSBuilders::Additional::ParallelLineSegmentBuilder::CreateFirstEndPoint( QPointF const & pos )
{
    m_firstEndPoint = new GraphicsPoint( pos );
    m_firstEndIsSelected = true;

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_firstEndPoint );
}

void GSBuilders::Additional::ParallelLineSegmentBuilder::CreateSecondEndPoint( QPointF const & pos )
{
    m_secondEndPoint = new GraphicsPoint( pos );

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_secondEndPoint );
}

void GSBuilders::Additional::ParallelLineSegmentBuilder::CreateTempPoint()
{
    m_tempPoint = new GraphicsPoint( m_firstEndPoint->GetPosition() );
}

void GSBuilders::Additional::ParallelLineSegmentBuilder::CreateTempLineSegment()
{
    m_tempLineSegment = new GraphicsLineSegment(
        m_firstEndPoint,
        m_tempPoint );
}

void GSBuilders::Additional::ParallelLineSegmentBuilder::CreateParallelLineSegment()
{
    m_parallelLineSegment = new GraphicsLineSegment(
        m_firstEndPoint,
        m_secondEndPoint );
}

void GSBuilders::Additional::ParallelLineSegmentBuilder::CreateFirstEndPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * ls )
{
    m_firstEndPoint = new GraphicsPoint(
        GSHelpers::GetPerpendicularBasePoint(
            scenePos, ls ) );
    m_firstEndIsSelected = true;
}

void GSBuilders::Additional::ParallelLineSegmentBuilder::CreateFirstEndPointOnCircle( QPointF const & scenePos, GraphicsCircle * circle )
{
    QLineF const l( circle->GetCenterPosition(), scenePos );

    m_firstEndPoint = new GraphicsPoint( l.pointAt( circle->GetRadius() / l.length() ) );
    m_firstEndIsSelected = true;
}



void GSBuilders::Additional::ParallelLineSegmentBuilder::SetTempPointPosition( QPointF const & pos )
{
    m_tempPoint->SetPosition( pos );
}



void GSBuilders::Additional::ParallelLineSegmentBuilder::DestroyTempLineSegment()
{
    delete m_tempLineSegment;
    m_tempLineSegment = nullptr;
}

void GSBuilders::Additional::ParallelLineSegmentBuilder::DestroyTempPoint()
{
    delete m_tempPoint;
    m_tempPoint = nullptr;
}



void GSBuilders::Additional::ParallelLineSegmentBuilder::Reset()
{
    m_targetLine = nullptr;
    m_targetLineSegment = nullptr;

    m_targetIsSelected = false;
    m_firstEndIsSelected = false;

    m_firstEndPoint = nullptr;
    m_secondEndPoint = nullptr;

    m_needToShowTempLineSegment = false;

    m_tempPoint = nullptr;
    m_tempLineSegment = nullptr;

    m_parallelLineSegment = nullptr;
}
