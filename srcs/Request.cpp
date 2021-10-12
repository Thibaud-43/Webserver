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
	m_buffer.erase(0, m_buffer.find(delimiter1) + delimiter1.length());

}		

void			Request::_parseLine(std::string & token)
{
	std::string delimiter = ": ";
	size_t		pos = token.find(delimiter);
	m_header[token.substr(0, pos)] = token.substr(pos + delimiter.length(), token.length() - 1);
}

void			Request::_parseHeaders(void)
{
	std::string delimiter = "\r\n";
	std::string delimiter2 = "\r\n\r\n";
	size_t pos = 0;
	std::string	token;
	while ((pos = m_buffer.find(delimiter)) != std::string::npos && pos != m_buffer.find(delimiter2)) 
	{
		token = m_buffer.substr(0, pos);
		_parseLine(token);
		m_buffer.erase(0, pos + delimiter.length());
	}
	token = m_buffer.substr(0, pos);
	_parseLine(token);
	m_buffer.erase(0, pos + delimiter2.length());
}

void			Request::_parseBody(void)
{
	m_body = m_buffer;
}

void			Request::_printHeader(void)
{
	std::cout << std::endl << std::endl << "MAP REQUEST HEADER" << std::endl;
	for (std::map<std::string, std::string>::iterator it = m_header.begin(); it != m_header.end(); it++)
	{
		std::cout << "[" << it->first << "]='" << it->second << "'" << std::endl; 
	}
}

void			Request::_printBody(void)
{
	std::cout << std::endl << std::endl << "MAP REQUEST BODY" << std::endl << m_body << std::endl;

}

void			Request::_printHex(std::string & token)
{
	std::cout << std::endl << std::endl << "HEX: " << token << std::endl;
	for (std::string::iterator it = token.begin(); it != token.end(); it++)
	{
		std::cout << int(*it) << std::endl; 
	}
}

void			Request::parse(void)
{
	_parseRequestLine();
	_parseHeaders();
	_parseBody();
	_printHeader();
	_printBody();
}

void	Request::execute(void)
{
	
}

void			Request::linkServer(std::vector<Server> & list)
{
	std::string	delimiter = ":";
	size_t		pos;
	std::string	server_name;
	std::string	port;

	pos = m_header["Host"].find(delimiter);
	if (pos > m_header["Host"].length())
	{
		server_name = m_header["Host"];
		port = "80";
	}
	else
	{
		server_name = m_header["Host"].substr(0, pos);
		port = m_header["Host"].substr(pos + delimiter.length(), m_header["Host"].length());
	}

	std::vector<std::string>::iterator it2;
	for (std::vector<Server>::iterator it = list.begin(); it != list.end(); it++)
	{
		it2 = std::find((*it).getNames().begin(), (*it).getNames().end(), server_name);
		if (port == (*it).getPort() && it2 != (*it).getNames().end())
		{
			m_server = &(*it);
			return ;
		}
	}
	m_server = NULL;
	return ;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */