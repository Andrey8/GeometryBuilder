#pragma once

#include <QList>

namespace GPM { class MovingPointGroup; }
namespace GeomConstr { class ConstructionData; }
class GraphicsLineSegment;
class GraphicsPoint;



namespace GPM
{
    class MovingData
    {
    public:
        MovingData();

        void SetGroups( QList< MovingPointGroup > const & groups ) { m_pointGroups = groups; }
        void SetStartPositionData();

        void DoNotAllowMoving() { m_movingIsAllowed = false; }

        void Clear();

		// getters
		QList< MovingPointGroup > const & GetPointGroups() const { return m_pointGroups; }
		QList< GraphicsLineSegment * > const & GetTransportLineSegments() const { return m_transportLineSegments; }

		bool MovingIsAllowed() const { return m_movingIsAllowed; }
    private:
        QList< MovingPointGroup > m_pointGroups;
        bool m_movingIsAllowed;

        QList< GraphicsLineSegment * > m_transportLineSegments;

        GeomConstr::ConstructionData const & m_cdata;
    };


}
