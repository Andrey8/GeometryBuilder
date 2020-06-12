#include "Tests/GUI/rect.h"
#include "DebugHelpers/debughelper.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>



Tests::GUI::Rect::Rect( QPointF const & pos, double w, double h )
    : m_position( pos ),
      m_width( w ),
      m_height( h )
{
    setPos( m_position );

    //setAcceptHoverEvents( true );
}



QRectF Tests::GUI::Rect::boundingRect() const
{
    double adj = 2;

    return QRectF( -m_width / 2 - adj,
                   -m_height / 2 - adj,
                   m_width + 2 * adj,
                   m_height + 2 * adj );
}

QPainterPath Tests::GUI::Rect::shape() const
{
    QPainterPath path;
    path.addRect( boundingRect() );

    return path;
}

void Tests::GUI::Rect::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    painter->setPen( QPen( Qt::black, 5 ) );
    painter->setBrush( QBrush( Qt::darkYellow ) );

    painter->drawRect( boundingRect() );
}



void Tests::GUI::Rect::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    DebugHelper::ShowTextInInput( "Rect mouse press." );

    QGraphicsItem::mousePressEvent( event );
}
