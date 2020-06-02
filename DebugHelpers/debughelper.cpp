#include "debughelper.h"

#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>



void DebugHelper::ShowMessageBoxIfTrue( bool expression )
{
	if ( expression )
    {
        QMessageBox * box = new QMessageBox;
        box->show();
    }
}

void DebugHelper::ShowMessageBox()
{
	QMessageBox * message = new QMessageBox;
    message->show();
}

void DebugHelper::ShowMessageBox( QString const & text )
{
	QMessageBox * message = new QMessageBox( QMessageBox::Information, title, text );
    message->show();
}

void DebugHelper::ShowTextInInput( QString const & text )
{
    qDebug() << text ;
}














