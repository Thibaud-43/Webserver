#include "Response.hpp"
#include <fstream>

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

void	Response::send_error(Response::status_code_t err, Client const * client, Location const & location)
{
	Response	rep;

	rep.start_header(err);
	if (err == "405")
		rep.append_to_header("Allow: " + location.getStrMethods());
	// AUTRES LIGNES POUR AUTRES ERREURS ?
	if (location.getErrPages().find(err) != location.getErrPages().end())
		rep.fill_body(location.getErrPages().at(err));
	else
	{
		rep.append_to_body("<html>\n");
		rep.append_to_body("<head><title>" + err + " " + Response::_status[err] + "</title></head>\n");
		rep.append_to_body("<body bgcolor=\"white\">\n");
		rep.append_to_body("<center><h1>" + err + " " + Response::_status[err] + "</h1></center>\n");
		rep.append_to_body("<hr><center>");
		rep.append_to_body(SERV_NAME);
		rep.append_to_body("</center>\n");
		rep.append_to_body("</body>\n");
		rep.append_to_body("</html>");
	}
	rep.add_content_length();
	rep.send_to_client(client);
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

void	Response::send_to_client(Client const * client) const
{
	client->sendResponse(getContent().data());
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

void	Response::fill_body(file_t const & file)
{
	std::ifstream	fstream(file);
	std::string		line;

	if (!fstream)
	{
		std::cerr << file << ": Not found";
		return ;
	}
	while (getline(fstream, line))
		m_body.append(line);
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

std::string	HTTPDate(void)
{
	char	buf[100];
	time_t	now = time(0);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now));
	return (std::string(buf));
}

std::string	OSName(void)
{
	#ifdef _WIN32
    return ("Windows 32-bit");
    #elif _WIN64
    return ("Windows 64-bit");
    #elif __APPLE__ || __MACH__
    return ("Mac OSX");
    #elif __linux__
    return ("Linux");
    #elif __FreeBSD__
    return ("FreeBSD");
    #elif __unix || __unix__
    return ("Unix");
    #else
    return ("Other");
    #endif
}
