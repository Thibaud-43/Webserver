#include "Request.hpp"

Request::list_type Request::_list = Request::list_type();

Request *	Request::getRequestFromClient(Client const & client)
{
	if (_list.empty())
	{
		_list.push_front(Request(&client));
		return &(*(_list.begin()));
	}
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

void		Request::removeRequest(Request const & request)
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


bool							Request::_checkBodySize(void)
{
	std::stringstream sstream(this->getHeader()["Content-Length"]);
	size_t lenght;
	sstream >> lenght;
	if (this->getBody().size() == lenght)
	{
		std::cout << " REMOVE LIST --> DETECT RECEIVE FULL BODY" << std::endl;
		return (true);
	}
	return (false);
}
bool							Request::_checkChunkAdvancement(void)
{
	std::string			s = this->getBody();
	std::string 		delimiter = "\r\n";
	std::string 		token;
	size_t				pos;
	while((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		s.erase(0, pos + delimiter.length());
		if (token == "0")
		{
			std::cout << " REMOVE LIST --> DETECT CHUNK END " << std::endl;
			return (true);
		}
	}
	return (false);
}

bool							Request::_checkRequestAdvancement(void)
{
	header_type::iterator		contentLenght = this->getHeader().find("Content-Length");
	header_type::iterator		transfertEncoding = this->getHeader().find("Transfer-Encoding");

	if (!this->getHeaderCompleted())
	{
		return false;
	}
	else if (contentLenght == this->getHeader().end() && transfertEncoding == this->getHeader().end())
	{
		std::cout << "REMOVE LIST --> NO BODY TO RECEIVE" << std::endl;
		return true;
	}
	else if (contentLenght != this->getHeader().end() && transfertEncoding == this->getHeader().end())
	{
		return _checkBodySize();
	}
	else if (contentLenght == this->getHeader().end() && transfertEncoding != this->getHeader().end())
	{
		return _checkChunkAdvancement();;
	}
	else
	{
		return false;
	}
}
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

Request::Request( const Request & src ): m_header(src.m_header), m_body(src.m_body), m_client(src.m_client), m_server(src.m_server), m_headerCompleted(src.m_headerCompleted)
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

void			Request::_bufferToRequestLine(std::string & buffer)
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

void			Request::_bufferToHeaderLine(std::string & token)
{
	std::string delimiter = ": ";
	size_t		pos = token.find(delimiter);
	m_header.insert(std::pair<std::string, std::string>(token.substr(0, pos), token.substr(pos + delimiter.length(), token.length() - 1)));
}

void			Request::_bufferToHeader(std::string & buffer)
{
	std::string delimiter = "\r\n";
	std::string delimiter2 = "\r\n\r\n";
	size_t pos = 0;
	std::string	token;

	if (m_header.empty())
		_bufferToRequestLine(buffer);
	if (buffer == delimiter)
	{
		buffer = "";
		m_headerCompleted = true;
		return ;
	}

	while ((pos = buffer.find(delimiter)) != std::string::npos && pos != buffer.find(delimiter2)) 
	{
		token = buffer.substr(0, pos);
		_bufferToHeaderLine(token);
		buffer.erase(0, pos + delimiter.length());
	}
	if (buffer.find(delimiter2) != std::string::npos)
	{
		token = buffer.substr(0, pos);
		_bufferToHeaderLine(token);
		buffer.erase(0, pos + delimiter2.length());
		m_headerCompleted = true;
	}
	else
	{
		m_headerCompleted = false;
	}
}

void			Request::_bufferToBody(std::string & buffer)
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


bool	Request::manage(std::string & buffer, std::vector<Server> const & servers)
{
	if (m_headerCompleted == false)
	{
		_bufferToHeader(buffer);
		if (m_headerCompleted == true)
		{
			_linkServer(servers);
			_linkLocation();
			_linkPath();
			if (!_parseHeader())
				return (true);
		}
	}
	if (m_headerCompleted == true)
	{
		_bufferToBody(buffer);
		_printHeader();
		_printBody();
		if (!execute())
			return (true);
		return _checkRequestAdvancement();
	}
	return (false);
}

bool	Request::_parseHeader(void)
{
	if (m_header.empty())
	{
		Response::send_error("400", m_client, m_server->getParams());
		return (false);
	}
	if (m_header["protocol"] != PROTOCOL)
	{
		Response::send_error("505", m_client, m_server->getParams());
		return (false);
	}
	if (!m_location->isAllowed(m_header["method"]))
	{
		Response::send_error("405", m_client, m_server->getParams());
		return (false);
	}
	return (true);
}
void			Request::_linkLocation(void)
{
	m_location = m_server->getLocation(m_header["uri"]);
}

void			Request::_linkPath(void)
{
	m_path = m_header["uri"];
	m_path.replace(0, m_location->getUri().size(), m_location->getRoot());
}

void			Request::_linkServer(std::vector<Server> const & list)
{
	std::string	delimiter = ":";
	size_t		pos;
	std::string	server_name;
	std::string	port;

	if (m_header.find("Host") == m_header.end())
		return ;
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

	std::vector<std::string>::const_iterator it2;
	for (std::vector<Server>::const_iterator it = list.begin(); it != list.end(); it++)
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

bool	Request::execute(void)
{
	Response::send_error("200", m_client, m_server->getParams());

	// METHOD DISPATCH

	return (true);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

Client const *	Request::getClient(void) const
{
	return m_client;
}

Request::header_type  &		Request::getHeader(void) 
{
	return m_header;
}

Request::body_type  &		Request::getBody(void) 
{
	return m_body;
}

bool				Request::getHeaderCompleted(void)
{
	return m_headerCompleted;
}

/* ************************************************************************** */