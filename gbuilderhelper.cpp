#include "gbuilderhelper.h"
#include "GraphicsShapes/graphicspoint.h"

#include <QPointF>



bool GBHelper::operator <( QPointF const & lhs, QPointF const & rhs )
{
    if ( lhs.x() < rhs.x() )
    {
        return true;
    }
    else if ( lhs.x() > rhs.x() )
    {
        return false;
    }
    else
    {
        if ( lhs.y() < rhs.y() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool GBHelper::operator <=( QPointF const & lhs, QPointF const & rhs )
{
    if ( lhs.x() < rhs.x() )
    {
        return true;
    }
    else if ( lhs.x() > rhs.x() )
    {
        return false;
    }
    else
    {
        if ( lhs.y() <= rhs.y() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
















