#include "GraphicsShapesBuilders/linebuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"
#include "GraphicsShapes/helpers.h"



GraphicsLine * GSBuilders::LineBuilder::GetLine()
{
    if ( m_line )
    {
        return m_line;
    }
    else
    {
        // throw
    }
}

GraphicsLine * GSBuilders::LineBuilder::GetTempLine()
{
    if ( m_tempLine )
    {
        return m_tempLine;
    }
    else
    {
        // throw
    }
}

GraphicsPoint * GSBuilders::LineBuilder::GetFirstPoint()
{
    if ( m_p1 )
    {
        return m_p1;
    }
    else
    {
        // throw
    }
}

GraphicsPoint * GSBuilders::LineBuilder::GetSecondPoint()
{
    if ( m_p2 )
    {
        return m_p2;
    }
    else
    {
        // throw
    }
}

GraphicsPoint * GSBuilders::LineBuilder::GetTempPoint()
{
    return m_tempPoint;
}

void GSBuilders::LineBuilder::SetFirstPoint( GraphicsPoint * p )
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

void GSBuilders::LineBuilder::SetSecondPoint( GraphicsPoint * p )
{
    if ( p )
    {
        m_p2 = p;
    }
    else
    {
        // throw
    }
}

void GSBuilders::LineBuilder::SetTempPointCenter( QPointF const & c )
{
    m_tempPoint->SetPosition( c );
}

void GSBuilders::LineBuilder::SetLineAsFirstPointOwner( GraphicsLine * l )
{
    m_firstPointOwnerLine = l;
}

void GSBuilders::LineBuilder::SetLineSegmentAsFirstPointOwner( GraphicsLineSegment * ls )
{
    m_firstPointOwnerLineSegment = ls;
}

bool GSBuilders::LineBuilder::FirstPointIsOnLine( GraphicsLine * l )
{
    return ( m_firstPointOwnerLine == l );
}

bool GSBuilders::LineBuilder::FirstPointIsOnLineSegment( GraphicsLineSegment * ls )
{
    return ( m_firstPointOwnerLineSegment == ls );
}

void GSBuilders::LineBuilder::RecalculateTempLine()
{
    if ( m_tempLine )
    {
        m_tempLine->CalculateGeometryData();
    }
    else
    {
        // throw
    }
}

void GSBuilders::LineBuilder::CreateLine()
{
    if ( m_p1 && m_p2 )
    {
        m_line = new GraphicsLine( m_p1, m_p2 );
        //m_p1->AddLineAsDefined( m_line );
        //m_p2->AddLineAsDefined( m_line );

        Helpers::Objects::GetConstructionData().StoreGraphicsLine( m_line );
        Helpers::Objects::GetConstructionData().AddLineAsDefined( m_p1, m_line );
        Helpers::Objects::GetConstructionData().AddLineAsDefined( m_p2, m_line );
    }
    else
    {
        // throw
    }
}

void GSBuilders::LineBuilder::CreateTempLine()
{
    if ( m_p1 && m_tempPoint )
    {
        m_tempLine = new GraphicsLine( m_p1, m_tempPoint );
        //DebugHelper::ShowTextInInput( "uuuuuu" );

        m_tempLine->setZValue( -10 );
        m_tempLine->SetInBuildingProcess( true );
    }
    else
    {
        // throw
    }
}

void GSBuilders::LineBuilder::CreateFirstPoint( QPointF const & c )
{
    if ( !m_p1 )
    {
        m_p1 = new GraphicsPoint( c );
        m_firstPointIsAdded = true;

        Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p1 );
    }
    else
    {
        // throw
    }
}

void GSBuilders::LineBuilder::CreateFirstPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * segment )
{
    GraphicsLine * line = new GraphicsLine( segment->p1(), segment->p2() );
    QPointF const center = GSHelpers::GetPerpendicularBasePoint( scenePos, line );

    m_p1 = new GraphicsPoint( center );
    m_firstPointIsAdded = true;

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p1 );
}

void GSBuilders::LineBuilder::CreateFirstPointOnCircle( QPointF const & scenePos, GraphicsCircle * c )
{
    QLineF const line( c->GetCenterPosition(), scenePos );
    qreal const d = line.length();

    m_p1 = new GraphicsPoint( line.pointAt( c->GetRadius() / d ) );
    m_firstPointIsAdded = true;

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p1 );
}

void GSBuilders::LineBuilder::CreateSecondPoint( QPointF const & c )
{
    if ( !m_p2 )
    {
        m_p2 = new GraphicsPoint( c );

        Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p2 );
    }
    else
    {
        // throw
    }
}

void GSBuilders::LineBuilder::CreateSecondPointOnLineSegment( QPointF const & scenePos, GraphicsLineSegment * segment )
{
    GraphicsLine * line = new GraphicsLine( segment->p1(), segment->p2() );
    QPointF const center = GSHelpers::GetPerpendicularBasePoint( scenePos, line );

    m_p2 = new GraphicsPoint( center );

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p2 );
}

void GSBuilders::LineBuilder::CreateSecondPointOnCircle( QPointF const & scenePos, GraphicsCircle * c )
{
    QLineF const line( c->GetCenterPosition(), scenePos );
    qreal const d = line.length();

    m_p2 = new GraphicsPoint( line.pointAt( c->GetRadius() / d ) );

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_p2 );
}

void GSBuilders::LineBuilder::CreateTempPoint( QPointF const & c )
{
    m_tempPoint = new GraphicsPoint( c );
}

void GSBuilders::LineBuilder::DestroyTempLine()
{
    delete m_tempLine;
    m_tempLine = nullptr;
}

void GSBuilders::LineBuilder::DestroyTempPoint()
{
    delete m_tempPoint;
    m_tempPoint = nullptr;
}

void GSBuilders::LineBuilder::Reset()
{
    m_line = nullptr;
    m_tempLine = nullptr;
    m_p1 = nullptr;
    m_p2 = nullptr;
    m_firstPointIsAdded = false;
    m_tempPoint = nullptr;
    m_firstPointOwnerLineSegment = nullptr;
    m_firstPointOwnerLine = nullptr;
}

int GSBuilders::LineBuilder::ShowMessageAboutBuildingOnLineSegment()
{
    QMessageBox::warning( 0, QObject::tr( "Message" ), QObject::tr( "It is impossible." ) );
}

