#include "interactionobjects.h"
#include "GraphicsShapes/graphicspolygon.h"
#include "GraphicsShapes/graphicspoint.h"



InteractionObjects::BuildingButtonsData InteractionObjects::buildingButtonsData;
GB::UserActionMode InteractionObjects::userActionMode = GB::UserActionMode::None;



void InteractionObjects::BuildingButtonsData::SetPushedBuildingButton_BuildingMode( int buttonId )
{
    SetPushedBuildingButton( buttonId, GB::BuildingButtonGroupObject::BuildingMode );

    m_buttonInBuildingMode = m_button;
}

void InteractionObjects::BuildingButtonsData::SetPushedBuildingButton_PolygonAlgorithmsMode( int buttonId )
{
    SetPushedBuildingButton( buttonId, GB::BuildingButtonGroupObject::PolygonAlgorithmsMode );

    m_buttonInPolygonAlgorithmsMode = m_button;
}

void InteractionObjects::BuildingButtonsData::SetPushedBuildingButton( int buttonId, GB::BuildingButtonGroupObject groupObject )
{
    switch ( groupObject )
    {
    case GB::BuildingButtonGroupObject::BuildingMode :
    {
        switch ( buttonId )
        {
        case 1 :
        {
            m_button = GB::PushedBuildingButton::AddPoint;

            break;
        }
        case 2 :
        {
            m_button = GB::PushedBuildingButton::DrawLineSegment;

            break;
        }
        case 3 :
        {
            m_button = GB::PushedBuildingButton::DrawLine;

            break;
        }
        case 4 :
        {
            m_button = GB::PushedBuildingButton::AddCircle;

            break;
        }
        case 5 :
        {
            m_button = GB::PushedBuildingButton::DrawPolygon;

            break;
        }
        case 6 :
        {
            m_button = GB::PushedBuildingButton::AddIntersectionPoint;

            break;
        }
        case 7 :
        {
            m_button = GB::PushedBuildingButton::ExtendLineSegment;

            break;
        }
        case 8 :
        {
            m_button = GB::PushedBuildingButton::AddLineSegmentMidpoint;

            break;
        }
        case 9 :
        {
            m_button = GB::PushedBuildingButton::AddPerpendicular;

            break;
        }
        case 10 :
        {
            m_button = GB::PushedBuildingButton::AddParallel;

            break;
        }
        case 11 :
        {
            m_button = GB::PushedBuildingButton::AddTangent;

            break;
        }
        }

        break;
    }
    case GB::BuildingButtonGroupObject::PolygonAlgorithmsMode :
    {
        switch ( buttonId )
        {
        case 1 :
        {
            m_button = GB::PushedBuildingButton::DrawPolygon;

            break;
        }
        case 2 :
        {
            m_button = GB::PushedBuildingButton::AddPoint;

            break;
        }
        }

        break;
    }
    }

    userActionMode = GB::UserActionMode::Building;
}



void InteractionObjects::TwoPolygonsSelecter::Deselect()
{
    if ( m_pol1 )
    {
        m_pol1->SetPaintMode( GB::PolygonPaintMode::Usual );
        m_pol1->update();
    }
    if ( m_pol2 )
    {
        m_pol2->SetPaintMode( GB::PolygonPaintMode::Usual );
        m_pol2->update();
    }
    if ( m_p )
    {
        m_p->SetPaintMode( GB::PointPaintMode::Usual );
        m_p->update();
    }

    Reset();
}

void InteractionObjects::TwoPolygonsSelecter::DeselectPolygons()
{
    if ( m_pol1 )
    {
        m_pol1->SetPaintMode( GB::PolygonPaintMode::Usual );
        m_pol1->update();
    }
    if ( m_pol2 )
    {
        m_pol2->SetPaintMode( GB::PolygonPaintMode::Usual );
        m_pol2->update();
    }
}



GB::CircleBuildingType & InteractionObjects::GetCircleBuildingType()
{
    static GB::CircleBuildingType type = GB::CircleBuildingType::WithCenter;

    return type;
}

GB::PerpendicularBuildingType & InteractionObjects::GetPerpendicularBuildingType()
{
    static GB::PerpendicularBuildingType type =
        GB::PerpendicularBuildingType::LineSegment;

    return type;
}

GB::ParallelBuildingType & InteractionObjects::GetParallelBuildingType()
{
    static GB::ParallelBuildingType type =
        GB::ParallelBuildingType::LineSegment;

    return type;
}

GB::TangentShapeBuildingType & InteractionObjects::GetTangentShapeBuildingType()
{
    static GB::TangentShapeBuildingType type =
        GB::TangentShapeBuildingType::LineSegments;

    return type;
}

InteractionObjects::TwoPolygonsSelecter & InteractionObjects::GetTwoPolygonsSelecter()
{
    static TwoPolygonsSelecter s;

    return s;
}
