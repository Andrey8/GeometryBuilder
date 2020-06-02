#include "IntersectionPointsBSEHandlers/handlerforlinesegment.h"
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



void IntersectionPointsBSEHandlers::HandlerForLineSegment::HandleIntersectionPointsAdding( QGraphicsSceneMouseEvent * event )
{
    GSBuilders::Additional::IntersectionPointsBuilder & ipbuilder =
        GSBuilders::Objects::GetIntersectionPointsBuilder();

    if ( !ipbuilder.FirstShapeIsSelected() )
    {
        ipbuilder.SetFirstShape( m_lineSegment );
        m_lineSegment->SetPaintMode( GB::LineSegmentPaintMode::Selected );

        m_lineSegment->update();
    }
    else
    {
        switch ( ipbuilder.GetFirstShapeType() )
        {
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::Line :
        {
            ipbuilder.GetOneLine()->SetPaintMode( GB::LinePaintMode::Usual );

            ipbuilder.SetSecondShape( m_lineSegment );

            Math::LineWithLineSegmentIntersectionData const data =
                GSHelpers::GetIntersectionData(
                    ipbuilder.GetOneLine(),
                    ipbuilder.GetOneLineSegment() );
            if ( data.GetLocation() == Math::LineWithLineSegmentLocation::Intersect )
            {
                ipbuilder.CreateIntersectionPoint( data.GetIntersectionPoint() );
                GraphicsPoint * p = ipbuilder.GetOnlyIntersectionPoint();

                m_lineSegment->scene()->addItem( p );

                try
                {
                    ipbuilder.GetOneLine()->PutPoint( p );
                    m_lineSegment->PutPoint( p );
                }
                catch ( Exception const & exc )
                {
                    qDebug() << exc.GetMessage() ;
                }
            }

            m_lineSegment->update();

            break;
        }
        case GSBuilders::Additional::IntersectionPointsBuilder::Shape::LineSegmnet :
        {
            ipbuilder.GetFirstLineSegment()->SetPaintMode( GB::LineSegmentPaintMode::Usual );

            if ( ipbuilder.GetFirstLineSegment() != m_lineSegment )
            {
                ipbuilder.SetSecondShape( m_lineSegment );

                Math::TwoLineSegmentsIntersectionData const data =
                    GSHelpers::GetIntersectionData(
                        ipbuilder.GetFirstLineSegment(),
                        ipbuilder.GetSecondLineSegment() );
                if ( data.GetLocation() == Math::TwoLineSegmentsLocation::Intersect )
                {
                    ipbuilder.CreateIntersectionPoint( data.GetIntersectionPoint() );
                    GraphicsPoint * p = ipbuilder.GetOnlyIntersectionPoint();

                    m_lineSegment->scene()->addItem( p );

                    try
                    {
                        ipbuilder.GetFirstLineSegment()->PutPoint( p );
                        m_lineSegment->PutPoint( p );
                    }
                    catch ( Exception const & exc )
                    {
                        qDebug() << exc.GetMessage() ;
                    }
                }
                else
                {
                    ipbuilder.ShowMessageAboutNoIntersection();

                    break;
                }

                m_lineSegment->update();
            }

            break;
        }
        }

        ipbuilder.Reset();
    }
}
