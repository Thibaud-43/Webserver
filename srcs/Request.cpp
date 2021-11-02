#include "Request.hpp"
#include "Cgi.hpp"

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


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request()
: m_client(NULL), m_server(NULL), m_location(NULL), m_status(BEGIN)
{
}

Request::Request(Client const * client)
: m_client(client), m_server(NULL), m_location(NULL), m_status(BEGIN)
{
}

Request::Request( const Request & src )
	: m_header(src.m_header), m_body(src.m_body), m_client(src.m_client), m_server(src.m_server)
	, m_location(src.m_location), m_path(src.m_path)
	, m_status(src.m_status)
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

Request &				Request::operator=( Request const & rhs )
{
	if ( this != &rhs )
	{
		m_header = rhs.m_header;
		m_body = rhs.m_body;
		m_client = rhs.m_client;
		m_server = rhs.m_server;
		m_location = rhs.m_location;
		m_path = rhs.m_path;
		m_status = rhs.m_status;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


bool			Request::_unChunked(std::string & str)
{
	std::string		delimiter = "\r\n";
	std::string		chunkSizeStr = str.substr(0, str.find(delimiter));
	std::string		newStr = "";
	int				chunkSizeInt = strtol(chunkSizeStr.c_str(), NULL, 16);
	int				i = 0;
	size_t			totalSize = chunkSizeInt;

	while (chunkSizeInt)
	{
		i = str.find(delimiter, i) + delimiter.length();
		newStr += str.substr(i, chunkSizeInt);
		i += chunkSizeInt + delimiter.length();
		chunkSizeStr = str.substr(i, str.find(delimiter));
		chunkSizeInt = strtol(chunkSizeStr.c_str(), NULL, 16);
		totalSize += chunkSizeInt;
	}
	if (newStr.length() != totalSize)
	{
		return (false);
	}
	str = newStr;
	std::stringstream	sstream;

	sstream << totalSize;
	m_header["Content-Length"] = sstream.str();
	return (true);
}

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
	m_status = REQUEST_LINE_AND_HOST_COMPLETED;
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
		m_status = HEADER_COMPLETED;
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
		m_status = HEADER_COMPLETED;
	}
	else
	{
		//m_status = HEADER_COMPLETED;
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
bool	Request::manage(std::string & buffer, std::vector<Server*> const & servers)
{

	if (m_status == BEGIN || m_status == REQUEST_LINE_AND_HOST_COMPLETED)
	{
		if (buffer != "\r\n" && _checkBufferCharacters(buffer) == false)
		{
			Response::send_error("400", m_client);
			return (false);
		}
		if (m_status == BEGIN)
			_bufferToRequestLine(buffer);
		_bufferToHeader(buffer);
	}
	if (m_status == HEADER_COMPLETED)
	{
		_printHeader();
		if (!_checkHeader(servers))
		{
			return (false);
		}
		if (m_header.at("method") == "POST" && m_location->getUpload())
			m_status = START_UPLOAD;
		else
			m_status = GET_BODY;
	}
	if (m_status == GET_BODY || m_status == START_UPLOAD || m_status == UPLOADING)
	{
		_bufferToBody(buffer);
		_checkRequestAdvancement();
		if (m_header.find("Transfer-Encoding") != m_header.end() && m_header["Transfer-Encoding"] == "chunked")
		{
			if (!_unChunked(m_body))
			{
				Response::send_error("400", m_client, m_location);
				return (false);
			}
		}
		_printBody();
		if (m_status == START_UPLOAD || m_status == UPLOADING)
		{
			_upload();
			m_status = UPLOADING;
		}
		else if (m_status == BODY_COMPLETED)
		{
			_execute();
			return false;
		}
	}
	return true;
}

bool	Request::_checkHeader(std::vector<Server*> const & servers)
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

void			Request::_linkPath(void)
{
	std::string	uri = m_header["uri"];

	if (uri == m_location->getUri())
		m_path = m_location->getRoot();
	else
	{
		uri.erase(uri.begin(), uri.begin() + m_location->getUri().size());	
		m_path = m_location->getRoot() + uri;
	}
}

void			Request::_linkServer(std::vector<Server*> const & list)
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
	for (std::vector<Server*>::const_iterator it = list.begin(); it != list.end(); it++)
	{
		it2 = std::find((*it)->getNames().begin(), (*it)->getNames().end(), server_name);
		if (port == (*it)->getPort() && it2 != (*it)->getNames().end())
		{
			m_server = (*it);
			return ;
		}
	}
	m_server = (*list.begin());
	return ;
}

bool	Request::_upload(void) const
{
	std::fstream	stream;

	if (m_status == START_UPLOAD)
		stream.open(m_path.data(), std::fstream::out | std::fstream::trunc);
	else
		stream.open(m_path.data(), std::fstream::out | std::fstream::app);
	if (!stream.is_open())
	{
		Response::send_error("403", m_client, m_location);
		return (false);
	}
	stream.write(m_path.data(), m_path.size());
	stream.close();
	return (true);
}

void	Request::_execute(void) const
{
	std::string const & method = m_header.at("method");

	if (method == "GET")
	{
		if (!_check_get())
			return ;
		Location::file_t const * cgi_path = _get_cgi_path();
		if (cgi_path)
		{
			File	f(*cgi_path);
			if (f.is_executable())
				return (_execute_cgi(*cgi_path));
			else
				return (Response::send_error("500", m_client, m_location));
		}
		else
			_get();
	}
	else if (method == "DELETE")
	{
		if (!_check_delete())
			return ;
		_delete();
	}
	else if (method == "POST")
	{
		if (!_check_post())
			return ;
		Location::file_t const * cgi_path = _get_cgi_path();
		if (cgi_path)
		{
			File	f(*cgi_path);
			if (f.is_executable())
				return (_execute_cgi(*cgi_path));
			else
				return (Response::send_error("500", m_client, m_location));
		}
		else
			_get();
	}
	else
		Response::send_error("501", m_client, m_location);
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
		if (*(m_path.end() - 1) != '/')
			Response::redirect("302", m_header.at("uri") + "/" , m_client);
		else if (m_location->getAutoindex())
			Response::send_index(m_path, m_header.at("uri"), m_client, m_location);
		else
			Response::send_error("403", m_client, m_location);
		return (false);
	}
	return (true);
}

bool	Request::_check_post(void) const
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
	else if (m_header.find("Transfer-Encoding") != m_header.end() && m_body.size() > m_location->getBodySize())
	{
		Response::send_error("413", m_client, m_location);
		return (false);
	}
	else if (file.is_directory())
	{
		if (*(m_path.end() - 1) != '/')
			Response::redirect("302", m_header.at("uri") + "/" , m_client);
		else if (m_location->getAutoindex())
			Response::send_index(m_path, m_header.at("uri"), m_client, m_location);
		else
			Response::send_error("403", m_client, m_location);
		return (false);
	}
	return (true);
}

bool	Request::_check_delete(void) const
{
	File const	file(m_path);

	if (!file.exist())
	{
		Response::send_error("404", m_client, m_location);
		return (false);
	}
	else if (!file.is_writable())
	{
		Response::send_error("403", m_client, m_location);
		return (false);
	}
	return (true);
}

void	Request::_delete(void) const
{
	Response	rep;

	if (remove(m_path.data()))
		Response::send_error("500", m_client, m_location);
	else
	{
		rep.start_header("200");
		rep.append_to_header("Connection: keep-alive");
		rep.append_to_body("<html>\n<body>\n<h1>File deleted.</h1>\n</body>\n<html>\n");
		rep.add_content_length();
		rep.send_to_client(m_client);
	}
}

void	Request::_get() const
{
	Response		rep;
	std::ifstream	fstream(m_path.data());
	char			buff[MAX_SERVER_BODY_SIZE + 1];
	File			f(m_path);

	if (!fstream.is_open())
	{
		Response::send_error("500", m_client, m_location);
		return ;
	}
	rep.start_header("200");
	rep.append_to_header("Connection: keep-alive");
	try
	{
		if (f.size() > MAX_SERVER_BODY_SIZE)
		{
			std::string		body;

			rep.append_to_header("Transfer-Encoding: chunked");
			rep.send_to_client(m_client);
			fstream.read(buff, MAX_SERVER_BODY_SIZE);
			while (fstream.gcount())
			{
				_chunk_size_to_client(fstream.gcount());
				buff[fstream.gcount()] = 0;
				body = buff;
				body += "\r\n";
				m_client->sendResponse(body.data());
				fstream.read(buff, MAX_SERVER_BODY_SIZE - 1);
			}
			m_client->sendResponse("0\r\n\r\n");
		}
		else
		{
			fstream.read(buff, MAX_SERVER_BODY_SIZE);
			while (fstream.gcount())
			{
				buff[fstream.gcount()] = 0;
				rep.append_to_body(buff);
				fstream.read(buff, MAX_SERVER_BODY_SIZE);
			}
			rep.send_to_client(m_client);
		}
	}
	catch(const std::exception& e)
	{
		return (Response::send_error("500", m_client, m_location));
	}
	fstream.close();
}

void	Request::_chunk_size_to_client(std::streamsize const & s) const
{
	std::stringstream stream;

	stream << std::hex << s << "\r\n";
	m_client->sendResponse(stream.str().data());
}

Location::file_t const *	Request::_get_cgi_path(void) const
{
	Location::cgi_t	const & cgi = m_location->getCGIPass();

	for (Location::cgi_t::const_iterator it = cgi.begin() ; it != cgi.end(); it++)
	{
		if (it->first.size() > m_path.size())
			break ;
		else if (m_path.find(it->first, m_path.size() - it->first.size()) != std::string::npos)
			return (&(it->second));
	}
	return (NULL);
}

void	Request::_execute_cgi(Location::file_t const & cgi_path) const
{
	Cgi				cgi(*this, cgi_path);
	char 			**argv = new char*[3];
	
	argv[0] = new char[cgi_path.size() + 1];
	cgi_path.copy(argv[0], cgi_path.size());
	argv[0][cgi_path.size()] = 0;
	argv[1] = new char[m_path.size() + 1];
	m_path.copy(argv[1], m_path.size());
	argv[1][m_path.size()] = 0;
	argv[2] = 0;
	if (!cgi.run(argv))
	{
		cgi.del_env(argv);
		Cgi::removeCgi(cgi);
		return (Response::send_error("500", m_client, m_location));
	}
	cgi.del_env(argv);
	Cgi::addCgi(cgi);
}


void							Request::_checkBodySize(void)
{
	std::stringstream sstream(this->getHeader().at("Content-Length"));
	size_t lenght;
	sstream >> lenght;
	if (this->getBody().size() - 2 == lenght)
	{
		m_status = BODY_COMPLETED;
	}
	return;
}

void							Request::_checkChunkAdvancement(void) 
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
			m_status = BODY_COMPLETED;
		}
	}
	return ;
}

void							Request::_checkRequestAdvancement(void)
{
	header_type::const_iterator		contentLenght = this->getHeader().find("Content-Length");
	header_type::const_iterator		transferEncoding = this->getHeader().find("Transfer-Encoding");

	if (contentLenght == this->getHeader().end() && transferEncoding == this->getHeader().end())
	{
		m_status = BODY_COMPLETED;
	}
	else if (contentLenght != this->getHeader().end() && transferEncoding == this->getHeader().end())
	{
		_checkBodySize();
	}
	else if (contentLenght == this->getHeader().end() && transferEncoding != this->getHeader().end())
	{
		_checkChunkAdvancement();;
	}
	return;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

Client const *	Request::getClient(void) const
{
	return m_client;
}

Request::header_type const &	Request::getHeader(void) const
{
	return m_header;
}

Request::body_type const &		Request::getBody(void) const
{
	return m_body;
}

bool				Request::getStatus(void) const
{
	return m_status;
}

std::string const &	Request::getPath(void) const
{
	return (m_path);
}

Server const *	Request::getServer(void) const
{
	return (m_server);
}
		
Location const *	Request::getLocation(void) const
{
	return (m_location);
}

/* ************************************************************************** */