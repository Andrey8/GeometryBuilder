#include "GraphicsShapesBuilders/linesegmentbuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"
#include "GraphicsShapes/helpers.h"



GraphicsLineSegment * GSBuilders::LineSegmentBuilder::GetLineSegment()
{
    if ( m_lineSegment )
    {
        return m_lineSegment;
    }
    else
    {
        // throw exception
    }
}

GraphicsLineSegment * GSBuilders::LineSegmentBuilder::GetTempLineSegment()
{
    if ( m_tempLineSegment )
    {
        return m_tempLineSegment;
    }
    else
    {
        // throw exception
    }
}

GraphicsPoint * GSBuilders::LineSegmentBuilder::GetFirstPoint()
{
    if ( m_p1 )
    {
        return m_p1;
    }
    else
    {
        // throw exception
    }
}

GraphicsPoint * GSBuilders::LineSegmentBuilder::GetSecondPoint()
{
    if ( m_p2 )
    {
        return m_p2;
    }
    else
    {
        // throw exception
    }
}

void GSBuilders::LineSegmentBuilder::SetFirstPoint( GraphicsPoint * p )
{
    if ( p )
    {
        m_p1 = p;
        m_firstPointIsAdded = true;
    }
    else
    {
        // throw
    }
}

void GSBuilders::LineSegmentBuilder::SetSecondPoint( GraphicsPoint * p )
{
    if ( p )
    {
        m_p2 = p;
    }
    else
    {
        // throw exception
    }
}

void GSBuilders::LineSegmentBuilder::SetTempPointCenter( QPointF const & newCenter )
{
    m_tempPoint->SetPosition( newCenter );
}

void GSBuilders::LineSegmentBuilder::SetLineAsFirstPointOwner( GraphicsLine * l )
{
    m_firstPointOwnerLine = l;
}

void GSBuilders::LineSegmentBuilder::SetLineSegmentAsFirstPointOwner( GraphicsLineSegment * ls )
{
    m_firstPointOwnerLineSegment = ls;
}

bool GSBuilders::LineSegmentBuilder::FirstPointIsOnLine( GraphicsLine * l )
{
    return ( m_firstPointOwnerLine == l );
}

bool GSBuilders::LineSegmentBuilder::FirstPointIsOnLineSegment( GraphicsLineSegment * ls )
{
    return ( m_firstPointOwnerLineSegment == ls );
}

void GSBuilders::LineSegmentBuilder::CreateFirstPoint( QPointF const & center )
{
    if ( !m_p1 )
    {
        m_p1 = new GraphicsPoint( center );
        m_firstPointIsAdded = true;

        Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p1 );
    }
    else
    {
        // throw exception
    }
}

void GSBuilders::LineSegmentBuilder::CreateFirstPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * segment )
{
    GraphicsLine * line = new GraphicsLine( segment->p1(), segment->p2() );
    QPointF const center = GSHelpers::GetPerpendicularBasePoint( scenePos, line );

    m_p1 = new GraphicsPoint( center );
    m_firstPointIsAdded = true;

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p1 );
}

void GSBuilders::LineSegmentBuilder::CreateFirstPointOnCircle( QPointF const & scenePos, GraphicsCircle * c )
{
    QLineF const line( c->GetCenterPosition(), scenePos );
    qreal const d = line.length();

    m_p1 = new GraphicsPoint( line.pointAt( c->GetRadius() / d ) );
    m_firstPointIsAdded = true;

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p1 );
}

void GSBuilders::LineSegmentBuilder::CreateSecondPoint( QPointF const & center )
{
    if ( !m_p2 )
    {
        m_p2 = new GraphicsPoint( center );

        Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p2 );
    }
    else
    {
        // throw exception
    }
}

void GSBuilders::LineSegmentBuilder::CreateSecondPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * segment )
{
    GraphicsLine * line = new GraphicsLine( segment->p1(), segment->p2() );
    QPointF const center = GSHelpers::GetPerpendicularBasePoint( scenePos, line );

    m_p2 = new GraphicsPoint( center );

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p2 );
}

void GSBuilders::LineSegmentBuilder::CreateSecondPointOnCircle( QPointF const & scenePos, GraphicsCircle * c )
{
    QLineF const line( c->GetCenterPosition(), scenePos );
    qreal const d = line.length();

    m_p2 = new GraphicsPoint( line.pointAt( c->GetRadius() / d ) );

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p2 );
}

void GSBuilders::LineSegmentBuilder::CreateTempPoint( QPointF const & center )
{
    m_tempPoint = new GraphicsPoint( center );
}

void GSBuilders::LineSegmentBuilder::CreateLineSegment()
{
    if ( m_p1 && m_p2 )
    {
        m_lineSegment = new GraphicsLineSegment( m_p1, m_p2 );

        Helpers::Objects::GetConstructionData().StoreGraphicsLineSegment( m_lineSegment );
        //m_lineSegment->SetAsContaining( true );
        Helpers::Objects::GetConstructionData().SetGLSAsContaining( m_lineSegment, true );

    }
    else
    {
        // throw exception
    }
}

void GSBuilders::LineSegmentBuilder::CreateTempLineSegment()
{
    if ( !m_tempLineSegment && m_p1 && m_tempPoint )
    {
        m_tempLineSegment = new GraphicsLineSegment( m_p1, m_tempPoint );
        //m_tempLineSegment->setZValue( 0 );
    }
}

void GSBuilders::LineSegmentBuilder::DestroyTempPoint()
{
    delete m_tempPoint;
    m_tempPoint = nullptr;
}

void GSBuilders::LineSegmentBuilder::DestroyTempLineSegment()
{
    delete m_tempLineSegment;
    m_tempLineSegment = nullptr;
}

void GSBuilders::LineSegmentBuilder::AttachLineSegmentToItsEndPoints()
{
    //m_p1->AddLineSegmentAsDefined( m_lineSegment );
    //m_p2->AddLineSegmentAsDefined( m_lineSegment );

    Helpers::Objects::GetConstructionData().AddLineSegmentAsDefined( m_p1, m_lineSegment );
    Helpers::Objects::GetConstructionData().AddLineSegmentAsDefined( m_p2, m_lineSegment );
}

void GSBuilders::LineSegmentBuilder::Reset()
{
    m_lineSegment = nullptr;
    m_tempLineSegment = nullptr;
    m_p1 = nullptr;
    m_p2 = nullptr;
    m_firstPointIsAdded = false;
    m_tempPoint = nullptr;
    m_firstPointOwnerLine = nullptr;
    m_firstPointOwnerLineSegment = nullptr;
}

