#include "BSEHandlers/bsehandlerforlinesegment.h"
#include "gbuildernamespace.h"
#include "planescene.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicspoint.h"
#include "interactionobjects.h"
#include "GraphicsShapesBuilders/pointbuilder.h"
#include "GraphicsShapesBuilders/linesegmentbuilder.h"
#include "GraphicsShapesBuilders/linebuilder.h"
#include "GraphicsShapesBuilders/circlebuilder.h"
#include "GraphicsShapesBuilders/polygonbuilder.h"
#include "GraphicsShapesBuilders/objects.h"
#include "GraphicsShapesBuilders/Additional/linesegmentmidpointbuilder.h"
#include "GraphicsShapesBuilders/Additional/perpendicularlinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/tangentlinesegmentbuilder.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"
//#include "GraphicsShapesBuilders/";

#include <QGraphicsScene>



void GraphicsShapesBSEHandlers::HandlerForLineSegment::HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent * event, GB::GraphicsShapeType type ) const
{
    PlaneScene * scene = static_cast< PlaneScene * >( m_lineSegment->scene() );

    switch ( type )
    {
    case GB::GraphicsShapeType::Point :
    {
        GSBuilders::PointBuilder & pb = GSBuilders::Objects::GetPointBuilder();

        pb.CreatePointOnLineSegment( event->scenePos(), m_lineSegment );
        scene->addItem( pb.GetPoint() );

        m_lineSegment->PutPoint( pb.GetPoint() );

        pb.Reset();

        //m_lineSegment->update();

        break;
    }
    case GB::GraphicsShapeType::LineSegment :
    {
        GSBuilders::LineSegmentBuilder & lsb = GSBuilders::Objects::GetLineSegmentBuilder();

        if ( !lsb.FirstPointIsAdded() )
        {
            lsb.CreateFirstPointOnLineSegment( event->scenePos(), m_lineSegment );
            scene->addItem( lsb.GetFirstPoint() );

            m_lineSegment->PutPoint( lsb.GetFirstPoint() );

            lsb.SetLineSegmentAsFirstPointOwner( m_lineSegment );

            lsb.CreateTempPoint( event->scenePos() );

            lsb.CreateTempLineSegment();
            scene->addItem( lsb.GetTempLineSegment() );

            m_lineSegment->update();
        }
        else
        {
            scene->removeItem( lsb.GetTempLineSegment() );
            lsb.DestroyTempLineSegment();

            lsb.DestroyTempPoint();

            lsb.CreateSecondPointOnLineSegment( event->scenePos(), m_lineSegment );
            scene->addItem( lsb.GetSecondPoint() );

            m_lineSegment->PutPoint( lsb.GetSecondPoint() );

            if ( lsb.FirstPointIsOnLineSegment( m_lineSegment ) )
            {
                lsb.Reset();
                m_lineSegment->update();

                return;
            }

            lsb.CreateLineSegment();
            scene->addItem( lsb.GetLineSegment() );

            lsb.AttachLineSegmentToItsEndPoints();

            lsb.Reset();

            m_lineSegment->update();
        }

        break;
    }
    case GB::GraphicsShapeType::Line :
    {
        GSBuilders::LineBuilder & lb = GSBuilders::Objects::GetLineBuilder();

        if ( !lb.FirstPointIsAdded() )
        {
            lb.CreateFirstPointOnLineSegment( event->scenePos(), m_lineSegment );
            scene->addItem( lb.GetFirstPoint() );

            m_lineSegment->PutPoint( lb.GetFirstPoint() );

            lb.SetLineSegmentAsFirstPointOwner( m_lineSegment );

            lb.CreateTempPoint( event->scenePos() );

            lb.CreateTempLine();
            scene->addItem( lb.GetTempLine() );

            m_lineSegment->update();
        }
        else
        {
            //DebugHelper::ShowTextInInput( "oppa" );

            scene->removeItem( lb.GetTempLine() );
            lb.DestroyTempLine();

            lb.DestroyTempPoint();

            if ( lb.FirstPointIsOnLineSegment( m_lineSegment ) )
            {
                lb.ShowMessageAboutBuildingOnLineSegment();

                return;
            }

            lb.CreateSecondPointOnLineSegment( event->scenePos(), m_lineSegment );
            scene->addItem( lb.GetSecondPoint() );

            m_lineSegment->PutPoint( lb.GetSecondPoint() );

            lb.CreateLine();
            scene->addItem( lb.GetLine() );
            scene->addItem( lb.GetLine()->GetInitialSegment() );

            lb.Reset();

            m_lineSegment->update();
        }

        break;
    }
    case GB::GraphicsShapeType::Circle :
    {
        GSBuilders::CircleBuilder & cb = GSBuilders::Objects::GetCircleBuilder();

        switch ( InteractionObjects::GetCircleBuildingType() )
        {
        case GB::CircleBuildingType::OnlyCircle :
        {
            break;
        }
        case GB::CircleBuildingType::WithCenter :
        {
            if ( !cb.CenterIsAdded() )
            {
                cb.CreateCenterOnLineSegment( event->scenePos(), m_lineSegment );
                scene->addItem( cb.GetCenter() );

                m_lineSegment->PutPoint( cb.GetCenter() );

                cb.CreateTempCircle();
                scene->addItem( cb.GetTempCircle() );
            }

            break;
        }
        case GB::CircleBuildingType::ThroughThreePoints :
        {
            break;
        }
        }

        break;
    }
    case GB::GraphicsShapeType::Polygon :
    {
        GSBuilders::PolygonBuilder & polb = GSBuilders::Objects::GetPolygonBuilder();

        if ( !polb.StartPointIsAdded() )
        {
            polb.CreateStartPoint( event->scenePos() );
            scene->addItem( polb.GetStartPoint() );

            m_lineSegment->PutPoint( polb.GetStartPoint() );

            polb.CreateTempPoint( event->scenePos() );

            polb.CreateFirstTempSide();
            scene->addItem( polb.GetTempSideFromLastVertex() );

            m_lineSegment->update();
        }
        else
        {
            scene->removeItem( polb.GetTempSideFromLastVertex() );
            polb.DestroyTempSideFromLastVertex();
            if ( polb.GetVerticesCount() >= 2 )
            {
                scene->removeItem( polb.GetTempSideFromStartPoint() );
                polb.DestroyTempSideFromStartPoint();
            }
            polb.DestroyTempPoint();

            polb.CreateVertex( event->scenePos() );
            scene->addItem( polb.GetVertex() );

            m_lineSegment->PutPoint( polb.GetVertex() );

            polb.CreateSide();
            scene->addItem( polb.GetSide() );

            polb.CreateTempPoint( event->scenePos() );

            polb.CreateTempSideFromLastVertex();
            scene->addItem( polb.GetTempSideFromLastVertex() );
            if ( polb.GetVerticesCount() >= 2 )
            {
                polb.CreateTempSideFromStartPoint();
                scene->addItem( polb.GetTempSideFromStartPoint() );
            }

            m_lineSegment->update();
        }
    }
    }
}

void GraphicsShapesBSEHandlers::HandlerForLineSegment::HandleLineSegmentMidpointAdding( QGraphicsSceneMouseEvent * event, GB::GraphicsLineSegmentLocationType type )
{
    PlaneScene * scene = static_cast< PlaneScene * >( m_lineSegment->scene() );

    GSBuilders::Additional::LineSegmentMidpointBuilder & builder =
        GSBuilders::Objects::GetLineSegmentMidpointBuilder();

    switch ( type )
    {
    case GB::GraphicsLineSegmentLocationType::Containing :
    {
        builder.SetFirstPoint( m_lineSegment->p1() );
        builder.SetSecondPoint( m_lineSegment->p2() );

        builder.CreateMidpoint();

        scene->addItem( builder.GetMidpoint() );

        m_lineSegment->PutPoint( builder.GetMidpoint() );

        builder.Reset();

        //m_lineSegment->update();

        break;
    }
    case GB::GraphicsLineSegmentLocationType::LineSegmentPart :
    {
        builder.SetFirstPoint( m_lineSegment->p1() );
        builder.SetSecondPoint( m_lineSegment->p2() );

        builder.CreateMidpoint();

        scene->addItem( builder.GetMidpoint() );

        m_lineSegment->GetContainingLineSegment()->PutPoint(
            builder.GetMidpoint() );

        builder.Reset();

        break;
    }
    case GB::GraphicsLineSegmentLocationType::LinePart :
    {
        break;
    }
    }



}

void GraphicsShapesBSEHandlers::HandlerForLineSegment::HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::PerpendicularLineSegmentBuilder & builder =
        GSBuilders::Objects::GetPerpendicularLineSegmentBuilder();

    if ( builder.SourcePointIsSelected() )
    {
        GraphicsPoint * sourcePoint = builder.GetSourcePoint();

        builder.GetSourcePoint()->SetPaintMode(
            GB::PointPaintMode::Usual );

        builder.SetLineSegment( m_lineSegment );

        if ( Helpers::Objects::GetConstructionData().GetInteriorPoints( m_lineSegment ).contains( sourcePoint ) ||
                m_lineSegment->p1() == sourcePoint ||
                m_lineSegment->p2() == sourcePoint )
        {
            builder.CreateTempPoint();
            builder.CreateTempPerpendicular();

            builder.GetTempPerpendicular()->SetPaintMode(
                GB::LineSegmentPaintMode::DotLine );

            m_lineSegment->scene()->addItem( builder.GetTempPoint() );
            m_lineSegment->scene()->addItem( builder.GetTempPerpendicular() );

            builder.NeedToShowTempPerpendicular();

            return;
        }

        builder.CreatePerpendicularToLineSegment();

        m_lineSegment->scene()->addItem( builder.GetPerpendicular() );
        m_lineSegment->scene()->addItem( builder.GetBasePoint() );
        m_lineSegment->scene()->update();

        builder.Reset();
    }
}

void GraphicsShapesBSEHandlers::HandlerForLineSegment::HandleParallelLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::ParallelLineSegmentBuilder & builder =
        GSBuilders::Objects::GetParallelLineSegmentBuilder();

    if ( !builder.TargetIsSelected() )
    {
        builder.SetTargetLineSegment( m_lineSegment );

        m_lineSegment->SetPaintMode(
            GB::LineSegmentPaintMode::Selected );
    }
    else
    {
        if ( !builder.FirstEndIsSelected() )
        {
            builder.CreateFirstEndPointOnLineSegment( event->scenePos(), m_lineSegment );
            m_lineSegment->scene()->addItem( builder.GetFirstEndPoint() );

            m_lineSegment->PutPoint( builder.GetFirstEndPoint() );

            builder.CreateTempPoint();
            builder.CreateTempLineSegment();

            builder.GetTempLineSegment()->SetPaintMode(
                GB::LineSegmentPaintMode::DotLine );

            m_lineSegment->scene()->addItem( builder.GetTempPoint() );
            m_lineSegment->scene()->addItem( builder.GetTempLineSegment() );

            builder.NeedToShowTempLineSegment();

            m_lineSegment->scene()->update();
        }
    }
}

void GraphicsShapesBSEHandlers::HandlerForLineSegment::HandleTangentLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::TangentLineSegmentBuilder & builder =
        GSBuilders::Objects::GetTangentLineSegmentBuilder();

    if ( !builder.SourcePointIsSelected() )
    {
        builder.CreateSourcePointOnLineSegment(
            event->scenePos(),
            m_lineSegment );
        m_lineSegment->scene()->addItem( builder.GetSourcePoint() );

        m_lineSegment->PutPoint( builder.GetSourcePoint() );

        builder.GetSourcePoint()->SetPaintMode(
            GB::PointPaintMode::AroundCircle );
    }
}






