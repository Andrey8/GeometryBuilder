#pragma once

#include <QGraphicsItem>

class QPainter;
class QPointF;
class QRectF;



namespace Tests
{
    namespace GUI
    {
        class Rect : public QGraphicsItem
        {
        public:
            Rect( QPointF const & pos, double w, double h );

            void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) override;
            QRectF boundingRect() const override;
            QPainterPath shape() const override;

            void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
            void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override {}
            void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override {}

        private:
            QPointF m_position;
            double m_width;
            double m_height;
        };
    }
}
