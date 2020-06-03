#include "DebugHelpers/constructioninfowriter.h"
#include "GraphicsShapes/graphicspoint.h"

//#include <QFile>
#include <QTextStream>



DebugHelper::ConstructionInfoWriter::ConstructionInfoWriter()
    : m_fileName( QString( "C:/Users/Andrey/Documents/GeometryBuilder/LogFiles/Info.txt" ) ),
      m_file( m_fileName )
{
    if ( !m_file.open( //QIODevice::Append |
                       QIODevice::WriteOnly |
                       QIODevice::Text ) )
    {
        // throw
    }
}

void DebugHelper::ConstructionInfoWriter::WriteToFile( GraphicsPoint * p )
{
    QTextStream stream( &m_file );
    QPointF const pos = p->GetPosition();
    stream << "Point "
           << m_chg.GetNext()
           << " : ( "
           << pos.x()
           << ", "
           << pos.y()
           << " )\n" ;
}

//DebugHelper::ConstructionInfoWriter::~ConstructionInfoWriter() {}
