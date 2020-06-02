#include "GraphicsShapesBuilders/Additional/linesegmentmidpointbuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/helpers.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"



void GSBuilders::Additional::LineSegmentMidpointBuilder::SetFirstPoint( GraphicsPoint * p )
{
    m_p1 = p;
    m_firstPointIsSelected = true;
}

void GSBuilders::Additional::LineSegmentMidpointBuilder::SetSecondPoint( GraphicsPoint * p )
{
    m_p2 = p;
}

void GSBuilders::Additional::LineSegmentMidpointBuilder::CreateMidpoint()
{
    m_midpoint = new GraphicsPoint( GSHelpers::GetMidpoint( m_p1, m_p2 ) );
    //m_midpoint->SetAsLineSegmentMidpoint( true );

    //PlaneScene::StorePoint( m_midpoint );
    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_midpoint );
}

void GSBuilders::Additional::LineSegmentMidpointBuilder::Reset()
{
    *this = LineSegmentMidpointBuilder();
}

