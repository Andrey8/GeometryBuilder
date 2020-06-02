#include "planescene.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicspolygon.h"
#include "DebugHelpers/debughelper.h"
#include "interactionobjects.h"
#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"
#include "GraphicsShapesBuilders/polygonbuilder.h"
#include "GraphicsShapesBuilders/objects.h"
#include "GraphicsShapesBuilders/Additional/linesegmentextender.h"
#include "BSEHandlers/bsehandlerforscene.h"

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>
#include <QLineF>
#include <QMessageBox>
#include <QPainter>
#include <QDebug>



PlaneScene::PlaneScene()
    : m_polygonBuilder( &GSBuilders::Objects::GetPolygonBuilder() ),
      m_lineSegmentExtender( new GSBuilders::Additional::LineSegmentExtender() ),
      m_BSEHandler( new GraphicsShapesBSEHandlers::HandlerForScene( this ) )
{}



void PlaneScene::Clear()
{
    clear();
}

void PlaneScene::ExtendLineSegment( GraphicsLineSegment * ls )
{
    GSBuilders::Additional::LineSegmentExtender * lse = m_lineSegmentExtender;

    lse->SetLineSegment( ls );
    lse->CreateLine();

    addItem( lse->GetLine() );

    removeItem( ls );
    //lse.DestroyLineSegment();

    lse->Reset();
}



void PlaneScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    QGraphicsScene::mousePressEvent( event );

    //DebugHelper::ShowTextInInput( "oppa" );

    foreach ( GraphicsPoint * point, Helpers::Objects::GetConstructionData().GetGraphicsPoints() )
    {
        if ( point->isUnderMouse() )
        {
            //DebugHelper::ShowTextInInput( "scene" );
            return;
        }
    }
    foreach ( GraphicsLineSegment * ls, Helpers::Objects::GetConstructionData().GetGraphicsLineSegments() )
    {
        if ( ls->isUnderMouse() )
        {
            return;
        }
    }
    foreach ( GraphicsLine * line, Helpers::Objects::GetConstructionData().GetGraphicsLines() )
    {
        if ( line->isUnderMouse() )
        {
            return;
        }
    }
    foreach ( GraphicsCircle * circle, Helpers::Objects::GetConstructionData().GetGraphicsCircles() )
    {
        if ( circle->isUnderMouse() )
        {
            return;
        }
    }

    bool cursorAbovePointThatIsNotPolygonStart = false;
    foreach ( GraphicsPoint * vertex, Helpers::Objects::GetConstructionData().GetGraphicsPoints() )
    {
        if ( vertex != m_polygonBuilder->GetStartPoint() &&
                vertex->contains( event->scenePos() - vertex->scenePos() ) )
        {
            cursorAbovePointThatIsNotPolygonStart = true;

            break;
        }
    }



    switch ( InteractionObjects::userActionMode )
    {
    case GB::UserActionMode::Building :
    {
        switch ( InteractionObjects::buildingButtonsData.GetPushedBuildingButton() )
        {
        case GB::PushedBuildingButton::AddPoint :
        {
            if ( event->button() == Qt::LeftButton )
            {
                //DebugHelper::ShowTextInInput( "scene" );

                m_BSEHandler->HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Point );
            }

            break;
        }
        case GB::PushedBuildingButton::DrawLineSegment :
        {
            if ( event->button() == Qt::LeftButton )
            {
                //DebugHelper::ShowTextInInput( "oppa" );
                m_BSEHandler->HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::LineSegment );
            }

            break;
        }
        case GB::PushedBuildingButton::DrawLine :
        {
            if ( event->button() == Qt::LeftButton )
            {
                m_BSEHandler->HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Line );
            }

            break;
        }
        case GB::PushedBuildingButton::AddCircle :
        {
            if ( InteractionObjects::GetCircleBuildingType() == GB::CircleBuildingType::ThroughThreePoints )
            {
                break;
            }

            if ( event->button() == Qt::LeftButton )
            {
                m_BSEHandler->HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Circle );
            }

            break;
        }
        case GB::PushedBuildingButton::DrawPolygon :
        {
            if ( event->button() == Qt::LeftButton )
            {
                m_BSEHandler->HandleGraphicsShapeAdding( event, GB::GraphicsShapeType::Polygon );
            }

            break;
        }
        case GB::PushedBuildingButton::AddPerpendicular :
        {
            if ( event->button() == Qt::LeftButton )
            {
                m_BSEHandler->HandlePerpendicularLSAdding( event );
            }

            break;
        }
        case GB::PushedBuildingButton::AddParallel :
        {
            if ( event->button() == Qt::LeftButton )
            {
                m_BSEHandler->HandleParallelLSAdding( event );
            }

            break;
        }
        case GB::PushedBuildingButton::AddTangent :
        {
            if ( event->button() == Qt::LeftButton )
            {
                //DebugHelper::ShowTextInInput( "In scene" );

                m_BSEHandler->HandleTangentLSAdding( event );
            }

            break;
        }
        }

        break;
    }
    case GB::UserActionMode::None :
    {
        break;
    }
    }
}



void PlaneScene::Test()
{
    GraphicsPoint * p1 = new GraphicsPoint( 100, 100 );
    GraphicsPoint * p2 = new GraphicsPoint( 400, 200 );
    GraphicsPoint * p3 = new GraphicsPoint( 200, 500 );
    GraphicsLineSegment * ls12 = new GraphicsLineSegment( p1, p2 );
    GraphicsLineSegment * ls23 = new GraphicsLineSegment( p2, p3 );
    GraphicsLineSegment * ls31 = new GraphicsLineSegment( p3, p1 );

    std::vector< GraphicsPoint * > points = { p1, p2, p3 };
    std::vector< GraphicsLineSegment * > edges = { ls12, ls23, ls31 };
    GraphicsPolygon * pol = new GraphicsPolygon( points, edges );
    addItem( pol );

    GraphicsPoint * a = new GraphicsPoint( 300, 300 );
    GraphicsPoint * b = new GraphicsPoint( 500, 500 );
    GraphicsLineSegment * ab = new GraphicsLineSegment( a, b );
    addItem( ab );
}

