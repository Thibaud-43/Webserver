#include "Response.hpp"

Response::status_t Response::_status = Response::_createStatus();

Response::status_t	Response::_createStatus(void)
{
	Response::status_t	status;

	status["100"] = "Continue";
	status["101"] = "Switching Protols";
	status["200"] = "OK";
	status["201"] = "Created";
	status["202"] = "Accepted";
	status["203"] = "Non-Authoritative Information";
	status["204"] = "No Content";
	status["205"] = "Reset Content";
	status["206"] = "Partial Content";
	status["301"] = "Moved Permanently";
	status["302"] = "Found";
	status["303"] = "See Other";
	status["304"] = "Not Modified";
	status["305"] = "Use Proxy";
	status["307"] = "Temporary Redirect";
	status["400"] = "Bad Request";
	status["401"] = "Unauthorized";
	status["402"] = "Bad Request";
	status["403"] = "Forbidden";
	status["404"] = "Not Found";
	status["405"] = "Method Not Allowed";
	status["406"] = "Not Acceptable";
	status["407"] = "Proxy Authentification Required";
	status["408"] = "Request Timeout";
	status["409"] = "Conflict";
	status["410"] = "Gone";
	status["411"] = "Length Required";
	status["412"] = "Precondition Failed";
	status["413"] = "Payload Too Large";
	status["414"] = "URI Too Long";
	status["415"] = "Unsupported Media Type";
	status["416"] = "Range Not Satisfiable";
	status["417"] = "Expectation Failed";
	status["426"] = "Upgrade Required";
	status["500"] = "Internal Server Error";
	status["501"] = "Not Implemented";
	status["502"] = "Bad Gateway";
	status["503"] = "Service Unavalaible";
	status["504"] = "Gateway Timeout";
	status["505"] = "HTTP Version Not Supported";
	return (status);
}

void	Response::_send_error(status_code_t err, Client const & client, Location const & location);
{
	
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

void	Response::add_content_length(void)
{
	std::stringstream	sstream;

	sstream << m_body.size();
	m_header += "Content-Length: " + sstream.str() + "\r\n";
}

std::string Response::getContent(void) const
{
	return (m_header + "\r\n" + m_body);
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