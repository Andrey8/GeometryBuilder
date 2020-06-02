#pragma once

#include <vector>

#include <QPointF>

class GraphicsPoint;
class GraphicsLineSegment;
class GraphicsLine;
class GraphicsCircle;
class GraphicsPolygon;





namespace GSBuilders
{
	namespace Additional
	{
		class IntersectionPointsBuilder
		{
		public:
			enum class Shape
			{
				None, Line, LineSegmnet, Circle
			};

			IntersectionPointsBuilder()
				: m_firstShapeType ( IntersectionPointsBuilder::Shape::None ),
				  m_secondShapeType ( IntersectionPointsBuilder::Shape::None ),
				  m_firstShapeIsSelected ( false ),
				  m_ls1 ( nullptr ),
				  m_ls2 ( nullptr ),
				  m_l1 ( nullptr ),
				  m_l2 ( nullptr ),
				  m_c1 ( nullptr ),
				  m_c2 ( nullptr )
			{}

			void CreateIntersectionPoint( QPointF const & p );
			void CreateIntersectionPoints( QPointF const & p1, QPointF const & p2 );

			Shape GetFirstShapeType();
			GraphicsLineSegment * GetFirstLineSegment();
			GraphicsLineSegment * GetSecondLineSegment();
			GraphicsLine * GetFirstLine();
			GraphicsLine * GetSecondLine();
			GraphicsCircle * GetFirstCircle() const { return m_c1; }
			GraphicsCircle * GetSecondCircle() const { return m_c2; }
			GraphicsLineSegment * GetOneLineSegment();
			GraphicsLine * GetOneLine();
			GraphicsCircle * GetOneCircle();
			GraphicsPoint * GetOnlyIntersectionPoint();
			std::vector< GraphicsPoint * > const & GetIntersectionPoints() const { return m_intersectionPoints; }

			void SetFirstShape( GraphicsLine * );
			void SetFirstShape( GraphicsLineSegment * );
			void SetFirstShape( GraphicsCircle * );
			void SetSecondShape( GraphicsLine * );
			void SetSecondShape( GraphicsLineSegment * );
			void SetSecondShape( GraphicsCircle * );

			bool FirstShapeIsSelected();

			void Reset();

			void ShowMessageAboutNoIntersection();
		private:
			Shape m_firstShapeType;
			Shape m_secondShapeType;
			bool m_firstShapeIsSelected;

			GraphicsLineSegment * m_ls1;
			GraphicsLineSegment * m_ls2;
			GraphicsLine * m_l1;
			GraphicsLine * m_l2;
			GraphicsCircle * m_c1;
			GraphicsCircle * m_c2;

			std::vector< GraphicsPoint * > m_intersectionPoints;
		};
	}
}
