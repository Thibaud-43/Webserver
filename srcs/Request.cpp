#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(Client client = Client()): m_client(client)
{

}

Request::Request(std::string buffer, Client & client): m_buffer(buffer), m_client(client)
{

}

/*Request::Request( const Request & src )
{
}*/


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Request &				Request::operator=( Request const & rhs )
{
	if ( this != &rhs )
	{
		m_header = rhs.m_header;
		m_body = rhs.m_body;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Request const & i )
{
	(void)o;
	(void)i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void			Request::_parseFirstLine(void)
{
	std::string delimiter1 = "\r\n";
	std::string delimiter2 = " ";
	std::string s = m_buffer.substr(0, m_buffer.find(delimiter1));
	
	size_t	count = 0;
	size_t pos = 0;
	std::string	token;
	while ((pos = s.find(delimiter2)) != std::string::npos) 
	{
		token = s.substr(0, pos);
		if (count == 0)
		{
			m_header["method"] = token;
		}
		else if(count == 1)
		{
			m_header["uri"] = token;
		}
		count++;
		s.erase(0, pos + delimiter2.length());
	}
	m_header["protocol"] = s;

}		


void			Request::parse(void)
{
	_parseFirstLine();

}

std::string		Request::execute(void) const
{
	std::string response = "HTTP/1.1 200 OK\r\n\r\nHello";

	m_client.sendResponse(response.data());
	return "";
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */