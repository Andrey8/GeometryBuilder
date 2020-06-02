#include "GraphicsShapesBuilders/Additional/perpendicularlinesegmentbuilder.h"
#include "GraphicsShapes/helpers.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"



void GSBuilders::Additional::PerpendicularLineSegmentBuilder::CreatePerpendicularToLine()
{
    m_basePoint = new GraphicsPoint(
        GSHelpers::GetPerpendicularBasePoint(
            m_sourcePoint->GetPosition(),
            m_line ) );

    m_perpendicular = new GraphicsLineSegment(
        m_sourcePoint,
        m_basePoint );
    // register graphics line segment
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::CreatePerpendicularToLineSegment()
{
    m_basePoint = new GraphicsPoint(
        GSHelpers::GetPerpendicularBasePoint(
            m_sourcePoint->GetPosition(),
            m_lineSegment ) );

    m_perpendicular = new GraphicsLineSegment(
        m_sourcePoint,
        m_basePoint );
    // register graphics line segment
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::CreateRisingPerpendicular()
{
    m_perpendicular = new GraphicsLineSegment(
        m_basePoint,
        m_secondEnd );
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::CreateSecondEnd( QPointF const & pos )
{
    m_secondEnd = new GraphicsPoint( pos );
}



void GSBuilders::Additional::PerpendicularLineSegmentBuilder::CreateTempPoint()
{
    m_tempPoint = new GraphicsPoint( m_sourcePoint->GetPosition() );
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::CreateTempPerpendicular()
{
    m_basePoint = m_sourcePoint;

    m_tempPerpendicular = new GraphicsLineSegment(
        m_tempPoint,
        m_basePoint );
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::SetTempPointPosition( QPointF const & pos )
{
    m_tempPoint->SetPosition( pos );
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::DestroyTempPerpendicular()
{
    delete m_tempPerpendicular;
    m_tempPerpendicular = nullptr;
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::DestroyTempPoint()
{
    delete m_tempPoint;
    m_tempPoint = nullptr;
}

void GSBuilders::Additional::PerpendicularLineSegmentBuilder::Reset()
{
    m_sourcePoint = nullptr;
    m_basePoint = nullptr;
    m_lineSegment = nullptr;
    m_line = nullptr;

    m_perpendicular = nullptr;

    m_sourcePointIsSelected = false;

    m_tempPerpendicular = nullptr;
    m_tempPoint = nullptr;
    m_doesItNeedToShowTempPerpendicular = false;
    m_secondEnd = nullptr;
}


