#include "GraphicsShapesBuilders/Additional/intersectionpointbuilder.h"
#include "DebugHelpers/exception.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"
#include "GraphicsShapesIntersections/objects.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"



GSBuilders::Additional::IntersectionPointsBuilder::Shape GSBuilders::Additional::IntersectionPointsBuilder::GetFirstShapeType()
{
    return m_firstShapeType;
}

GraphicsLineSegment * GSBuilders::Additional::IntersectionPointsBuilder::GetFirstLineSegment()
{
    return m_ls1;
}

GraphicsLineSegment * GSBuilders::Additional::IntersectionPointsBuilder::GetSecondLineSegment()
{
    return m_ls2;
}

GraphicsLine * GSBuilders::Additional::IntersectionPointsBuilder::GetFirstLine()
{
    return m_l1;
}

GraphicsLine * GSBuilders::Additional::IntersectionPointsBuilder::GetSecondLine()
{
    return m_l2;
}

GraphicsLineSegment * GSBuilders::Additional::IntersectionPointsBuilder::GetOneLineSegment()
{
    if ( m_ls1 && !m_ls2 )
    {
        return m_ls1;
    }
    else if ( !m_ls1 && m_ls2 )
    {
        return m_ls2;
    }
    else
    {
        throw Exception( "IntersectionPointBuilder::GetOneLineSegment : segment is not one." );
    }
}

GraphicsLine * GSBuilders::Additional::IntersectionPointsBuilder::GetOneLine()
{
    if ( m_l1 && !m_l2 )
    {
        return m_l1;
    }
    else if ( !m_l1 && m_l2 )
    {
        return m_l2;
    }
    else
    {
        throw Exception( "IntersectionPointBuilder::GetOneLine : line is not one." );
    }
}

GraphicsCircle * GSBuilders::Additional::IntersectionPointsBuilder::GetOneCircle()
{
    if ( m_c1 && !m_c2 )
    {
        return m_c1;
    }
    else if ( !m_c1 && m_c2 )
    {
        return m_c2;
    }
    else
    {
        throw Exception( "IntersectionPointBuilder::GetOneCircle : circle is not one." );
    }
}

GraphicsPoint * GSBuilders::Additional::IntersectionPointsBuilder::GetOnlyIntersectionPoint()
{
    if ( m_intersectionPoints.size() == 1 )
    {
        return m_intersectionPoints[ 0 ];
    }
    else
    {
        throw Exception( "IntersectionPointsBuilder::GetIntersectionPoint : intersection point is not only." );
    }
}

void GSBuilders::Additional::IntersectionPointsBuilder::SetFirstShape( GraphicsLineSegment * ls )
{
    m_ls1 = ls;
    m_firstShapeIsSelected = true;
    m_firstShapeType = Shape::LineSegmnet;
}

void GSBuilders::Additional::IntersectionPointsBuilder::SetFirstShape( GraphicsLine * l )
{
    m_l1 = l;
    m_firstShapeIsSelected = true;
    m_firstShapeType = Shape::Line;
}

void GSBuilders::Additional::IntersectionPointsBuilder::SetFirstShape( GraphicsCircle * c )
{
    m_c1 = c;
    m_firstShapeIsSelected = true;
    m_firstShapeType = Shape::Circle;
}

void GSBuilders::Additional::IntersectionPointsBuilder::SetSecondShape( GraphicsLineSegment * ls )
{
    m_ls2 = ls;
    m_secondShapeType = Shape::LineSegmnet;
}

void GSBuilders::Additional::IntersectionPointsBuilder::SetSecondShape( GraphicsLine * l )
{
    m_l2 = l;
    m_secondShapeType = Shape::Line;
}

void GSBuilders::Additional::IntersectionPointsBuilder::SetSecondShape( GraphicsCircle * c )
{
    m_c2 = c;
    m_secondShapeType = Shape::Circle;
}

bool GSBuilders::Additional::IntersectionPointsBuilder::FirstShapeIsSelected()
{
    return m_firstShapeIsSelected;
}

void GSBuilders::Additional::IntersectionPointsBuilder::CreateIntersectionPoint( QPointF const & pos )
{
    GraphicsPoint * p = new GraphicsPoint( pos );
    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( p );

    m_intersectionPoints.reserve( 1 );

    m_intersectionPoints.push_back( p );
    //m_intersectionPoints[ 0 ]->SetAsIntersectionPoint();

    switch ( m_firstShapeType )
    {
    case Shape::Line :
    {
        switch ( m_secondShapeType )
        {
        case Shape::Line :
        {
            //m_intersectionPoints[ 0 ]->SetIntersectionType( GSI::IntersectionType::LineWithLine );
            GSI::GetIntersectionManager().AddPointWithIntersectionType(
                m_intersectionPoints[ 0 ], GSI::IntersectionType::LineWithLine );

            break;
        }
        case Shape::LineSegmnet :
        {
            //m_intersectionPoints[ 0 ]->SetIntersectionType( GSI::IntersectionType::LineWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType(
                m_intersectionPoints[ 0 ], GSI::IntersectionType::LineWithLineSegment );

            break;
        }
        }

        break;
    }
    case Shape::LineSegmnet :
    {
        switch ( m_secondShapeType )
        {
        case Shape::Line :
        {
            //m_intersectionPoints[ 0 ]->SetIntersectionType( GSI::IntersectionType::LineWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType(
                m_intersectionPoints[ 0 ], GSI::IntersectionType::LineWithLineSegment );

            break;
        }
        case Shape::LineSegmnet :
        {
            //m_intersectionPoints[ 0 ]->SetIntersectionType( GSI::IntersectionType::LineSegmentsWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType(
                m_intersectionPoints[ 0 ], GSI::IntersectionType::LineSegmentsWithLineSegment );

            break;
        }
        case Shape::Circle :
        {
            //m_intersectionPoints[ 0 ]->SetIntersectionType( GSI::IntersectionType::CircleWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType(
                m_intersectionPoints[ 0 ], GSI::IntersectionType::CircleWithLineSegment );

            break;
        }
        }

        break;
    }
    case Shape::Circle :
    {
        switch ( m_secondShapeType )
        {
        case Shape::LineSegmnet :
        {
            //m_intersectionPoints[ 0 ]->SetIntersectionType( GSI::IntersectionType::CircleWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType(
                m_intersectionPoints[ 0 ], GSI::IntersectionType::CircleWithLineSegment );

            break;
        }
        }

        break;
    }
    }
}

void GSBuilders::Additional::IntersectionPointsBuilder::CreateIntersectionPoints( QPointF const & pos1, QPointF const & pos2 )
{
    GraphicsPoint * p1 = new GraphicsPoint( pos1 );
    GraphicsPoint * p2 = new GraphicsPoint( pos2 );
    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( p1 );
    GeomConstr::Objects::GetConstructionData().StoreGraphicsPoint( p2 );

    m_intersectionPoints.reserve( 2 );

    m_intersectionPoints.push_back( p1 );
    m_intersectionPoints.push_back( p2 );

    //p1->SetAsIntersectionPoint();
    //p2->SetAsIntersectionPoint();

    switch ( m_firstShapeType )
    {
    case Shape::Circle :
    {
        switch ( m_secondShapeType )
        {
        case Shape::Line :
        {
            //p1->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLine );
            //p2->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLine );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p1, GSI::IntersectionType::CircleWithLine );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p2, GSI::IntersectionType::CircleWithLine );

            break;
        }
        case Shape::LineSegmnet :
        {
            //p1->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLineSegment );
            //p2->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p1, GSI::IntersectionType::CircleWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p2, GSI::IntersectionType::CircleWithLineSegment );

            break;
        }
        case Shape::Circle :
        {
            //p1->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithCircle );
            //p2->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithCircle );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p1, GSI::IntersectionType::CircleWithCircle );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p2, GSI::IntersectionType::CircleWithCircle );

            break;
        }
        }

        break;
    }
    case Shape::Line :
    {
        switch ( m_secondShapeType )
        {
        case Shape::Circle :
        {
            //p1->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLine );
            //p2->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLine );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p1, GSI::IntersectionType::CircleWithLine );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p2, GSI::IntersectionType::CircleWithLine );

            break;
        }
        }

        break;
    }
    case Shape::LineSegmnet :
    {
        switch ( m_secondShapeType )
        {
        case Shape::Circle :
        {
            //p1->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLineSegment );
            //p2->SetIntersectionType( GraphicsPoint::IntersectionType::CircleWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p1, GSI::IntersectionType::CircleWithLineSegment );
            GSI::GetIntersectionManager().AddPointWithIntersectionType( p2, GSI::IntersectionType::CircleWithLineSegment );

            break;
        }
        }

        break;
    }
    }
}

void GSBuilders::Additional::IntersectionPointsBuilder::Reset()
{
    m_firstShapeType = Shape::None;
    m_secondShapeType = Shape::None;
    m_firstShapeIsSelected = false;
    m_ls1 = nullptr;
    m_ls2 = nullptr;
    m_l1 = nullptr;
    m_l2 = nullptr;
    m_c1 = nullptr;
    m_c2 = nullptr;
    m_intersectionPoints.clear();
}

void GSBuilders::Additional::IntersectionPointsBuilder::ShowMessageAboutNoIntersection()
{
    QMessageBox::warning( 0,
                          QObject::tr( "Message" ),
                          QObject::tr( "No intersection." ) );
}

