#pragma once

#include <map>

class GraphicsPoint;
namespace GPM { class MovingData; }
namespace Helpers { class ConstructionData; }

class QGraphicsSceneMouseEvent;



namespace GPM
{
    class MovingManager
    {
        typedef std::map< GraphicsPoint *, MovingData * >::iterator MapIterator;
    public:
        MovingManager();

        void AddNewPoint( GraphicsPoint * );

        void CalculateMovingData( GraphicsPoint * );
        void HandleMoveEvent( GraphicsPoint *, QGraphicsSceneMouseEvent * );
        void HandleReleaseEvent( GraphicsPoint *, QGraphicsSceneMouseEvent * );

        MovingData * GetMovingData( GraphicsPoint * ) const;
    private:
        std::map< GraphicsPoint *, MovingData * > m_pointsWithDatas;

        Helpers::ConstructionData & m_cdata;
    };
}
