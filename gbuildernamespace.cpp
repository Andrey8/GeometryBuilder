#include "gbuildernamespace.h"



GB::MainWindowGeometryData GB::mainWindowGeometryData;



// class MainWindowGeometryData :

GB::MainWindowGeometryData::MainWindowGeometryData()
{}



void GB::MainWindowGeometryData::SetMainWindowRect( QRect const & rect )
{
	m_mainWindowRect = rect;
}

void GB::MainWindowGeometryData::SetPlaneViewSize( QSize const & size )
{
	m_planeViewSize = size;
}

// end









