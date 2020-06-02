#include "BSEHandlers/bsehandlerforpoint.h"
#include "gbuildernamespace.h"
#include "planescene.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicspolygon.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
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
//#include "GraphicsShapesBuilders/";

#include <QGraphicsScene>



void GraphicsShapesBSEHandlers::HandlerForPoint::HandleGraphicsShapeAdding(
    QGraphicsSceneMouseEvent * event,
    GB::GraphicsShapeType type ) const
{
    PlaneScene * scene = static_cast< PlaneScene * >( m_point->scene() );

    switch ( type )
    {
    case GB::GraphicsShapeType::Point :
    {
        break;
    }
    case GB::GraphicsShapeType::LineSegment :
    {
        GSBuilders::LineSegmentBuilder & lsb = GSBuilders::Objects::GetLineSegmentBuilder();

        if ( !lsb.FirstPointIsAdded() )
        {
            lsb.SetFirstPoint( m_point );

            lsb.CreateTempPoint( event->scenePos() );

            lsb.CreateTempLineSegment();
            scene->addItem( lsb.GetTempLineSegment() );

            scene->update();
        }
        else
        {
            scene->removeItem( lsb.GetTempLineSegment() );
            lsb.DestroyTempLineSegment();

            lsb.DestroyTempPoint();

            lsb.SetSecondPoint( m_point );

            if ( lsb.GetFirstPoint() == lsb.GetSecondPoint() )
            {
                lsb.Reset();

                return;
            }

            lsb.CreateLineSegment();
            scene->addItem( lsb.GetLineSegment() );

            lsb.AttachLineSegmentToItsEndPoints();

            lsb.Reset();

            scene->update();
        }

        break;
    }
    case GB::GraphicsShapeType::Line :
    {
        GSBuilders::LineBuilder & lb = GSBuilders::Objects::GetLineBuilder();

        if ( !lb.FirstPointIsAdded() )
        {
            lb.SetFirstPoint( m_point );

            lb.CreateTempPoint( event->scenePos() );

            lb.CreateTempLine();
            scene->addItem( lb.GetTempLine() );

            scene->update();
        }
        else
        {
            scene->removeItem( lb.GetTempLine() );
            lb.DestroyTempLine();

            lb.DestroyTempPoint();

            lb.SetSecondPoint( m_point );

            lb.CreateLine();
            scene->addItem( lb.GetLine() );
            scene->addItem( lb.GetLine()->GetInitialSegment() );

            lb.Reset();

            m_point->update();
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
            scene->removeItem( cb.GetTempCircle() );
            cb.DestroyTempCircle();

            cb.CreateCircle( m_point );
            scene->addItem( cb.GetCircle() );

            cb.Reset();

            break;
        }
        case GB::CircleBuildingType::WithCenter :
        {
            if ( !cb.CenterIsAdded() )
            {
                cb.SetCenter( m_point );

                cb.CreateTempCircle();
                scene->addItem( cb.GetTempCircle() );
            }
            else
            {
                scene->removeItem( cb.GetTempCircle() );
                cb.DestroyTempCircle();

                cb.CreateCircle( m_point );
                scene->addItem( cb.GetCircle() );

                cb.Reset();
            }

            break;
        }
        case GB::CircleBuildingType::ThroughThreePoints :
        {
            if ( !cb.FirstPointIsSelected() )
            {
                cb.SetFirstPoint( m_point );
                m_point->SetPaintMode( GB::PointPaintMode::Selected );

                m_point->update();
            }
            else if ( !cb.SecondPointIsSelected() )
            {
                if ( m_point == cb.GetFirstPointFromTriple() )
                {
                    cb.SetFirstPoint( nullptr );
                    m_point->SetPaintMode( GB::PointPaintMode::Usual );
                }
                else
                {
                    cb.SetSecondPoint( m_point );
                    m_point->SetPaintMode( GB::PointPaintMode::Selected );
                }

                m_point->update();
            }
            else if ( !cb.ThirdPointIsSelected() )
            {
                if ( m_point == cb.GetFirstPointFromTriple() )
                {
                    m_point->SetPaintMode( GB::PointPaintMode::Usual );

                    m_point->update();

                    cb.SetFirstPoint( cb.GetSecondPointFromTriple() );
                    cb.SetSecondPoint( nullptr );
                }
                else if ( m_point == cb.GetSecondPointFromTriple() )
                {
                    cb.SetSecondPoint( nullptr );
                    m_point->SetPaintMode( GB::PointPaintMode::Usual );

                    m_point->update();
                }
                else
                {
                    cb.SetThirdPoint( m_point );

                    cb.GetFirstPointFromTriple()->SetPaintMode( GB::PointPaintMode::Usual );
                    cb.GetSecondPointFromTriple()->SetPaintMode( GB::PointPaintMode::Usual );

                    cb.CreateCircle( GB::CircleBuildingType::ThroughThreePoints );
                    scene->addItem( cb.GetCircle() );

                    cb.Reset();
                }
            }

            break;
        }
        }

        break;
    }
    case GB::GraphicsShapeType::Polygon :
    {
        GSBuilders::PolygonBuilder & polb = GSBuilders::Objects::GetPolygonBuilder();

        if ( polb.IsItStartPoint( m_point ) )
        {
            if ( polb.GetVerticesCount() <= 2 )
            {
                polb.ShowMessageAboutPolygonVerticesCount();

                return;
            }

            scene->removeItem( polb.GetTempSideFromLastVertex() );
            polb.DestroyTempSideFromLastVertex();
            scene->removeItem( polb.GetTempSideFromStartPoint() );
            polb.DestroyTempSideFromStartPoint();
            polb.DestroyTempPoint();

            polb.CreateLastSide();
            scene->addItem( polb.GetSide() );

            polb.CreateGraphicsPolygon();
            scene->addItem( polb.GetGraphicsPolygon() );

            polb.Reset();

            m_point->update();
        }

        break;
    }
    }
}

void GraphicsShapesBSEHandlers::HandlerForPoint::HandleLineSegmentMidpointAdding( QGraphicsSceneMouseEvent * event )
{
    GSBuilders::Additional::LineSegmentMidpointBuilder & builder = GSBuilders::Objects::GetLineSegmentMidpointBuilder();

    if ( !builder.FirstPointIsSelected() )
    {
        builder.SetFirstPoint( m_point );

        builder.GetFirstPoint()->SetPaintMode( GB::PointPaintMode::AroundCircle );

        m_point->update();
    }
    else
    {
        builder.SetSecondPoint( m_point );

        builder.CreateMidpoint();
        m_point->scene()->addItem( builder.GetMidpoint() );

        builder.GetFirstPoint()->SetPaintMode( GB::PointPaintMode::Usual );

        builder.Reset();

        m_point->update();
    }
}

void GraphicsShapesBSEHandlers::HandlerForPoint::HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::PerpendicularLineSegmentBuilder & builder =
        GSBuilders::Objects::GetPerpendicularLineSegmentBuilder();

    if ( !builder.SourcePointIsSelected() )
    {
        builder.SetSourcePoint( m_point );

        m_point->SetPaintMode( GB::PointPaintMode::AroundCircle );

        m_point->update();
    }
}

void GraphicsShapesBSEHandlers::HandlerForPoint::HandleParallelLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::ParallelLineSegmentBuilder & builder =
        GSBuilders::Objects::GetParallelLineSegmentBuilder();

    if ( !builder.FirstEndIsSelected() )
    {
        builder.SetFirstEndPoint( m_point );

        builder.CreateTempPoint();
        builder.CreateTempLineSegment();

        builder.GetTempLineSegment()->SetPaintMode(
            GB::LineSegmentPaintMode::DotLine );

        m_point->scene()->addItem( builder.GetTempPoint() );
        m_point->scene()->addItem( builder.GetTempLineSegment() );

        builder.NeedToShowTempLineSegment();

        //m_point->scene()->update();
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

        m_point->scene()->removeItem( builder.GetTempLineSegment() );
        builder.DestroyTempLineSegment();

        builder.CreateSecondEndPoint( builder.GetTempPoint()->GetPosition() );
        m_point->scene()->removeItem( builder.GetTempPoint() );
        builder.DestroyTempPoint();
        m_point->scene()->addItem( builder.GetSecondEndPoint() );

        builder.CreateParallelLineSegment();
        m_point->scene()->addItem( builder.GetParallelLineSegment() );

        builder.Reset();

        //m_point->scene()->update();
    }
}

void GraphicsShapesBSEHandlers::HandlerForPoint::HandleTangentLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::TangentLineSegmentBuilder & builder =
        GSBuilders::Objects::GetTangentLineSegmentBuilder();

    if ( !builder.SourcePointIsSelected() )
    {
        builder.SetSourcePoint( m_point );

        m_point->SetPaintMode(
            GB::PointPaintMode::AroundCircle );
    }
}





