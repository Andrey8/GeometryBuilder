#include "GraphicsShapesIntersections/intersectionsmanager.h"

#include <iostream>



GSI::CircleWithLineIntersectionData const & GSI::IntersectionManager::GetIntersectionData( GraphicsCircle * c, GraphicsLine * l ) const
{
    for ( ShapesPairWithIntersectionData const & pairWithData : m_shapesPairsWithIntersectionDatas )
    {
        if ( pairWithData.GetCircle() == c && pairWithData.GetLine() == l )
        {
            return pairWithData.GetCircleWithLineIntersectionData();
        }
    }

    throw Exception( "GGItemsHelpers::IntersectionManager::GetIntersectionPoints : circle and line not found as intersecting." );
}

GSI::CircleWithLineSegmentIntersectionData const & GSI::IntersectionManager::GetIntersectionData( GraphicsCircle * c, GraphicsLineSegment * ls ) const
{
    for ( ShapesPairWithIntersectionData const & pairWithData : m_shapesPairsWithIntersectionDatas )
    {
        if ( pairWithData.GetCircle() == c && pairWithData.GetLineSegment() == ls )
        {
            return pairWithData.GetCircleWithLineSegmentIntersectionData();
        }
    }

    throw Exception( "GGItemsHelpers::IntersectionManager::GetIntersectionPoints : circle and line not found as intersecting." );
}

GSI::IntersectionType GSI::IntersectionManager::GetIntersectionType( GraphicsPoint * p ) const
{
    try
    {
        return m_pointsWithIntersectionTypes.at( p );
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}



bool GSI::IntersectionManager::IsIntersectionPoint( GraphicsPoint * p ) const
{
    return m_pointsWithIntersectionTypes.at( p ) != IntersectionType::None;
}



void GSI::IntersectionManager::RewriteData( GraphicsCircle * c, GraphicsLine * l, CircleWithLineIntersectionData const & data )
{
    for ( ShapesPairWithIntersectionData & pairWithData : m_shapesPairsWithIntersectionDatas )
    {
        if ( pairWithData.GetCircle() == c && pairWithData.GetLine() == l )
        {
            pairWithData.SetIntersectionData( data );

            break;
        }
    }
}

void GSI::IntersectionManager::AddData( GraphicsCircle * c, GraphicsLine * l, CircleWithLineIntersectionData const & data )
{
    m_shapesPairsWithIntersectionDatas.push_back( ShapesPairWithIntersectionData( c, l, data ) );
}

void GSI::IntersectionManager::AddData( GraphicsCircle * c, GraphicsLineSegment * ls, CircleWithLineSegmentIntersectionData const & data )
{
    m_shapesPairsWithIntersectionDatas.push_back( ShapesPairWithIntersectionData( c, ls, data ) );
}

void GSI::IntersectionManager::AddPointWithIntersectionType( GraphicsPoint * p, IntersectionType type )
{
    MapIterator it = m_pointsWithIntersectionTypes.find( p );
    if ( it != m_pointsWithIntersectionTypes.end() )
    {
        m_pointsWithIntersectionTypes.at( p ) = type;
    }
    else
    {
        m_pointsWithIntersectionTypes.insert( std::make_pair( p, type ) );
    }
}
