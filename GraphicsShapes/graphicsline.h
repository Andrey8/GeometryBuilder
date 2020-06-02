#pragma once

#include "GraphicsShapes/graphicspoint.h"
#include "GraphicsShapes/graphicslinesegment.h"
#include "gbpointpair.h"
#include "gvector.h"
#include "DebugHelpers/debughelper.h"
#include "BSEHandlers/bsehandlerforline.h"

#include <QGraphicsItem>
#include <QVector>

class QPainter;
class QPointF;
class QRectF;



class GraphicsLine : public QGraphicsItem
{
public:
    GraphicsLine( GraphicsPoint * p1, GraphicsPoint * p2 );

    // getters
    QList< GraphicsLineSegment * > const & GetPartitionSegments() const { return m_partitionSegments; }
    GBPointPair const & GetDefiningPoints() const { return m_definingPoints; }
    QList< GraphicsPoint * > GetNondefiningPoints() const;
    GB::LinePaintMode GetPaintMode() const { return m_paintMode; }
    QPainterPath GetLinearPath() { return m_linearPath; }
    GBPointPair const & GetExtensionEnds() const { return m_extensionEnds; }
    GeomShapes::Line GetLine() const { return GeomShapes::Line( m_definingPoints.GetFirst()->GetPosition(), m_definingPoints.GetSecond()->GetPosition() ); }
    GraphicsLineSegment * GetInitialSegment() const { return m_initialSegment; }

    // setters
    void SetPaintMode( GB::LinePaintMode mode );
    void SetInBuildingProcess( bool value ) { m_isInBuildingProcess = value; }

    // questions
    bool IsVertical() const;
    bool IsHorizontal() const;
    bool Contains( GraphicsPoint * point ) const;
    bool HasSegment( GraphicsLineSegment * segment ) const;
    bool ContainsPointAsDefining( GraphicsPoint const * point ) const;

    void PutPoint( GraphicsPoint * p, GraphicsLineSegment * segment );
    void PutPoint( GraphicsPoint * p );

    bool operator==( GraphicsLine const & other ) const;

    void CalculateGeometryData();
    void CalculatePaintData();

    QPainterPath shape() const override;
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) override;
    QRectF boundingRect() const override;

    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
private:
    GBPointPair m_definingPoints;

    GB::LinePaintMode m_paintMode;
    GB::LinePaintMode m_previousPaintMode;

    QList< GraphicsPoint * > m_points;
    QList< GraphicsLineSegment * > m_partitionSegments;
    GBPointPair m_ends;
    GBPointPair m_extensionEnds;
    QPair< GraphicsLineSegment *, GraphicsLineSegment * > m_extendingSegments;
    GraphicsLineSegment * m_initialSegment;

    QPainterPath m_selectionAreaPath;
    QPainterPath m_linearPath;

    bool m_isInBuildingProcess;



    GBPointPair GetExtensionEnds( GraphicsPoint * newBeg, GraphicsPoint * newEnd, QSizeF const & areaSize ) const;
    QPair< GraphicsLineSegment *, GraphicsLineSegment * > GetExtendingSegments();
    void DestroyOldGeometryData();
    void CalculateSelectionArea();
    void CalculateLinearPath();
};
