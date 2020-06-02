#pragma once

#include <QString>



class Exception
{
public:
	Exception( QString const & message )
		: m_message( message )
	{}

	QString const & GetMessage() const { return m_message; }
private:
	QString const m_message;
};

