#include "IntersectionPointsBSEHandlers/handlerforline.h"
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



void IntersectionPointsBSEHandlers::HandlerForLine::HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * event )
{
    GSBuilders::Additional::IntersectionPointsBuilder & ipbuilder =
        GSBuilders::Objects::GetIntersectionPointsBuilder();

    if ( !ipbuilder.FirstShapeIsSelected() )
    {
        ipbuilder.SetFirstShape( m_line );
        m_line->SetPaintMode( GB::LinePaintMode::Selected );

        m_line->update();
    }
    else
    {
        switch ( ipbuilder.GetFirstShapeType() )
        {
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::Line :
        {
            ipbuilder.GetFirstLine()->SetPaintMode( GB::LinePaintMode::Usual );

            if ( ipbuilder.GetFirstLine() != m_line )
            {
                ipbuilder.SetSecondShape( m_line );

                Math::TwoLinesIntersectionData const data =
                    GSHelpers::GetIntersectionData(
                        ipbuilder.GetFirstLine(),
                        ipbuilder.GetSecondLine() );
                if ( data.GetLocation() == Math::TwoLinesLocation::Intersect )
                {
                    ipbuilder.CreateIntersectionPoint( data.GetIntersectionPoint() );
                    GraphicsPoint * p = ipbuilder.GetOnlyIntersectionPoint();
                    m_line->scene()->addItem( p );

                    try
                    {
                        ipbuilder.GetFirstLine()->PutPoint( p );
                        m_line->PutPoint( p );
                    }
                    catch ( Exception const & exc )
                    {
                        qDebug() << exc.GetMessage() ;
                    }
                }
                else if ( data.GetLocation() == Math::TwoLinesLocation::Parallel )
                {
                    return;
                }

                m_line->update();
            }

            break;
        }
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::LineSegmnet :
        {
            ipbuilder.GetOneLineSegment()->SetPaintMode( GB::LineSegmentPaintMode::Usual );

            ipbuilder.SetSecondShape( m_line );

            Math::LineWithLineSegmentIntersectionData const data =
                GSHelpers::GetIntersectionData(
                    ipbuilder.GetOneLine(),
                    ipbuilder.GetOneLineSegment() );
            if ( data.GetLocation() == Math::LineWithLineSegmentLocation::Intersect )
            {
                ipbuilder.CreateIntersectionPoint( data.GetIntersectionPoint() );
                GraphicsPoint * p = ipbuilder.GetOnlyIntersectionPoint();

                m_line->scene()->addItem( p );

                try
                {
                    ipbuilder.GetOneLineSegment()->PutPoint( p );
                    m_line->PutPoint( p );
                }
                catch ( Exception const & exc )
                {
                    qDebug() << exc.GetMessage() ;
                }
            }

            m_line->update();

            break;
        }
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::Circle :
        {
            ipbuilder.GetOneCircle()->SetPaintMode( GB::CirclePaintMode::Usual );

            ipbuilder.SetSecondShape( m_line );

            Math::CircleWithLineIntersectionData const data =
                GSHelpers::GetIntersectionData(
                    ipbuilder.GetOneCircle(),
                    ipbuilder.GetOneLine() );
            if ( data.GetLocation() == Math::CircleWithLineLocation::IntersectionInTwoPoints )
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

                m_line->scene()->addItem( p1 );
                m_line->scene()->addItem( p2 );

                try
                {
                    ipbuilder.GetOneCircle()->PutPoint( p1 );
                    ipbuilder.GetOneCircle()->PutPoint( p2 );
                    m_line->PutPoint( p1 );
                    m_line->PutPoint( p2 );
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
        }

        ipbuilder.Reset();
    }
}
