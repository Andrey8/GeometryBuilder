#include "Helpers/constructiondata.h"
#include "Helpers/graphicslinesegmentdata.h"
#include "DebugHelpers/exception.h"
#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicscircle.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "GraphicsPointMoving/movingpointsgroup.h"
#include "GraphicsPointMoving/movingmanager.h"
#include "GraphicsPointMoving/objects.h"
#include "GraphicsShapesIntersections/intersectionsmanager.h"
#include "GraphicsShapesIntersections/objects.h"

#include <iostream>
#include <QDebug>



void GeomConstr::ConstructionData::StoreGraphicsPoint( GraphicsPoint * point )
{
    m_graphicsPoints.push_back( point );

    m_pointsWithDatas.insert( std::pair< GraphicsPoint *, GraphicsPointData * >(
                                  point,
                                  new GraphicsPointData() ) );

    GPM::Objects::GetMovingManager().AddNewPoint( point );

    GSI::GetIntersectionManager().AddPointWithIntersectionType(
        point, GSI::IntersectionType::None );
}

void GeomConstr::ConstructionData::StoreGraphicsLineSegment( GraphicsLineSegment * ls )
{
    m_graphicsLineSegments.push_back( ls );

    GraphicsLineSegmentData * data = new GraphicsLineSegmentData();
    data->m_geometryData = ls->GetGeometryData();
    data->m_locationData = ls->GetLocationData();
    m_lineSegmentsWithDatas.insert( std::make_pair( ls, data ) );
}



void GeomConstr::ConstructionData::AddLineSegmentAsDefined( GraphicsPoint * p, GraphicsLineSegment * ls )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        it->second->m_definedLineSegments.push_back( ls );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::AddLineSegmentAsNondefined( GraphicsPoint * p, GraphicsLineSegment * ls )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        it->second->m_nondefinedLineSegments.push_back( ls );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::AddLineAsDefined( GraphicsPoint * p, GraphicsLine * line )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        it->second->m_definedLines.push_back( line );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::AddLineAsNondefined( GraphicsPoint * p, GraphicsLine * line )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        it->second->m_nondefinedLines.push_back( line );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::AddCircleAsDefined( GraphicsPoint * p, GraphicsCircle * circle, GPM::CircleMovingGuidePointType type )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        it->second->m_definedCirclesWithData.push_back(
            new GPM::CircleWithGuidePointType( circle, type ) );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::AddCircleAsNondefined( GraphicsPoint * p, GraphicsCircle * circle )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        it->second->m_nondefinedCircles.push_back( circle );
        it->second->m_nondefinedCirclesWithAngles.push_back(
            CircleWithAngle( circle,
                             Math::GetPolarAngleOfVector(
                                 QLineF( circle->GetCenterPosition(), p->GetPosition() ) ) ) );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::AddPolygon( GraphicsPoint * p, GraphicsPolygon * pol )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        it->second->m_polygons.push_back( pol );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::AddPointAsLineSegmentInterior( GraphicsPoint * p, GraphicsLineSegment * ls )
{
    LSWithDatasMapIterator it = m_lineSegmentsWithDatas.find( ls );

    if ( it != m_lineSegmentsWithDatas.end() )
    {
        it->second->m_interiorPoints.push_back( p );
    }
    else
    {
        throw Exception( "ERROR" );
    }
}




void GeomConstr::ConstructionData::RemoveDefinedLineSegment( GraphicsPoint * p, GraphicsLineSegment * ls )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    it->second->m_definedLineSegments.remove( ls );
}

void GeomConstr::ConstructionData::RemoveNondefinedLineSegment( GraphicsPoint * p, GraphicsLineSegment * ls )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    it->second->m_nondefinedLineSegments.remove( ls );
}



Math::Angle const & GeomConstr::ConstructionData::GetAngleInNondefinedCircle( GraphicsPoint * p, GraphicsCircle * circle )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    if ( it != m_pointsWithDatas.end() )
    {
        foreach ( CircleWithAngle const & pair, it->second->m_nondefinedCirclesWithAngles )
        {
            if ( pair.m_circle == circle )
            {
                return pair.m_angle;
            }
        }

        throw Exception( "ERROR" );
    }

    throw Exception( "ERROR" );
}

std::list< GraphicsLineSegment * > const & GeomConstr::ConstructionData::GetDefinedLineSegments( GraphicsPoint * p ) const
{
    try
    {
        return m_pointsWithDatas.at( p )->m_definedLineSegments;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}

std::list< GraphicsLineSegment * > const & GeomConstr::ConstructionData::GetNondefinedLineSegments( GraphicsPoint * p ) const
{
    //MapIterator it = m_pointsWithDatas.find( p );
    try
    {
        return m_pointsWithDatas.at( p )->m_nondefinedLineSegments;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}

std::list< GraphicsLine * > const & GeomConstr::ConstructionData::GetDefinedLines( GraphicsPoint * p ) const
{
    try
    {
        return m_pointsWithDatas.at( p )->m_definedLines;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}

std::list< GraphicsLine * > const & GeomConstr::ConstructionData::GetNondefinedLines( GraphicsPoint * p ) const
{
    try
    {
        return m_pointsWithDatas.at( p )->m_nondefinedLines;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}

std::list< GPM::CircleWithGuidePointType * > const & GeomConstr::ConstructionData::GetDefinedCirclesWithData( GraphicsPoint * p ) const
{
    try
    {
        return m_pointsWithDatas.at( p )->m_definedCirclesWithData;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}

std::list< GraphicsCircle * > const & GeomConstr::ConstructionData::GetNondefinedCircles( GraphicsPoint * p ) const
{
    try
    {
        return m_pointsWithDatas.at( p )->m_nondefinedCircles;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}

uint16_t GeomConstr::ConstructionData::GetNondefinedShapesCount( GraphicsPoint * p ) const
{
    GraphicsPointData * gpdata = m_pointsWithDatas.at( p );

    return ( gpdata->m_nondefinedLineSegments.size() +
             gpdata->m_nondefinedLines.size() +
             gpdata->m_nondefinedCircles.size() );
}

std::list< GraphicsPolygon * > const & GeomConstr::ConstructionData::GetPolygons( GraphicsPoint * p ) const
{
    try
    {
        return m_pointsWithDatas.at( p )->m_polygons;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}

QList< GraphicsPoint * > const & GeomConstr::ConstructionData::GetInteriorPoints( GraphicsLineSegment * ls ) const
{
    try
    {
        return m_lineSegmentsWithDatas.at( ls )->m_interiorPoints;
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}



void GeomConstr::ConstructionData::SetGLSAsLinePart( GraphicsLineSegment * ls, bool isLinePart )
{
    LSWithDatasMapIterator it = m_lineSegmentsWithDatas.find( ls );

    if ( it != m_lineSegmentsWithDatas.end() )
    {
        it->second->m_locationData->m_isLinePart = isLinePart;
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::SetGLSAsLineSegmentPart( GraphicsLineSegment * ls, bool isLineSegmentPart )
{
    LSWithDatasMapIterator it = m_lineSegmentsWithDatas.find( ls );

    if ( it != m_lineSegmentsWithDatas.end() )
    {
        it->second->m_locationData->m_isLineSegmentPart = isLineSegmentPart;
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::SetGLSAsContaining( GraphicsLineSegment * ls, bool isContaining )
{
    LSWithDatasMapIterator it = m_lineSegmentsWithDatas.find( ls );

    if ( it != m_lineSegmentsWithDatas.end() )
    {
        it->second->m_locationData->m_isContaining = isContaining;
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::SetGLSContainingLine( GraphicsLineSegment * ls, GraphicsLine * line )
{
    LSWithDatasMapIterator it = m_lineSegmentsWithDatas.find( ls );

    if ( it != m_lineSegmentsWithDatas.end() )
    {
        it->second->m_locationData->m_containingLine = line;
    }
    else
    {
        throw Exception( "ERROR" );
    }
}

void GeomConstr::ConstructionData::SetGLSContainingLineSegment( GraphicsLineSegment * dest, GraphicsLineSegment * containing )
{
    LSWithDatasMapIterator it = m_lineSegmentsWithDatas.find( dest );

    if ( it != m_lineSegmentsWithDatas.end() )
    {
        it->second->m_locationData->m_containingLineSegment = containing;
    }
    else
    {
        throw Exception( "ERROR" );
    }
}



bool GeomConstr::ConstructionData::PointIsPolygonVertex( GraphicsPoint * p ) const
{
    try
    {
        return !m_pointsWithDatas.at( p )->m_polygons.empty();
    }
    catch ( std::out_of_range const & exc )
    {
        std::cout << exc.what() << std::endl ;
    }
}



void GeomConstr::ConstructionData::CalculateAngleInNondefinedCirclesOf( GraphicsPoint * p )
{
    PointsWithDatasMapIterator it = m_pointsWithDatas.find( p );

    std::list< CircleWithAngle > circlesWithAngles = it->second->m_nondefinedCirclesWithAngles;

    for ( auto it = circlesWithAngles.begin(); it != circlesWithAngles.end(); ++it )
    {
        GraphicsCircle * c = ( *it ).m_circle;

        ( *it ).m_angle = Math::GetPolarAngleOfVector(
                              QLineF( c->GetCenterPosition(), p->GetPosition() ) );
    }
}


