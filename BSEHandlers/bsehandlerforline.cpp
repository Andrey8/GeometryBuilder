#include "BSEHandlers/bsehandlerforline.h"
#include "planescene.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/helpers.h"

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



void GraphicsShapesBSEHandlers::HandlerForLine::HandleGraphicsShapeAdding( QGraphicsSceneMouseEvent * event, GB::GraphicsShapeType type ) const
{
    PlaneScene * scene = static_cast< PlaneScene * >( m_line->scene() );
    QList< GraphicsLineSegment * > const & partitionSegments = m_line->GetPartitionSegments();

    switch ( type )
    {
    case GB::GraphicsShapeType::Point :
    {
        GSBuilders::PointBuilder & pb = GSBuilders::Objects::GetPointBuilder();

        foreach ( GraphicsLineSegment * ls, partitionSegments )
        {
            if ( ls->GetSelectionAreaPath().contains( event->scenePos() ) )
            {
                pb.CreatePointOnLineSegment( event->scenePos(), ls );
                scene->addItem( pb.GetPoint() );

                m_line->PutPoint( pb.GetPoint(), ls );

                pb.Reset();

                m_line->update();

                return;
            }
        }

        break;
    }
    case GB::GraphicsShapeType::LineSegment :
    {
        GSBuilders::LineSegmentBuilder & lsb = GSBuilders::Objects::GetLineSegmentBuilder();

        foreach ( GraphicsLineSegment * ls, partitionSegments )
        {
            if ( ls->GetSelectionAreaPath().contains( event->scenePos() ) )
            {
                if ( !lsb.FirstPointIsAdded() )
                {
                    lsb.CreateFirstPointOnLineSegment( event->scenePos(), ls );
                    scene->addItem( lsb.GetFirstPoint() );

                    lsb.SetLineAsFirstPointOwner( m_line );

                    m_line->PutPoint( lsb.GetFirstPoint(), ls );

                    lsb.CreateTempPoint( event->scenePos() );

                    lsb.CreateTempLineSegment();
                    scene->addItem( lsb.GetTempLineSegment() );

                    m_line->update();
                }
                else
                {
                    scene->removeItem( lsb.GetTempLineSegment() );
                    lsb.DestroyTempLineSegment();

                    lsb.DestroyTempPoint();

                    lsb.CreateSecondPointOnLineSegment( event->scenePos(), ls );
                    scene->addItem( lsb.GetSecondPoint() );

                    m_line->PutPoint( lsb.GetSecondPoint(), ls );

                    if ( lsb.FirstPointIsOnLine( m_line ) )
                    {
                        lsb.Reset();
                        m_line->update();

                        return;
                    }

                    lsb.CreateLineSegment();
                    scene->addItem( lsb.GetLineSegment() );

                    lsb.AttachLineSegmentToItsEndPoints();

                    lsb.Reset();

                    m_line->update();
                }

                return;
            }
        }

        break;
    }
    case GB::GraphicsShapeType::Line :
    {
        GSBuilders::LineBuilder & lb = GSBuilders::Objects::GetLineBuilder();

        foreach ( GraphicsLineSegment * ls, partitionSegments )
        {
            if ( ls->GetSelectionAreaPath().contains( event->scenePos() ) )
            {
                if ( !lb.FirstPointIsAdded() )
                {
                    lb.CreateFirstPointOnLineSegment( event->scenePos(), ls );
                    scene->addItem( lb.GetFirstPoint() );

                    m_line->PutPoint( lb.GetFirstPoint(), ls );

                    lb.SetLineAsFirstPointOwner( m_line );

                    lb.CreateTempPoint( event->scenePos() );

                    lb.CreateTempLine();
                    scene->addItem( lb.GetTempLine() );

                    m_line->update();
                }
                else
                {
                    scene->removeItem( lb.GetTempLine() );
                    lb.DestroyTempLine();

                    lb.DestroyTempPoint();

                    lb.CreateSecondPointOnLineSegment( event->scenePos(), ls );
                    scene->addItem( lb.GetSecondPoint() );

                    m_line->PutPoint( lb.GetSecondPoint(), ls );

                    if ( lb.FirstPointIsOnLine( m_line ) )
                    {
                        lb.Reset();
                        m_line->update();

                        return;
                    }

                    lb.CreateLine();
                    scene->addItem( lb.GetLine() );
                    scene->addItem( lb.GetLine()->GetInitialSegment() );

                    lb.Reset();

                    m_line->update();
                }

                return;
            }
        }

        break;
    }
    case GB::GraphicsShapeType::Circle :
    {
        GSBuilders::CircleBuilder & cb = GSBuilders::Objects::GetCircleBuilder();

        foreach ( GraphicsLineSegment * ls, partitionSegments )
        {
            if ( ls->GetSelectionAreaPath().contains( event->scenePos() ) )
            {
                if ( !cb.CenterIsAdded() )
                {
                    cb.CreateCenterOnLineSegment( event->scenePos(), ls );
                    scene->addItem( cb.GetCenter() );

                    m_line->PutPoint( cb.GetCenter(), ls );

                    cb.CreateTempCircle();
                    scene->addItem( cb.GetTempCircle() );
                }

                break;
            }
        }

        break;
    }
    case GB::GraphicsShapeType::Polygon :
    {
        GSBuilders::PolygonBuilder & polb = GSBuilders::Objects::GetPolygonBuilder();

        foreach ( GraphicsLineSegment * ls, partitionSegments )
        {
            if ( ls->GetSelectionAreaPath().contains( event->scenePos() ) )
            {
                if ( !polb.StartPointIsAdded() )
                {
                    polb.CreateStartPoint( event->scenePos() );
                    scene->addItem( polb.GetStartPoint() );

                    m_line->PutPoint( polb.GetStartPoint(), ls );

                    polb.CreateTempPoint( event->scenePos() );

                    polb.CreateFirstTempSide();
                    scene->addItem( polb.GetTempSideFromLastVertex() );

                    m_line->update();
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

                    m_line->PutPoint( polb.GetVertex(), ls );

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

                    m_line->update();
                }
            }
        }

        break;
    }
    }
}

void GraphicsShapesBSEHandlers::HandlerForLine::HandleLineSegmentMidpointAdding( QGraphicsSceneMouseEvent * event )
{
    PlaneScene * scene = static_cast< PlaneScene * >( m_line->scene() );
    QList< GraphicsLineSegment * > const & partitionSegments = m_line->GetPartitionSegments();

    GSBuilders::Additional::LineSegmentMidpointBuilder & builder =
        GSBuilders::Objects::GetLineSegmentMidpointBuilder();

    GraphicsPoint * temp = new GraphicsPoint( event->scenePos() );
    GraphicsLineSegment * segment = GSHelpers::GetContainingSegment( partitionSegments.toStdList(), temp );
    delete temp;

    builder.SetFirstPoint( segment->p1() );
    builder.SetSecondPoint( segment->p2() );

    builder.CreateMidpoint();

    scene->addItem( builder.GetMidpoint() );

    m_line->PutPoint( builder.GetMidpoint() );

    builder.Reset();
}

void GraphicsShapesBSEHandlers::HandlerForLine::HandlePerpendicularLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::PerpendicularLineSegmentBuilder & builder =
        GSBuilders::Objects::GetPerpendicularLineSegmentBuilder();

    if ( builder.SourcePointIsSelected() )
    {
        GraphicsPoint * sourcePoint = builder.GetSourcePoint();

        sourcePoint->SetPaintMode( GB::PointPaintMode::Usual );

        builder.SetLine( m_line );

        if ( m_line->Contains( sourcePoint ) )
        {
            builder.CreateTempPoint();
            builder.CreateTempPerpendicular();

            builder.GetTempPerpendicular()->SetPaintMode(
                GB::LineSegmentPaintMode::DotLine );

            m_line->scene()->addItem( builder.GetTempPoint() );
            m_line->scene()->addItem( builder.GetTempPerpendicular() );

            builder.NeedToShowTempPerpendicular();

            return;
        }

        builder.CreatePerpendicularToLine();

        m_line->scene()->addItem( builder.GetPerpendicular() );
        m_line->scene()->addItem( builder.GetBasePoint() );
        m_line->scene()->update();

        builder.Reset();
    }
}

void GraphicsShapesBSEHandlers::HandlerForLine::HandleParallelLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::ParallelLineSegmentBuilder & builder =
        GSBuilders::Objects::GetParallelLineSegmentBuilder();

    if ( !builder.TargetIsSelected() )
    {
        builder.SetTargetLine( m_line );

        m_line->SetPaintMode( GB::LinePaintMode::Selected );
    }
    else
    {
        if ( !builder.FirstEndIsSelected() )
        {
            QList< GraphicsLineSegment * > const & partitionSegments =
                m_line->GetPartitionSegments();

            foreach ( GraphicsLineSegment * ls, partitionSegments )
            {
                if ( ls->GetSelectionAreaPath().contains( event->scenePos() ) )
                {
                    builder.CreateFirstEndPointOnLineSegment( event->scenePos(), ls );
                    m_line->scene()->addItem( builder.GetFirstEndPoint() );

                    m_line->PutPoint( builder.GetFirstEndPoint(), ls );

                    builder.CreateTempPoint();
                    builder.CreateTempLineSegment();

                    builder.GetTempLineSegment()->SetPaintMode(
                        GB::LineSegmentPaintMode::DotLine );

                    m_line->scene()->addItem( builder.GetTempPoint() );
                    m_line->scene()->addItem( builder.GetTempLineSegment() );

                    builder.NeedToShowTempLineSegment();

                    m_line->update();

                    return;
                }
            }
        }
    }
}

void GraphicsShapesBSEHandlers::HandlerForLine::HandleTangentLSAdding( QGraphicsSceneMouseEvent * event ) const
{
    GSBuilders::Additional::TangentLineSegmentBuilder & builder =
        GSBuilders::Objects::GetTangentLineSegmentBuilder();

    if ( !builder.SourcePointIsSelected() )
    {
        builder.CreateSourcePointOnLine(
            event->scenePos(),
            m_line );
        m_line->scene()->addItem( builder.GetSourcePoint() );

        m_line->PutPoint( builder.GetSourcePoint() );

        builder.GetSourcePoint()->SetPaintMode(
            GB::PointPaintMode::AroundCircle );
    }
}






