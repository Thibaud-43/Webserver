#include "Request.hpp"

Request::list_type Request::_list = Request::list_type();

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

/*Request::Request(): m_client(Client())
{

}*/

Request::Request(Client const * client): m_client(client), m_headerCompleted(false)
{
	m_server = NULL;
}

Request::Request( const Request & src ): m_header(src.m_header), m_body(src.m_body), m_client(src.m_client), m_server(src.m_server)
{

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{

	/*for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (m_client == (*it).getClient())
		{
			_list.erase(it);
			return ;
		}
	}*/
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

void			Request::_parseRequestLine(std::string & buffer)
{
	std::string delimiter1 = "\r\n";
	std::string delimiter2 = " ";

	std::string s = buffer.substr(0, buffer.find(delimiter1));
	size_t	count = 0;
	size_t pos = 0;
	std::string	token;
	while ((pos = s.find(delimiter2)) != std::string::npos) 
	{

		token = s.substr(0, pos);
		if (count == 0)
		{
			m_header.insert(std::pair<std::string, std::string>("method", token));
		}
		else if(count == 1)
		{
			m_header.insert(std::pair<std::string, std::string>("uri", token));
		}
		count++;
		token.clear();
		s.erase(0, pos + delimiter2.length());
	}
	m_header.insert(std::pair<std::string, std::string>("protocol", s));
	buffer.erase(0, buffer.find(delimiter1) + delimiter1.length());
}		

void			Request::_parseLine(std::string & token)
{
	std::string delimiter = ": ";
	size_t		pos = token.find(delimiter);
	m_header.insert(std::pair<std::string, std::string>(token.substr(0, pos), token.substr(pos + delimiter.length(), token.length() - 1)));
}

void			Request::_parseHeaders(std::string & buffer)
{
	std::string delimiter = "\r\n";
	std::string delimiter2 = "\r\n\r\n";
	size_t pos = 0;
	std::string	token;
    if (buffer.empty())
    {
        return ;
    }
	while ((pos = buffer.find(delimiter)) != std::string::npos && pos != buffer.find(delimiter2)) 
	{
		token = buffer.substr(0, pos);
		_parseLine(token);
		buffer.erase(0, pos + delimiter.length());
	}
	token = buffer.substr(0, pos);
	_parseLine(token);
	if (buffer.find(delimiter2) != std::string::npos)
	{
		token = buffer.substr(0, pos);
		_parseLine(token);
		buffer.erase(0, pos + delimiter2.length());
		m_headerCompleted = true;
	}
	else
	{
		token = buffer.substr(0, pos);
		_parseLine(token);
		buffer.erase(0, pos + delimiter.length());
	}
}

void			Request::_parseBody(std::string & buffer)
{
	m_body = buffer;
}

void			Request::_printHeader(void)
{
	std::cout << std::endl << std::endl << "MAP REQUEST HEADER" << std::endl;
	for (std::map<std::string, std::string>::iterator it = m_header.begin(); it != m_header.end(); it++)
	{
		std::cout << "[" << it->first << "]='" << it->second << "'" << std::endl; 
	}
	std::cout << "List size:" << _list.size() << std::endl;
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

void			Request::parse(std::string & buffer)
{
	std::cout << buffer << std::endl;
	if (m_header.empty())
		_parseRequestLine(buffer);
	if (m_headerCompleted == false)
		_parseHeaders(buffer);
	if (m_headerCompleted == true)
		_parseBody(buffer);
	_printHeader();
	_printBody();

}

void	Request::execute(void)
{
	if (m_header.empty())
		return (Response::send_error("400", m_client, m_server->getParams()));
	if (m_header["protocol"] != PROTOCOL)
		return (Response::send_error("505", m_client, m_server->getParams()));
	Response::send_error("200", m_client, m_server->getParams());
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

	Client const *	Request::getClient(void) const
	{
		return m_client;
	}

	Request *	Request::getRequestFromClient(Client const & client)
	{
		for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
		{
			if (&client == (*it).getClient())
			{
				return (&(*it));
			}
		}
		_list.push_front(Request(&client));
		return &(*(_list.begin()));
	}
	void		Request::removeRequestFromClient(Client const & client)
	{
		for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
		{
			if (&client == (*it).getClient())
			{
				_list.erase(it);
				return ;
			}
		}
	}
	void		Request::removeRequestFromRequest(Request const & request)
	{
		for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
		{
			if (request.getClient() == (*it).getClient())
			{
				_list.erase(it);
				return ;
			}
		}
	}
/* ************************************************************************** */