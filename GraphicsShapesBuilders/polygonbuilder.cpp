#include "GraphicsShapesBuilders/polygonbuilder.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicspolygon.h"
#include "Helpers/objects.h"
#include "Helpers/constructiondata.h"



void GSBuilders::PolygonBuilder::CreateStartPoint( QPointF const & c )
{
    m_startPoint = new GraphicsPoint( c );
    m_polygonStartPointIsAdded = true;
    m_vertices.push_back( m_startPoint );
    m_lastVertex = m_startPoint;

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_startPoint );
}

void GSBuilders::PolygonBuilder::CreateTempPoint( QPointF const & c )
{
    m_tempPoint = new GraphicsPoint( c );
}

void GSBuilders::PolygonBuilder::CreateFirstTempSide()
{
    if ( !m_tempSideFromLastVertex && m_startPoint && m_tempPoint )
    {
        m_tempSideFromLastVertex = new GraphicsLineSegment( m_startPoint, m_tempPoint );
    }
    else
    {
        // throw
    }
}

void GSBuilders::PolygonBuilder::CreateVertex( QPointF const & c )
{
    m_currentVertex = new GraphicsPoint( c );
    m_vertices.push_back( m_currentVertex );

    Helpers::Objects::GetConstructionData().StoreGraphicsPoint( m_currentVertex );
}

void GSBuilders::PolygonBuilder::CreateSide()
{
    if ( m_lastVertex && m_currentVertex )
    {
        m_currentSide = new GraphicsLineSegment( m_lastVertex, m_currentVertex );
        m_edges.push_back( m_currentSide );
        //m_lastVertex->AddLineSegmentAsDefined( m_currentSide );
        //m_currentVertex->AddLineSegmentAsDefined( m_currentSide );
        m_lastVertex = m_currentVertex;

        Helpers::Objects::GetConstructionData().StoreGraphicsLineSegment( m_currentSide );
        Helpers::Objects::GetConstructionData().AddLineSegmentAsDefined( m_lastVertex, m_currentSide );
        Helpers::Objects::GetConstructionData().AddLineSegmentAsDefined( m_currentVertex, m_currentSide );
    }
    else
    {
        // throw
    }
}

void GSBuilders::PolygonBuilder::CreateTempSideFromLastVertex()
{
    if ( !m_tempSideFromLastVertex && m_currentVertex && m_tempPoint )
    {
        m_tempSideFromLastVertex = new GraphicsLineSegment( m_lastVertex, m_tempPoint );
    }
    else
    {
        // throw
    }
}

void GSBuilders::PolygonBuilder::CreateTempSideFromStartPoint()
{
    m_tempSideFromStartPoint = new GraphicsLineSegment( m_startPoint, m_tempPoint );
    m_tempSideFromStartPoint->SetAsInPolygonBuildingProcess( true );
}

void GSBuilders::PolygonBuilder::CreateLastSide()
{
    m_currentSide = new GraphicsLineSegment( m_lastVertex, m_startPoint );
    m_edges.push_back( m_currentSide );
    //m_lastVertex->AddLineSegmentAsDefined( m_currentSide );
    //m_startPoint->AddLineSegmentAsDefined( m_currentSide );
    m_polygonStartPointIsAdded = false;

    Helpers::Objects::GetConstructionData().StoreGraphicsLineSegment( m_currentSide );
    Helpers::Objects::GetConstructionData().AddLineSegmentAsDefined( m_lastVertex, m_currentSide );
    Helpers::Objects::GetConstructionData().AddLineSegmentAsDefined( m_startPoint, m_currentSide );
}

void GSBuilders::PolygonBuilder::CreateGraphicsPolygon()
{
    m_polygon = new GraphicsPolygon( m_vertices, m_edges );

    //for ( GBLineSegment * e : m_edges ) { e->hide(); }

    for ( GraphicsPoint * p : m_vertices )
    {
        Helpers::Objects::GetConstructionData().AddPolygon( p, m_polygon );
        //p->AddPolygon( m_polygon );
    }

    Helpers::Objects::GetConstructionData().StoreGraphicsPolygon( m_polygon );
}

GraphicsPoint * GSBuilders::PolygonBuilder::GetStartPoint()
{
    if ( m_startPoint )
    {
        return m_startPoint;
    }
    else
    {
        // throw
    }
}

GraphicsLineSegment * GSBuilders::PolygonBuilder::GetSide()
{
    if ( m_currentSide )
    {
        return m_currentSide;
    }
    else
    {
        // throw
    }
}

GraphicsPoint * GSBuilders::PolygonBuilder::GetVertex()
{
    if ( m_currentVertex )
    {
        return m_currentVertex;
    }
    else
    {
        // throw
    }
}

GraphicsLineSegment * GSBuilders::PolygonBuilder::GetTempSideFromLastVertex()
{
    return m_tempSideFromLastVertex;
}

GraphicsLineSegment * GSBuilders::PolygonBuilder::GetTempSideFromStartPoint()
{
    return m_tempSideFromStartPoint;
}

void GSBuilders::PolygonBuilder::SetTempPointPosition( QPointF const & c )
{
    if ( m_tempPoint )
    {
        m_tempPoint->SetPosition( c );
    }
    else
    {
        // throw
    }
}

bool GSBuilders::PolygonBuilder::IsItStartPoint( GraphicsPoint * p )
{
    return ( p == m_startPoint );
}

void GSBuilders::PolygonBuilder::DestroyTempSideFromLastVertex()
{
    delete m_tempSideFromLastVertex;
    m_tempSideFromLastVertex = nullptr;
}

void GSBuilders::PolygonBuilder::DestroyTempSideFromStartPoint()
{
    delete m_tempSideFromStartPoint;
    m_tempSideFromStartPoint = nullptr;
}

void GSBuilders::PolygonBuilder::DestroyTempPoint()
{
    delete m_tempPoint;
    m_tempPoint = nullptr;
}

void GSBuilders::PolygonBuilder::Reset()
{
    m_polygon = nullptr;
    m_vertices.clear();
    m_edges.clear();
    m_startPoint = nullptr;
    m_currentVertex = nullptr;
    m_lastVertex = nullptr;
    m_currentSide = nullptr;
    m_tempSideFromLastVertex = nullptr;
    m_polygonStartPointIsAdded = false;
    m_tempPoint = nullptr;
}

void GSBuilders::PolygonBuilder::ShowMessageAboutPolygonVerticesCount()
{
    QMessageBox * message = new QMessageBox;

    message->setText( "Polygon must have at least 3 vertices" );
    message->setIcon( QMessageBox::Warning );

    message->show();
}
