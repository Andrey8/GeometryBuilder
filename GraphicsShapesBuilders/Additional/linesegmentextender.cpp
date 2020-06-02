#include "GraphicsShapesBuilders/Additional/linesegmentextender.h"
#include "Helpers/constructiondata.h"
#include "Helpers/objects.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsShapes/graphicsline.h"


void GSBuilders::Additional::LineSegmentExtender::SetLineSegment( GraphicsLineSegment * ls )
{
    m_lineSegment = ls;
}

void GSBuilders::Additional::LineSegmentExtender::CreateLine()
{
    GraphicsPoint * p1 = m_lineSegment->p1();
    GraphicsPoint * p2 = m_lineSegment->p2();
    QList< GraphicsPoint * > const points = Helpers::Objects::GetConstructionData().GetInteriorPoints( m_lineSegment );

    m_line = new GraphicsLine( p1, p2 );
    //p1->AddLineAsDefined( m_line );
    //p2->AddLineAsDefined( m_line );
    Helpers::Objects::GetConstructionData().AddLineAsDefined( p1, m_line );
    Helpers::Objects::GetConstructionData().AddLineAsDefined( p2, m_line );
    foreach ( GraphicsPoint * p, points )
    {
        //p->RemoveNondefinedLineSegment( m_lineSegment );
        Helpers::Objects::GetConstructionData().RemoveNondefinedLineSegment( p, m_lineSegment );

        m_line->PutPoint( p );
    }
    //p1->RemoveDefinedLineSegment( m_lineSegment );
    //p2->RemoveDefinedLineSegment( m_lineSegment );
    Helpers::Objects::GetConstructionData().RemoveDefinedLineSegment( p1, m_lineSegment );
    Helpers::Objects::GetConstructionData().RemoveDefinedLineSegment( p2, m_lineSegment );

    Helpers::Objects::GetConstructionData().StoreGraphicsLine( m_line );
}

GraphicsLine * GSBuilders::Additional::LineSegmentExtender::GetLine()
{
    return m_line;
}

void GSBuilders::Additional::LineSegmentExtender::DestroyLineSegment()
{
    //delete m_lineSegment;
    m_lineSegment = nullptr;
}

void GSBuilders::Additional::LineSegmentExtender::Reset()
{
    m_lineSegment = nullptr;
    m_line = nullptr;
}

