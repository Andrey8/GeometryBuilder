#pragma once

#include <QList>

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsLine;
class GraphicsCircle;



namespace GPM
{
    enum class CircleMovingGuidePointType
    {
        Center,
        OnlyPointOnCircle,
        OneOfThreePointsOnCircle
    };

    struct CircleWithGuidePointType
    {
        CircleWithGuidePointType( GraphicsCircle * c, CircleMovingGuidePointType type )
            : m_circle( c ),
              m_guideType( type )
        {}

        GraphicsCircle * m_circle;
        CircleMovingGuidePointType m_guideType;
    };



    class MovingPointGroup
    {
    public:
        enum class JointShape
        {
            Line,
            LineSegment,
            Circle
        };

        enum class PointPositionType
        {
            InsideDefiningSegemnt,
            OutDefiningSegmentAndNearerToGuide,
            OutDefiningSegmentAndNearerToNotGuide,
            InsideLineSegment
        };

        struct PointWithStartPosData
        {
            PointWithStartPosData( GraphicsPoint * p, PointPositionType posType, qreal d )
                : m_point( p ),
                  m_posType( posType ),
                  m_distanceToRotationCenter( d )
            {}

            GraphicsPoint * m_point;
            PointPositionType m_posType;
            qreal m_distanceToRotationCenter;
        };

        void Move();

        // setters
        void SetStartPositionData();
        void SetPoints( QList< GraphicsPoint * > const & points ) { m_points = points; }

		// getters
		JointShape GetJointShape() const { return m_jointShape; }
		GraphicsLineSegment * GetJointLineSegment() const { return m_jointLineSegment; }
		GraphicsLine * GetJointLine() const { return m_jointLine; }
		QList< GraphicsPoint * > const & GetPoints() const { return m_points; }
		PointPositionType GetPositionTypeOf( GraphicsPoint * ) const;
		qreal GetDistanceFromRotationCenterTo( GraphicsPoint * ) const;
		QList< GraphicsPoint * > const & GetIntersectionPoints() const { return m_intersectionPoints; }

		bool IsEmpty() const { return m_points.isEmpty(); }

		void Clear();

		MovingPointGroup( GraphicsLine * jointLine, GraphicsPoint * guidePoint );
		MovingPointGroup( GraphicsLineSegment * jointLineSegment, GraphicsPoint * guidePoint );
		MovingPointGroup( GraphicsCircle * jointCircle, CircleMovingGuidePointType type, GraphicsPoint * guidePoint );
	private:
		QList< GraphicsPoint * > m_points;
		QList< PointWithStartPosData > m_pointsWithStartPosData;

		JointShape m_jointShape;
		GraphicsLine * m_jointLine;
		GraphicsLineSegment * m_jointLineSegment;
		GraphicsCircle * m_jointCircle;
		CircleMovingGuidePointType m_guidePointType;
		GraphicsPoint * m_guidePoint;

        QList< GraphicsPoint * > m_intersectionPoints;
    };
}
