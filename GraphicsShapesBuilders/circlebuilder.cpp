#include "GraphicsShapesBuilders/circlebuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"
#include "GraphicsShapes/helpers.h"
#include "GraphicsPointMoving/movingpointsgroup.h"



GSBuilders::CircleBuilder::CircleBuilder()
    : m_tempCircle( nullptr ),
      m_circle( nullptr ),
      m_center( nullptr ),
      m_centerIsAdded( false ),

      m_p1( nullptr ),
      m_p2( nullptr ),
      m_p3( nullptr )
{}

void GSBuilders::CircleBuilder::SetCenterPos( QPointF const & c )
{
    m_centerPos = c;
    m_centerIsAdded = true;
}

void GSBuilders::CircleBuilder::SetCenter( GraphicsPoint * p )
{
    m_center = p;
    m_centerIsAdded = true;
    m_centerPos = p->GetPosition();
}

void GSBuilders::CircleBuilder::SetTempRadius( qreal r )
{
    m_tempCircle->SetRadius( r );
    m_radius = r;
}

void GSBuilders::CircleBuilder::CreateTempCircle()
{
    m_tempCircle = new GraphicsCircle( m_centerPos, 0 );
    m_tempCircle->SetInBuildingProcess( true );
}

void GSBuilders::CircleBuilder::DestroyTempCircle()
{
    delete m_tempCircle;
    m_tempCircle = nullptr;
}

void GSBuilders::CircleBuilder::CreateCircle()
{
    m_circle = new GraphicsCircle( m_centerPos, m_radius );
    m_circle->SetDefiningObjects( CircleDefiningObjects( m_center ) );
    //m_center->AddCircleAsDefined( m_circle, GPM::CircleMovingGuidePointType::Center );

    GeomConstr::Objects::GetConstructionData().StoreGraphicsCircle( m_circle );
    GeomConstr::Objects::GetConstructionData().AddCircleAsDefined( m_center, m_circle, GPM::CircleMovingGuidePointType::Center );
}

void GSBuilders::CircleBuilder::CreateCircle( GraphicsPoint * p )
{
    m_radius = Math::GetDistance( m_centerPos, p->GetPosition() );

    m_circle = new GraphicsCircle( m_centerPos, m_radius );
    m_circle->SetDefiningObjectsType( GraphicsCircle::DefiningObjectsType::CenterAndPointOnCircle );
    m_circle->SetDefiningObjects( CircleDefiningObjects( m_center, p ) );
    //m_center->AddCircleAsDefined( m_circle, GPM::CircleMovingGuidePointType::Center );
    //p->AddCircleAsDefined( m_circle, GPM::CircleMovingGuidePointType::OnlyPointOnCircle );

    GeomConstr::Objects::GetConstructionData().StoreGraphicsCircle( m_circle );
    GeomConstr::Objects::GetConstructionData().AddCircleAsDefined( m_center, m_circle, GPM::CircleMovingGuidePointType::Center );
    GeomConstr::Objects::GetConstructionData().AddCircleAsDefined( p, m_circle, GPM::CircleMovingGuidePointType::OnlyPointOnCircle );
}

void GSBuilders::CircleBuilder::CreateCircle( GB::CircleBuildingType type )
{
    if ( type == GB::CircleBuildingType::ThroughThreePoints )
    {
        m_centerPos = Math::GetCircumcircleCenter( m_p1->GetPosition(), m_p2->GetPosition(), m_p3->GetPosition() );
        m_radius = Math::GetCircumcircleRadius( m_p1->GetPosition(), m_p2->GetPosition(), m_p3->GetPosition() );

        m_circle = new GraphicsCircle( m_centerPos, m_radius );
        m_circle->SetDefiningObjectsType( GraphicsCircle::DefiningObjectsType::ThreePoints );
        m_circle->SetDefiningObjects( CircleDefiningObjects( m_p1, m_p2, m_p3 ) );

        //m_p1->AddCircleAsDefined( m_circle, GPM::CircleMovingGuidePointType::OneOfThreePointsOnCircle );
        //m_p2->AddCircleAsDefined( m_circle, GPM::CircleMovingGuidePointType::OneOfThreePointsOnCircle );
        //m_p3->AddCircleAsDefined( m_circle, GPM::CircleMovingGuidePointType::OneOfThreePointsOnCircle );

        GeomConstr::Objects::GetConstructionData().StoreGraphicsCircle( m_circle );
        GeomConstr::Objects::GetConstructionData().AddCircleAsDefined( m_p1, m_circle, GPM::CircleMovingGuidePointType::OneOfThreePointsOnCircle );
        GeomConstr::Objects::GetConstructionData().AddCircleAsDefined( m_p2, m_circle, GPM::CircleMovingGuidePointType::OneOfThreePointsOnCircle );
        GeomConstr::Objects::GetConstructionData().AddCircleAsDefined( m_p3, m_circle, GPM::CircleMovingGuidePointType::OneOfThreePointsOnCircle );
    }
}

void GSBuilders::CircleBuilder::CreateCenter()
{
    m_center = new GraphicsPoint( m_centerPos );

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_center );
}

void GSBuilders::CircleBuilder::CreateCenterOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * ls )
{
    m_centerPos = GSHelpers::GetPerpendicularBasePoint( scenePos, ls );
    m_center = new GraphicsPoint( m_centerPos );
    m_centerIsAdded = true;

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_center );
}

void GSBuilders::CircleBuilder::CreateCenterOnOtherCircle( QPointF const & scenePos, GraphicsCircle * c )
{
    QLineF const l( c->GetCenterPosition(), scenePos );

    m_centerPos = l.pointAt( c->GetRadius() / l.length() );
    m_center = new GraphicsPoint( m_centerPos );
    m_centerIsAdded = true;

    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( m_center );
}

void GSBuilders::CircleBuilder::Reset()
{
    m_tempCircle = nullptr;
    m_circle = nullptr;
    m_centerIsAdded = false;
    m_center = nullptr;

    m_p1 = nullptr;
    m_p2 = nullptr;
    m_p3 = nullptr;
}

