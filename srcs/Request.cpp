#include "Request.hpp"
#include "Delete.hpp"
#include "Get.hpp"
#include "Post.hpp"

bool	Request::_decrement(size_t const & n)
{
	if (m_header.find("Content-Length") == m_header.end())
		return true;
	size_t   s_str;
    std::istringstream(m_header["Content-Length"]) >> s_str;
	s_str -= n;
	if (s_str < 0)
		return false;
	std::stringstream ss;
	ss << s_str;
	m_header["Content-Length"] = ss.str();
	return true;
}

size_t	Request::_strToSize(std::string const & str)
{
	size_t   s_str;
    std::istringstream( str ) >> s_str;
	return s_str;
}
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request()
: Client(),  m_location(NULL), m_path(std::string()), m_header(header_type())
{
}

Request::Request(const Request & src)
: Client(src), m_location(src.m_location), m_path(src.m_path), m_header(src.m_header)
{
}

Request::Request(const Client & src)
: Client(src), m_location(NULL), m_path(std::string()), m_header(header_type())
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

void			Request::_linkPath(void)
{
	std::string	uri = m_header["uri"];

	if (uri == m_location->getUri())
		m_path.setPath(m_location->getRoot());
	else
	{
		uri.erase(uri.begin(), uri.begin() + m_location->getUri().size());	
		m_path.setPath(m_location->getRoot() + uri);
	}
}

void			Request::_linkLocation(void)
{
	std::string	delimiter = "?";
	std::string	token;
	size_t		pos = m_header["uri"].find(delimiter);
	m_header["query_string"] = "";
	if (pos != std::string::npos)
	{
		token = m_header["uri"].substr(0, pos);
		m_header["query_string"] = m_header["uri"].substr(pos + delimiter.length(), m_header["uri"].length());
		m_header["uri"] = token;

	}
	m_location = m_server->getLocation(m_header["uri"]);
	
}

void			Request::_bufferToRequestLine(void)
{
	std::string delimiter1 = "\r\n";
	std::string delimiter2 = " ";
	std::string s = m_buff.substr(0, m_buff.find(delimiter1));
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
	m_buff.erase(0, m_buff.find(delimiter1) + delimiter1.length());
}	

void			Request::_bufferToHeaderLine(std::string & token)
{
	std::string delimiter = ": ";
	size_t		pos = token.find(delimiter);
	m_header.insert(std::pair<std::string, std::string>(token.substr(0, pos), token.substr(pos + delimiter.length(), token.length() - 1)));
}

void			Request::_bufferToHeader(void)
{
	std::string delimiter = "\r\n";
	std::string delimiter2 = "\r\n\r\n";
	size_t pos = 0;
	std::string	token;

	if (m_buff == delimiter)
	{
		m_buff = "";
		return ;
	}
	while ((pos = m_buff.find(delimiter)) != std::string::npos && pos != m_buff.find(delimiter2)) 
	{
		token = m_buff.substr(0, pos);
		_bufferToHeaderLine(token);
		m_buff.erase(0, pos + delimiter.length());
	}
	if (m_buff.find(delimiter2) != std::string::npos)
	{
		token = m_buff.substr(0, pos);
		_bufferToHeaderLine(token);
		m_buff.erase(0, pos + delimiter2.length());
		return ;
	}
	else
	{
		return ;
	}
}

void			Request::_printHeader(void)
{
	std::cout << std::endl << std::endl << "MAP REQUEST HEADER" << std::endl;
	for (std::map<std::string, std::string>::iterator it = m_header.begin(); it != m_header.end(); it++)
	{
		std::cout << "[" << it->first << "]='" << it->second << "'" << std::endl << std::endl; 
	}
}

bool	Request::_checkHeader(void)
{
	if (_checkRequestLine() == false || _checkHost() == false)
		return (false);
	_linkLocation();
	_linkPath();
	if (!m_location->isAllowed(m_header["method"]))
	{
		_send(Response::create_error("405", &m_server->getParams()));
		return (false);
	}
	if (!_decrement(m_buff.size()))
	{
		_send(Response::create_error("413", &m_server->getParams()));
		return (false);		
	}
	return (true);
}

bool	Request::_checkHost(void)
{
	if (m_header["Host"].empty())
	{
		_send(Response::create_error("400", NULL));
		return (false);
	}
	return (true);
}

bool	Request::_checkRequestLine(void)
{
	if (m_header.empty() || m_header["protocol"].empty() || m_header["method"].empty() || m_header["uri"].empty())
	{
		_send(Response::create_error("400", NULL));
		return (false);
	}
	if (m_header["protocol"] != PROTOCOL)
	{
		_send(Response::create_error("505", NULL));
		return (false);
	}
	if (m_header["method"] != "GET" && m_header["method"] != "POST" && m_header["method"] != "DELETE")
	{
		_send(Response::create_error("400", NULL));
		return (false);
	}
	return (true);
}

bool	Request::execute(ASocket **ptr)
{
	std::string		method = "";
	if (!_fillBuffer())
	{
		return false;
	}
	if (m_buff.find("\r\n\r\n") == std::string::npos)
	{
		return true;
	}
	_bufferToRequestLine();
	_bufferToHeader();
	_printHeader();
	if(!_checkHeader())
		return false;
	method = m_header["method"];
	if (method == "GET")
		_convert<Get>(ptr);
	else if (method == "POST")
		_convert<Post>(ptr);
	else if (method == "DELETE")
		_convert<Delete>(ptr);
	return (*ptr)->execute(ptr);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */

