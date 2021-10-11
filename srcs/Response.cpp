#include "Response.hpp"

Response::status_t Response::_status = createStatus();

Response::status_t	Response::createStatus(void)
{
	_status["100"] = "Continue";
	_status["101"] = "Switching Protols";
	_status["200"] = "OK";
	_status["201"] = "Created";
	_status["202"] = "Accepted";
	_status["203"] = "Non-Authoritative Information";
	_status["204"] = "No Content";
	_status["205"] = "Reset Content";
	_status["206"] = "Partial Content";
	_status["301"] = "Moved Permanently";
	_status["302"] = "Found";
	_status["303"] = "See Other";
	_status["304"] = "Not Modified";
	_status["305"] = "Use Proxy";
	_status["307"] = "Temporary Redirect";
	_status["400"] = "Bad Request";
	_status["401"] = "Unauthorized";
	_status["402"] = "Bad Request";
	_status["403"] = "Forbidden";
	_status["404"] = "Not Found";
	_status["405"] = "Method Not Allowed";
	_status["406"] = "Not Acceptable";
	_status["407"] = "Proxy Authentification Required";
	_status["408"] = "Request Timeout";
	_status["409"] = "Conflict";
	_status["410"] = "Gone";
	_status["411"] = "Length Required";
	_status["412"] = "Precondition Failed";
	_status["413"] = "Payload Too Large";
	_status["414"] = "URI Too Long";
	_status["415"] = "Unsupported Media Type";
	_status["416"] = "Range Not Satisfiable";
	_status["417"] = "Expectation Failed";
	_status["426"] = "Upgrade Required";
	_status["500"] = "Internal Server Error";
	_status["501"] = "Not Implemented";
	_status["502"] = "Bad Gateway";
	_status["503"] = "Service Unavalaible";
	_status["504"] = "Gateway Timeout";
	_status["505"] = "HTTP Version Not Supported";
	return (_status);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response()
{
}

Response::Response( const Response & src ): m_header(src.m_header), m_body(src.m_body)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Response &				Response::operator=( Response const & rhs )
{
	if ( this != &rhs )
		m_header = rhs.m_header;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Response const & i )
{
	o << i.getContent();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	Response::append_to_header(std::string const & str)
{
	m_header.append(str);
}

void	Response::append_to_body(std::string const & str)
{
	m_body.append(str);
}

void	Response::send_to_client(Client const & client) const
{
	client.sendResponse(getContent().data());
}

std::string const & Response::getContent(void) const
{
	return (std::string(m_header + "\r\n" + m_body));
}

void	Response::start_header(Response::status_code_t const & status)
{
	m_header += PROTOCOL;
	m_header += " " + status + " " + _status[status] + "\r\n";
	m_header += "Date: " + HTTPDate() + "\r\n";
	m_header += "Server: ";
	m_header += SERV_NAME;
	m_header += " (" + OSName() + ")\r\n";
}

void	Response::clear(void)
{
	m_header.clear();
	m_body.clear();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */