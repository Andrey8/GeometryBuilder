#pragma once

#include <QGraphicsScene>
#include <QPointF>
#include <QList>

class GraphicsLine;
class GraphicsLineSegment;

class QGraphicsSceneMouseEvent;

namespace GSBuilders { class PolygonBuilder; }
namespace GSBuilders { namespace Additional { class LineSegmentExtender; } }
namespace GraphicsShapesBSEHandlers { class HandlerForScene; }



class PlaneScene : public QGraphicsScene
{
    Q_OBJECT

public:
    void ExtendLineSegment( GraphicsLineSegment * );

    PlaneScene();
public slots:
    void Clear();
signals:
    void MovingLineSegmentsShowingPrepared();
    void CharacteristicFieldShowingPrepared();
protected:
    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
private:
    GSBuilders::PolygonBuilder * m_polygonBuilder;
    GSBuilders::Additional::LineSegmentExtender * m_lineSegmentExtender;

    GraphicsShapesBSEHandlers::HandlerForScene * m_BSEHandler;

    void Test();
};

