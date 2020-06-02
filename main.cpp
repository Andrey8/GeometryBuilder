#include "Widgets/mainwindow.h"
#include "polygonalgorithms.h"

#include <QApplication>
//#include <QDesktopWidget>
//#include <QRect>
#include <QDebug>



int main( int argc, char * argv[] )
{
	//PolygonAlgorithms::Tests();
	//Math::Tests();




	QApplication app( argc, argv );

	MainWindow mw;
	mw.show();

	return app.exec();
}
