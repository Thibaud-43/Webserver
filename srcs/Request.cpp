#include "Request.hpp"

Request::list_type Request::_list = Request::list_type();

Request *	Request::getRequestFromClient(Client const & client)
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (&client == (*it).getClient())
			return (&(*it));
	}
	return (NULL);
}

Request *	Request::createRequest(Client const & client)
{
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

void		Request::unChunked(std::string & str)
{
	std::string		delimiter = "\r\n";
	std::string		chunkSizeStr = str.substr(0, str.find(delimiter));
	std::string		newStr = "";
	int				chunkSizeInt = strtol(chunkSizeStr.c_str(), NULL, 16);
	int				i = 0;

	while (chunkSizeInt)
	{
		i = str.find(delimiter, i) + delimiter.length();
		newStr += str.substr(i, chunkSizeInt);
		i += chunkSizeInt + delimiter.length();
		chunkSizeStr = str.substr(i, str.find(delimiter));
		chunkSizeInt = strtol(chunkSizeStr.c_str(), NULL, 16);
	}
	str = newStr;
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

bool		Request::_checkBufferCharacters(std::string & str)
{
	for (std::string ::iterator it = str.begin(); it != str.end(); it++)
	{
		if (isprint(int(*it)))
			return (true);
	}
	return (false);
	
}

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
		if (buffer != "\r\n" && _checkBufferCharacters(buffer) == false)
		{
			Response::send_error("400", m_client);
			return (true);
		}
		if (m_header.empty())
			_bufferToRequestLine(buffer);	
		_bufferToHeader(buffer);
		if (m_headerCompleted == true)
		{
			if (!_checkHeader(servers))
				return (true);
		}
	}
	if (m_headerCompleted == true)
	{
		_bufferToBody(buffer);
		_printBody();
		if (m_header.find("Transfer-Encoding") != m_header.end() && m_header["Transfer-Encoding"] == "chunked")
			unChunked(m_body);
		_printHeader();
		_printBody();
		if (!_execute())
			return (true);
		return _checkRequestAdvancement();
	}
	return (false);
}

bool	Request::_checkHeader(std::vector<Server> const & servers)
{
	if (_checkRequestLine() == false || _checkHost() == false)
		return (false);
	_linkServer(servers);
	_linkLocation();
	_linkPath();
	if (!m_location->isAllowed(m_header["method"]))
	{
		Response::send_error("400", m_client, &m_server->getParams());
		return (false);
	}
	return (true);
}


bool	Request::_checkHost(void)
{
	if (m_header["Host"].empty())
	{
		Response::send_error("400", m_client);
		return (false);
	}
	return (true);
}
bool	Request::_checkRequestLine(void)
{
	if (m_header.empty() || m_header["protocol"].empty() || m_header["method"].empty() || m_header["uri"].empty())
	{
		Response::send_error("400", m_client);
		return (false);
	}
	if (m_header["protocol"] != PROTOCOL)
	{
		Response::send_error("505", m_client);
		return (false);
	}
	if (m_header["method"] != "GET" && m_header["method"] != "POST" && m_header["method"] != "DELETE")
	{
		Response::send_error("400", m_client);
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
	m_server = &(*list.begin());
	return ;
}

bool	Request::_execute(void) const
{
	std::string const & method = m_header.at("method");

	if (method == "GET")
	{
		if (!_check_get())
			return (false);
		if (_is_cgi())
			return (_get_cgi());
		return (_get());
	}
	else if (method == "DELETE")
	{
		// VERIFS
		// DELETE
	}
	else if (method == "POST")
	{
		// VERIFS
		// POST
	}
	else
		Response::send_error("501", m_client, m_location);
	return (false);
}

bool	Request::_check_get(void) const
{
	File const	file(m_path);

	if (!file.exist())
	{
		Response::send_error("404", m_client, m_location);
		return (false);
	}
	else if (!file.is_readable())
	{
		Response::send_error("403", m_client, m_location);
		return (false);
	}
	else if (m_body.size() > m_location->getBodySize())
	{
		Response::send_error("413", m_client, m_location);
		return (false);
	}
	else if (m_header.find("Range") != m_header.end())
	{
		// RANGE NOT SUPPORTED
		Response::send_error("416", m_client, m_location);
		return (false);
	}
	else if (file.is_directory())
	{
		std::cout << m_path << std::endl;
		if (*(m_path.end() - 1) != '/')
			Response::redirect("302", m_path + "/" , m_client);
		else if (m_location->autoindex())
			Response::send_index(m_path, m_client, m_location);
		else
			Response::send_error("403", m_client, m_location);
		return (false);
	}
	return (true);
}

bool	Request::_get(void) const
{
	Response		rep;
	std::ifstream	fstream(m_path);
	char			buff[MAX_SERVER_BODY_SIZE];

	if (!fstream.is_open())
		return (Response::send_error("500", m_client, m_location));
	rep.start_header("200");
	while (fstream.readsome(buff, MAX_SERVER_BODY_SIZE - 1))
	{
		buff[MAX_SERVER_BODY_SIZE - 1] = 0;
		rep.append_to_body(buff);
	}
	rep.send_to_client(m_client);
	return (false);
}

bool	Request::_is_cgi(void) const
{
	return (false);
}

bool	Request::_get_cgi(void) const
{
	return (false);
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