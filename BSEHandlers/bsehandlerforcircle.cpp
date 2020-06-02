#include "BSEHandlers/bsehandlerforcircle.h"
#include "planescene.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/helpers.h"
#include "interactionobjects.h"

#include "GraphicsShapesBuilders/pointbuilder.h"
#include "GraphicsShapesBuilders/linesegmentbuilder.h"
#include "GraphicsShapesBuilders/linebuilder.h"
#include "GraphicsShapesBuilders/circlebuilder.h"
#include "GraphicsShapesBuilders/polygonbuilder.h"
#include "GraphicsShapesBuilders/objects.h"
#include "GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/tangentlinesegmentbuilder.h"
//#include "GraphicsShapesBuilders/";



void GraphicsShapesBSEHandlers::HandlerForCircle::HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent * event, GB::GraphicsShapeType type ) const
{
    PlaneScene * scene = static_cast< PlaneScene * >( m_circle->scene() );

    switch ( type )
    {
    case GB::GraphicsShapeType::Point :
    {
        GSBuilders::PointBuilder & pb = GSBuilders::Objects::GetPointBuilder();

        pb.CreatePointOnCircle( event->scenePos(), m_circle );
        scene->addItem( pb.GetPoint() );

        m_circle->PutPoint( pb.GetPoint() );

        pb.Reset();

        m_circle->update();

        break;
    }
    case GB::GraphicsShapeType::LineSegment :
    {
        GSBuilders::LineSegmentBuilder & lsb = GSBuilders::Objects::GetLineSegmentBuilder();

        if ( !lsb.FirstPointIsAdded() )
        {
            lsb.CreateFirstPointOnCircle( event->scenePos(), m_circle );
            scene->addItem( lsb.GetFirstPoint() );

            m_circle->PutPoint( lsb.GetFirstPoint() );

            lsb.CreateTempPoint( event->scenePos() );

            lsb.CreateTempLineSegment();
            scene->addItem( lsb.GetTempLineSegment() );

            m_circle->update();
        }
        else
        {
            scene->removeItem( lsb.GetTempLineSegment() );
            lsb.DestroyTempLineSegment();

            lsb.DestroyTempPoint();

            lsb.CreateSecondPointOnCircle( event->scenePos(), m_circle );
            scene->addItem( lsb.GetSecondPoint() );

            m_circle->PutPoint( lsb.GetSecondPoint() );

            lsb.CreateLineSegment();
            scene->addItem( lsb.GetLineSegment() );

            lsb.AttachLineSegmentToItsEndPoints();

            lsb.Reset();

            m_circle->update();
        }

        break;
    }
    case GB::GraphicsShapeType::Line :
    {
        GSBuilders::LineBuilder & lb = GSBuilders::Objects::GetLineBuilder();

        if ( !lb.FirstPointIsAdded() )
        {
            lb.CreateFirstPointOnCircle( event->scenePos(), m_circle );
            scene->addItem( lb.GetFirstPoint() );

            m_circle->PutPoint( lb.GetFirstPoint() );

            lb.CreateTempPoint( event->scenePos() );

            lb.CreateTempLine();
            scene->addItem( lb.GetTempLine() );

            m_circle->update();
        }
        else
        {
            scene->removeItem( lb.GetTempLine() );
            lb.DestroyTempLine();

            lb.DestroyTempPoint();

            lb.CreateSecondPointOnCircle( event->scenePos(), m_circle );
            scene->addItem( lb.GetSecondPoint() );

            m_circle->PutPoint( lb.GetSecondPoint() );

            lb.CreateLine();
            scene->addItem( lb.GetLine() );
            scene->addItem( lb.GetLine()->GetInitialSegment() );

            lb.Reset();

            m_circle->update();
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
                cb.CreateCenterOnOtherCircle( event->scenePos(), m_circle );
                scene->addItem( cb.GetCenter() );

                m_circle->PutPoint( cb.GetCenter() );

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


        break;
    }
    }
}

void GraphicsShapesBSEHandlers::HandlerForCircle::HandleParallelLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::ParallelLineSegmentBuilder & builder =
        GSBuilders::Objects::GetParallelLineSegmentBuilder();

    if ( builder.TargetIsSelected() &&
            !builder.FirstEndIsSelected() )
    {
        builder.CreateFirstEndPointOnCircle( event->scenePos(), m_circle );
        m_circle->scene()->addItem( builder.GetFirstEndPoint() );

        m_circle->PutPoint( builder.GetFirstEndPoint() );

        builder.CreateTempPoint();
        builder.CreateTempLineSegment();

        builder.GetTempLineSegment()->SetPaintMode(
            GB::LineSegmentPaintMode::DotLine );

        m_circle->scene()->addItem( builder.GetTempPoint() );
        m_circle->scene()->addItem( builder.GetTempLineSegment() );

        builder.NeedToShowTempLineSegment();

        m_circle->scene()->update();
    }
}

void GraphicsShapesBSEHandlers::HandlerForCircle::HandleTangentLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    PlaneScene * scene = static_cast< PlaneScene * >( m_circle->scene() );

    GSBuilders::Additional::TangentLineSegmentBuilder & builder =
        GSBuilders::Objects::GetTangentLineSegmentBuilder();

    if ( !builder.SourcePointIsSelected() )
    {
        builder.CreateSourcePointOnCircle(
            event->scenePos(), m_circle );
        scene->addItem( builder.GetSourcePoint() );

        m_circle->PutPoint( builder.GetSourcePoint() );

        builder.GetSourcePoint()->SetPaintMode(
            GB::PointPaintMode::AroundCircle );
    }
    else
    {
        builder.GetSourcePoint()->SetPaintMode(
            GB::PointPaintMode::Usual );

        if ( GSHelpers::PointIsOutsideCircle(
                    event->scenePos(), m_circle ) )
        {
            builder.Reset();

            builder.ShowMessageAboutImpossibleBuilding();

            return;
        }

        builder.SetCircle( m_circle );

        builder.CreateTangentPoints();
        builder.CreateTangentLineSegments();

        scene->addItem( builder.GetTangentPoints().first );
        scene->addItem( builder.GetTangentPoints().second );
        scene->addItem( builder.GetTangentLineSegments().first );
        scene->addItem( builder.GetTangentLineSegments().second );

        builder.Reset();
    }
}






