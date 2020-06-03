#pragma once

#include <QMessageBox>
#include <QString>



namespace DebugHelper
{
	QString const title = "Debug Helper" ;

	void ShowMessageBoxIfTrue( bool expressionToCheck );

    void ShowMessageBox();
	void ShowMessageBox( QString const & text );
	void ShowTextInInput( QString const & text );
}
