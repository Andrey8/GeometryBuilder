#include "BSEHandlers/bsehandlerforscene.h"
#include "planescene.h"
#include "interactionobjects.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicslinesegment.h"

#include "GraphicsShapesBuilders/pointbuilder.h"
#include "GraphicsShapesBuilders/linesegmentbuilder.h"
#include "GraphicsShapesBuilders/linebuilder.h"
#include "GraphicsShapesBuilders/circlebuilder.h"
#include "GraphicsShapesBuilders/polygonbuilder.h"
#include "GraphicsShapesBuilders/objects.h"
#include "GraphicsShapesBuilders/Additional/perpendicularlinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/tangentlinesegmentbuilder.h"
//#include "GraphicsShapesBuilders/";



void GraphicsShapesBSEHandlers::HandlerForScene::HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent * event, GB::GraphicsShapeType type ) const
{
    switch ( type )
    {
    case GB::GraphicsShapeType::Point :
    {
        GSBuilders::PointBuilder & pb = GSBuilders::Objects::GetPointBuilder();

        pb.CreatePointOnFreePlace( event->scenePos() );
        m_scene->addItem( pb.GetPoint() );

        pb.Reset();

        m_scene->update();

        break;
    }
    case GB::GraphicsShapeType::LineSegment :
    {
        GSBuilders::LineSegmentBuilder & lsb = GSBuilders::Objects::GetLineSegmentBuilder();

        if ( !lsb.FirstPointIsAdded() )
        {
            lsb.CreateFirstPoint( event->scenePos() );
            m_scene->addItem( lsb.GetFirstPoint() );

            lsb.CreateTempPoint( event->scenePos() );

            lsb.CreateTempLineSegment();
            m_scene->addItem( lsb.GetTempLineSegment() );

            m_scene->update();
        }
        else
        {
            m_scene->removeItem( lsb.GetTempLineSegment() );
            lsb.DestroyTempLineSegment();

            lsb.DestroyTempPoint();

            lsb.CreateSecondPoint( event->scenePos() );
            m_scene->addItem( lsb.GetSecondPoint() );

            //DebugHelper::ShowTextInInput( "oppa" );

            lsb.CreateLineSegment();
            GraphicsLineSegment * ls = lsb.GetLineSegment();
            m_scene->addItem( ls );

            lsb.AttachLineSegmentToItsEndPoints();

            lsb.Reset();

            m_scene->update();
        }

        break;
    }
    case GB::GraphicsShapeType::Line :
    {
        GSBuilders::LineBuilder & lb = GSBuilders::Objects::GetLineBuilder();

        if ( !lb.FirstPointIsAdded() )
        {
            lb.CreateFirstPoint( event->scenePos() );
            m_scene->addItem( lb.GetFirstPoint() );

            lb.CreateTempPoint( event->scenePos() );

            lb.CreateTempLine();

            //DebugHelper::ShowTextInInput( "uuuuuu" );

            m_scene->addItem( lb.GetTempLine() );

            m_scene->update();
        }
        else
        {
            m_scene->removeItem( lb.GetTempLine() );
            lb.DestroyTempLine();

            lb.DestroyTempPoint();

            lb.CreateSecondPoint( event->scenePos() );
            m_scene->addItem( lb.GetSecondPoint() );

            lb.CreateLine();
            m_scene->addItem( lb.GetLine() );
            m_scene->addItem( lb.GetLine()->GetInitialSegment() );

            lb.Reset();

            m_scene->update();
        }

        break;
    }
    case GB::GraphicsShapeType::Circle :
    {
        GSBuilders::CircleBuilder & cb = GSBuilders::Objects::GetCircleBuilder();

        if ( !cb.CenterIsAdded() )
        {
            cb.SetCenterPos( event->scenePos() );

            cb.CreateCenter();
            if ( InteractionObjects::GetCircleBuildingType() == GB::CircleBuildingType::WithCenter )
            {
                m_scene->addItem( cb.GetCenter() );
            }

            cb.CreateTempCircle();
            m_scene->addItem( cb.GetTempCircle() );

            m_scene->update();
        }
        else
        {
            //DebugHelper::ShowTextInInput( "aaaaaa" );
            m_scene->removeItem( cb.GetTempCircle() );
            cb.DestroyTempCircle();


            cb.CreateCircle();
            m_scene->addItem( cb.GetCircle() );

            cb.Reset();

            m_scene->update();
        }

        break;
    }
    case GB::GraphicsShapeType::Polygon :
    {
        GSBuilders::PolygonBuilder & polb = GSBuilders::Objects::GetPolygonBuilder();

        if ( !polb.StartPointIsAdded() )
        {
            polb.CreateStartPoint( event->scenePos() );
            m_scene->addItem( polb.GetStartPoint() );

            polb.CreateTempPoint( event->scenePos() );

            polb.CreateFirstTempSide();
            m_scene->addItem( polb.GetTempSideFromLastVertex() );

            m_scene->update();
        }
        else
        {
            m_scene->removeItem( polb.GetTempSideFromLastVertex() );
            polb.DestroyTempSideFromLastVertex();
            if ( polb.GetVerticesCount() >= 2 )
            {
                m_scene->removeItem( polb.GetTempSideFromStartPoint() );
                polb.DestroyTempSideFromStartPoint();
            }
            polb.DestroyTempPoint();

            polb.CreateVertex( event->scenePos() );
            m_scene->addItem( polb.GetVertex() );

            polb.CreateSide();
            m_scene->addItem( polb.GetSide() );

            polb.CreateTempPoint( event->scenePos() );

            polb.CreateTempSideFromLastVertex();
            m_scene->addItem( polb.GetTempSideFromLastVertex() );
            if ( polb.GetVerticesCount() >= 2 )
            {
                polb.CreateTempSideFromStartPoint();
                m_scene->addItem( polb.GetTempSideFromStartPoint() );
            }

            m_scene->update();
        }

        break;
    }
    }
}

void GraphicsShapesBSEHandlers::HandlerForScene::HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::PerpendicularLineSegmentBuilder & builder =
        GSBuilders::Objects::GetPerpendicularLineSegmentBuilder();

    if ( builder.DoesItNeedToShowTempPerpendicular() )
    {
        builder.NoNeedToShowTempPerpendicular();

        m_scene->removeItem( builder.GetTempPerpendicular() );
        builder.DestroyTempPerpendicular();

        builder.CreateSecondEnd( builder.GetTempPoint()->GetPosition() );
        builder.DestroyTempPoint();
        m_scene->addItem( builder.GetSecondEnd() );

        builder.CreateRisingPerpendicular();
        m_scene->addItem( builder.GetPerpendicular() );

        builder.Reset();

        m_scene->update();
    }
}

void GraphicsShapesBSEHandlers::HandlerForScene::HandleParallelLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::ParallelLineSegmentBuilder & builder =
        GSBuilders::Objects::GetParallelLineSegmentBuilder();

    if ( builder.TargetIsSelected() )
    {
        if ( !builder.FirstEndIsSelected() )
        {
            builder.CreateFirstEndPoint( event->scenePos() );
            m_scene->addItem( builder.GetFirstEndPoint() );

            builder.CreateTempPoint();
            builder.CreateTempLineSegment();

            builder.GetTempLineSegment()->SetPaintMode(
                GB::LineSegmentPaintMode::DotLine );

            m_scene->addItem( builder.GetTempPoint() );
            m_scene->addItem( builder.GetTempLineSegment() );

            builder.NeedToShowTempLineSegment();

            m_scene->update();
        }
        else
        {
            builder.BanTempLineSegmentShowing();

            if ( builder.GetTargetLine() )
            {
                builder.GetTargetLine()->SetPaintMode(
                    GB::LinePaintMode::Usual );
            }
            if ( builder.GetTargetLineSegment() )
            {
                builder.GetTargetLineSegment()->SetPaintMode(
                    GB::LineSegmentPaintMode::Usual );
            }

            m_scene->removeItem( builder.GetTempLineSegment() );
            builder.DestroyTempLineSegment();

            builder.CreateSecondEndPoint( builder.GetTempPoint()->GetPosition() );
            m_scene->removeItem( builder.GetTempPoint() );
            builder.DestroyTempPoint();
            m_scene->addItem( builder.GetSecondEndPoint() );

            builder.CreateParallelLineSegment();
            m_scene->addItem( builder.GetParallelLineSegment() );

            builder.Reset();

            m_scene->update();
        }
    }
}

void GraphicsShapesBSEHandlers::HandlerForScene::HandleTangentLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::TangentLineSegmentBuilder & builder =
        GSBuilders::Objects::GetTangentLineSegmentBuilder();

    if ( !builder.SourcePointIsSelected() )
    {
        builder.CreateSourcePoint( event->scenePos() );

        builder.GetSourcePoint()->SetPaintMode(
            GB::PointPaintMode::AroundCircle );

        m_scene->addItem( builder.GetSourcePoint() );
    }
}




