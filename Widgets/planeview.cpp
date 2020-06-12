#include "planeview.h"
#include "planescene.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "DebugHelpers/debughelper.h"
#include "interactionobjects.h"
#include "GraphicsShapesBuilders/linesegmentbuilder.h"
#include "GraphicsShapesBuilders/linebuilder.h"
#include "GraphicsShapesBuilders/circlebuilder.h"
#include "GraphicsShapesBuilders/polygonbuilder.h"
#include "GraphicsShapesBuilders/Additional/perpendicularlinesegmentbuilder.h"
#include "GraphicsShapesBuilders/Additional/parallellinesegmentbuilder.h"
#include "GraphicsShapesBuilders/objects.h"
#include "GraphicsShapes/helpers.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>



PlaneView::PlaneView( QWidget * parent )
    : QGraphicsView( parent ),
      m_scene( new PlaneScene() )
{
    m_scene->setParent( this );
    setSceneRect( viewport()->rect() );
    setScene( m_scene );

    setMouseTracking( true );
    setViewportUpdateMode( FullViewportUpdate );
    setRenderHint( QPainter::Antialiasing );
    //setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    //setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setAlignment( Qt::AlignLeft | Qt::AlignTop );
}



//void PlaneView::mousePressEvent( QMouseEvent * event ) {}

void PlaneView::mouseMoveEvent( QMouseEvent * event )
{
    QGraphicsView::mouseMoveEvent( event );

    switch ( InteractionObjects::buildingButtonsData.GetPushedBuildingButton() )
    {
    case GB::PushedBuildingButton::AddPoint :
    {
        break;
    }
    case GB::PushedBuildingButton::DrawLineSegment :
    {
        GSBuilders::LineSegmentBuilder & lsb = GSBuilders::Objects::GetLineSegmentBuilder();

        if ( lsb.FirstPointIsAdded() )
        {
            lsb.SetTempPointCenter( mapToScene( event->pos() ) );

            m_scene->update();
        }

        break;
    }
    case GB::PushedBuildingButton::DrawLine :
    {
        GSBuilders::LineBuilder & lb = GSBuilders::Objects::GetLineBuilder();

        if ( lb.FirstPointIsAdded() )
        {
            lb.SetTempPointCenter( mapToScene( event->pos() ) );
            lb.RecalculateTempLine();

            m_scene->update();
        }

        break;
    }
    case GB::PushedBuildingButton::AddCircle :
    {
        GSBuilders::CircleBuilder & cb = GSBuilders::Objects::GetCircleBuilder();

        if ( cb.CenterIsAdded() )
        {
            qreal const r = Math::GetDistance( cb.GetCenterPos(), event->pos() );
            cb.SetTempRadius( r );

            m_scene->update();
        }

        break;
    }
    case GB::PushedBuildingButton::DrawPolygon :
    {
        GSBuilders::PolygonBuilder & polb = GSBuilders::Objects::GetPolygonBuilder();

        if ( polb.StartPointIsAdded() )
        {
            polb.SetTempPointPosition( event->pos() );

            m_scene->update();
        }

        break;
    }
    case GB::PushedBuildingButton::AddPerpendicular :
    {
        GSBuilders::Additional::PerpendicularLineSegmentBuilder & builder =
            GSBuilders::Objects::GetPerpendicularLineSegmentBuilder();

        if ( builder.DoesItNeedToShowTempPerpendicular() )
        {
            QPointF const cursorPos = mapToScene( event->pos() );

            QPointF cursorBasePos;
            if ( builder.GetTargetLine() )
            {
                cursorBasePos = GSHelpers::GetPerpendicularBasePoint(
                                    cursorPos,
                                    builder.GetTargetLine() );
            }
            else if ( builder.GetTargetLineSegment() )
            {
                cursorBasePos = GSHelpers::GetPerpendicularBasePoint(
                                    cursorPos,
                                    builder.GetTargetLineSegment() );
            }
            Math::Vector const v = Math::Vector(
                                       cursorBasePos,
                                       builder.GetBasePoint()->GetPosition() );

            builder.SetTempPointPosition(
                cursorPos + v );

            m_scene->update();
        }

        break;
    }
    case GB::PushedBuildingButton::AddParallel :
    {
        GSBuilders::Additional::ParallelLineSegmentBuilder & builder =
            GSBuilders::Objects::GetParallelLineSegmentBuilder();

        if ( builder.DoesItNeedToShowTempLineSegment() )
        {
            QPointF const cursorPos = mapToScene( event->pos() );

            QPointF tempPointPos;
            if ( builder.GetTargetLine() )
            {
                QPointF const p1 = builder.GetFirstEndPoint()->GetPosition();
                Math::Vector const v = GSHelpers::GetVector( builder.GetTargetLine() );
                QPointF const p2 = p1 + v;

                tempPointPos = Math::GetPerpendicularBasePoint(
                                   cursorPos, QLineF( p1, p2 ) );
            }
            else if ( builder.GetTargetLineSegment() )
            {
                QPointF const p1 = builder.GetFirstEndPoint()->GetPosition();
                Math::Vector const v = GSHelpers::GetVector( builder.GetTargetLineSegment() );
                QPointF const p2 = p1 + v;

                tempPointPos = Math::GetPerpendicularBasePoint(
                                   cursorPos, QLineF( p1, p2 ) );
            }

            builder.SetTempPointPosition( tempPointPos );

            m_scene->update();
        }

        break;
    }
    }



    switch ( InteractionObjects::userActionMode )
    {
    case GB::UserActionMode::Building :
    {
        setCursor( Qt::CrossCursor );

        break;
    }
    case GB::UserActionMode::PolygonSelection :
    {
        setCursor( Qt::PointingHandCursor );

        break;
    }
    default :
        setCursor( Qt::ArrowCursor );
    }
}



void PlaneView::TestingPaint()
{
    GraphicsCircle * c1 = new GraphicsCircle( QPointF( 300, 300 ), 100 );
    m_scene->addItem( c1 );
    GraphicsCircle * c2 = new GraphicsCircle( QPointF( 450, 375 ), 150 );
    m_scene->addItem( c2 );
}


