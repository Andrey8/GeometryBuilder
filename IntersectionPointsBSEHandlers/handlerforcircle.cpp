#include "IntersectionPointsBSEHandlers/handlerforcircle.h"
#include "GraphicsShapesBuilders/Additional/intersectionpointbuilder.h"
#include "GraphicsShapesBuilders/objects.h"
#include "gbuildernamespace.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"
#include "GraphicsShapesIntersections/objects.h"
#include "GraphicsShapes/helpers.h"

#include <QGraphicsScene>



void IntersectionPointsBSEHandlers::HandlerForCircle::HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * event )
{
    GSBuilders::Additional::IntersectionPointsBuilder & ipbuilder =
        GSBuilders::Objects::GetIntersectionPointsBuilder();

    if ( !ipbuilder.FirstShapeIsSelected() )
    {
        ipbuilder.SetFirstShape( m_circle );
        m_circle->SetPaintMode( GB::CirclePaintMode::Selected );

        m_circle->update();
    }
    else
    {
        switch ( ipbuilder.GetFirstShapeType() )
        {
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::Line :
        {
            ipbuilder.GetOneLine()->SetPaintMode( GB::LinePaintMode::Usual );

            ipbuilder.SetSecondShape( m_circle );

            Math::CircleWithLineIntersectionData const data =
                GSHelpers::GetIntersectionData( ipbuilder.GetOneCircle(), ipbuilder.GetOneLine() );
            if ( data.GetLocation() == Math::CircleWithLineLocation::IntersectionInTwoPoints )
            {
                try
                {
                    std::pair< QPointF const, QPointF const > const pointsPair = data.GetIntersectionPointsPair();
                    ipbuilder.CreateIntersectionPoints( pointsPair.first, pointsPair.second );

                    std::vector< GraphicsPoint * > const points = ipbuilder.GetIntersectionPoints();
                    GraphicsPoint * p1 = points[ 0 ];
                    GraphicsPoint * p2 = points[ 1 ];

                    GSI::GetIntersectionManager().AddData(
                        ipbuilder.GetOneCircle(),
                        ipbuilder.GetOneLine(),
                        GSI::CircleWithLineIntersectionData(
                            Math::CircleWithLineLocation::IntersectionInTwoPoints,
                            points ) );

                    m_circle->scene()->addItem( p1 );
                    m_circle->scene()->addItem( p2 );

                    ipbuilder.GetOneLine()->PutPoint( p1 );
                    ipbuilder.GetOneLine()->PutPoint( p2 );
                    m_circle->PutPoint( p1 );
                    m_circle->PutPoint( p2 );
                }
                catch ( Exception const & exc )
                {
                    qDebug() << exc.GetMessage() ;
                }
            }
            else if ( data.GetLocation() == Math::CircleWithLineLocation::Touching )
            {

            }

            break;
        }
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::LineSegmnet :
        {
            ipbuilder.GetOneLineSegment()->SetPaintMode( GB::LineSegmentPaintMode::Usual );

            ipbuilder.SetSecondShape( m_circle );

            Math::CircleWithLineSegmentIntersectionData const data =
                GSHelpers::GetIntersectionData(
                    ipbuilder.GetOneCircle(),
                    ipbuilder.GetOneLineSegment() );
            switch ( data.GetLocation() )
            {
            case Math::CircleWithLineSegmentLocation::IntersectionInTwoPoints :
            {
                std::pair< QPointF const, QPointF const > const pointsPair = data.GetIntersectionPointsPair();
                ipbuilder.CreateIntersectionPoints( pointsPair.first, pointsPair.second );

                std::vector< GraphicsPoint * > const points = ipbuilder.GetIntersectionPoints();
                GraphicsPoint * p1 = points[ 0 ];
                GraphicsPoint * p2 = points[ 1 ];

                GSI::GetIntersectionManager().AddData(
                    ipbuilder.GetOneCircle(),
                    ipbuilder.GetOneLineSegment(),
                    GSI::CircleWithLineSegmentIntersectionData(
                        Math::CircleWithLineSegmentLocation::IntersectionInTwoPoints,
                        points ) );

                m_circle->scene()->addItem( p1 );
                m_circle->scene()->addItem( p2 );

                try
                {
                    ipbuilder.GetOneLineSegment()->PutPoint( p1 );
                    ipbuilder.GetOneLineSegment()->PutPoint( p2 );
                    m_circle->PutPoint( p1 );
                    m_circle->PutPoint( p2 );
                }
                catch ( Exception const & exc )
                {
                    qDebug() << exc.GetMessage() ;
                }

                break;
            }
            case Math::CircleWithLineSegmentLocation::IntersectionInOnePoint :
            {
                QPointF const pos = data.GetOnlyIntersectionPoint();
                ipbuilder.CreateIntersectionPoint( pos );

                std::vector< GraphicsPoint * > const points = ipbuilder.GetIntersectionPoints();
                GraphicsPoint * p = points[ 0 ];

                GSI::GetIntersectionManager().AddData(
                    ipbuilder.GetOneCircle(),
                    ipbuilder.GetOneLineSegment(),
                    GSI::CircleWithLineSegmentIntersectionData(
                        Math::CircleWithLineSegmentLocation::IntersectionInOnePoint,
                        points ) );

                m_circle->scene()->addItem( p );

                try
                {
                    ipbuilder.GetOneLineSegment()->PutPoint( p );
                    m_circle->PutPoint( p );
                }
                catch ( Exception const & exc )
                {
                    qDebug() << exc.GetMessage() ;
                }

                break;
            }
            case Math::CircleWithLineSegmentLocation::Touching :
            {


                break;
            }
            case Math::CircleWithLineSegmentLocation::NoIntersection :
            {


                break;
            }
            }

            break;
        }
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::Circle :
        {
            ipbuilder.GetOneCircle()->SetPaintMode( GB::CirclePaintMode::Usual );

            ipbuilder.SetSecondShape( m_circle );

            Math::TwoCirclesIntersectionData const data =
                GSHelpers::GetIntersectionData(
                    ipbuilder.GetFirstCircle(),
                    ipbuilder.GetSecondCircle() );
            switch ( data.GetLocation() )
            {
            case Math::TwoCirclesLocation::IntersectionInTwoPoints :
            {
                std::pair< QPointF const, QPointF const > const pointsPair =
                    data.GetIntersectionPointsPair();

                ipbuilder.CreateIntersectionPoints( pointsPair.first, pointsPair.second );

                std::vector< GraphicsPoint * > const points = ipbuilder.GetIntersectionPoints();
                GraphicsPoint * p1 = points[ 0 ];
                GraphicsPoint * p2 = points[ 1 ];

                //

                m_circle->scene()->addItem( p1 );
                m_circle->scene()->addItem( p2 );

                try
                {
                    ipbuilder.GetFirstCircle()->PutPoint( p1 );
                    ipbuilder.GetFirstCircle()->PutPoint( p2 );
                    m_circle->PutPoint( p1 );
                    m_circle->PutPoint( p2 );
                }
                catch ( Exception const & exc )
                {
                    qDebug() << exc.GetMessage() ;
                }

                break;
            }
            case Math::TwoCirclesLocation::Touching :
            {
                QPointF const pos = data.GetOneIntersectionPoint();
                ipbuilder.CreateIntersectionPoint( pos );

                std::vector< GraphicsPoint * > const points = ipbuilder.GetIntersectionPoints();
                GraphicsPoint * p = points[ 0 ];

                //

                m_circle->scene()->addItem( p );

                try
                {
                    ipbuilder.GetFirstCircle()->PutPoint( p );
                    m_circle->PutPoint( p );
                }
                catch ( Exception const & exc )
                {
                    qDebug() << exc.GetMessage() ;
                }

                break;
            }
            case Math::TwoCirclesLocation::NoIntersection :
            {
                ipbuilder.ShowMessageAboutNoIntersection();

                break;
            }
            }

            break;
        }
        }

        ipbuilder.Reset();
    }
}
