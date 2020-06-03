#pragma once

#include <QFile>

class GraphicsPoint;



namespace DebugHelper
{
    class CharGenerator
    {
    public:
        CharGenerator()
            : m_i( 0 )
        {}

        char GetNext() { return 'A' + ( m_i++ ); }
    private:
        int m_i;
    };

    class ConstructionInfoWriter
    {
    public:
        ConstructionInfoWriter();

        void WriteToFile( GraphicsPoint * );

        //~ConstructionInfoWriter();
    private:
        QString m_fileName;
        QFile m_file;

        CharGenerator m_chg;
    };
}
