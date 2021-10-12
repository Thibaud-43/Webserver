#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

/*Request::Request(): m_client(Client())
{

}*/

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

void			Request::_parseRequestLine(void)
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

void			Request::_parseLine(std::string & token)
{
	std::string delimiter = ": ";
	m_header[m_buffer.substr(0, m_buffer.find(delimiter))] = m_buffer.substr(m_buffer.find(delimiter), token.size());
}

void			Request::_parseRequestHeaders(void)
{
	std::string delimiter = "\r\n";
	std::string s = m_buffer.substr(0, m_buffer.find(delimiter));
	
	size_t pos = 0;
	std::string	token;
	while ((pos = s.find(delimiter)) != std::string::npos) 
	{
		token = s.substr(0, pos);
		_parseLine(token);
		s.erase(0, pos + delimiter.length());
	}
}

void			Request::_printHeader(void)
{
	std::cout << std::endl << std::endl << "MAP REQUEST HEADER" << std::endl;
	for (std::map<std::string, std::string>::iterator it = m_header.begin(); it != m_header.end(); it++)
	{
		std::cout << "[" << it->first << "] = " << it->second << std::endl; 
	}
	
}

void			Request::parse(void)
{
	_parseRequestLine();
	_parseRequestHeaders();
	_printHeader();
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