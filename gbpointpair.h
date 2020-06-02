#pragma once

#include "GraphicsShapes//graphicspoint.h"



class GBPointPair
{
public:
    // getters
    GraphicsPoint * GetFirst() const {
        return m_first;
    }
    GraphicsPoint * GetSecond() const {
        return m_second;
    }
    GBPointPair GetSwaped() const {
        return GBPointPair( m_second, m_first );
    }
    GBPointPair GetSorted() const
    {
        if ( m_first->X() == m_second->X() )
        {
            if ( m_first->Y() <= m_second->Y() )
            {
                return *this;
            }
            else
            {
                return this->GetSwaped();
            }
        }
        else
        {
            if ( m_first->X() < m_second->X() )
            {
                return *this;
            }
            else
            {
                return this->GetSwaped();
            }
        }
    }
    GraphicsPoint * GetOtherItem( GraphicsPoint const * item ) const
    {
        if ( item != m_first && item != m_second )
        {
            throw Exception( "GBPointPair::GetOtherItem : pair do not contain item." );
        }
        else if ( item == m_first )
        {
            return m_second;
        }
        else if ( item == m_second )
        {
            return m_first;
        }
    }

    // setters
    void SetPoints( GraphicsPoint * first, GraphicsPoint * second )
    {
        if ( !first || !second )
        {
            // throw
        }

        m_first = first;
        m_second = second;
    }
    void SetFirst( GraphicsPoint * first ) {
        m_first = first;
    }
    void SetSecond( GraphicsPoint * second ) {
        m_second = second;
    }
    void SetFirstCenter( QPointF const & center )
    {
        if ( m_first )
        {
            m_first->SetPosition( center );
        }
        else
        {
            // throw
        }
    }
    void SetSecondCenter( QPointF const & center )
    {
        if ( m_second )
        {
            m_second->SetPosition( center );
        }
        else
        {
            // throw
        }
    }

    // adders
    void AddPoint( GraphicsPoint * p )
    {
        if ( p == nullptr )
        {
            // throw exception
        }
        else
        {
            if ( m_first == nullptr )
            {
                m_first = p;
            }
            else if ( m_second == nullptr )
            {
                m_second = p;
            }
            else
            {
                // throw exception
            }
        }
    }

    void Invalidate()
    {
        m_first = nullptr;
        m_second = nullptr;
    }

    // questions
    bool IsValid() const
    {
        return ( m_first && m_second );
    }
    bool ContainsItem( GraphicsPoint const * point ) const
    {
        return ( point == m_first || point == m_second );
    }

    GBPointPair( GraphicsPoint * first, GraphicsPoint * second )
        : m_first( first ),
          m_second( second )
    {}
    GBPointPair()
        : m_first( nullptr ),
          m_second( nullptr )
    {}
private:
    GraphicsPoint * m_first;
    GraphicsPoint * m_second;
};

