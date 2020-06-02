#pragma once

#include <QPointF>



namespace GeomShapes
{
	class Circle
	{
	public:
		Circle( QPointF const & c, double r )
			: m_center( c ),
			  m_radius( r )
		{}

		QPointF const & GetCenter() const { return m_center; }
		double GetRadius() const { return m_radius; }
	private:
		QPointF const m_center;
		double const m_radius;
	};
}
