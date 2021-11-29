#include "Response.hpp"

Response::status_t Response::_status = Response::_createStatus();

Response::status_t	Response::_createStatus(void)
{
	Response::status_t	status;

	status["200"] = "OK";
	status["201"] = "Created";
	status["300"] = "Multiple Choices";
	status["301"] = "Moved Permanently";
	status["302"] = "Found";
	status["303"] = "See Other";
	status["304"] = "Not Modifed";
	status["307"] = "Temporary Redirect";
	status["308"] = "Temporary Redirect";
	status["400"] = "Bad Request";
	status["403"] = "Forbidden";
	status["404"] = "Not Found";
	status["405"] = "Method Not Allowed";
	status["408"] = "Request Timeout";
	status["409"] = "Conflict";
	status["411"] = "Length Required";
	status["412"] = "Precondition Failed";
	status["413"] = "Payload Too Large";
	status["415"] = "Unsupported Media Type";
	status["416"] = "Range Not Satisfiable";
	status["431"] = "Request Header Fields Too Large",
	status["500"] = "Internal Server Error";
	status["505"] = "HTTP Version Not Supported";
	return (status);
}

Response	Response::create_error(status_code_t const & err, Location const * location)
{
	if (location && location->getErrPages().find(err) != location->getErrPages().end())
		return (Response::create_redirect("302", location->getErrPages().at(err)));
	else
	{
		Response	rep;

		rep.start_header(err);
		if (location && err == "405")
			rep.append_to_header("Allow: " + location->getStrMethods());
		rep.append_to_body("<html>\n");
		rep.append_to_body("<head><title>" + err + " " + Response::_status[err] + "</title></head>\n");
		rep.append_to_body("<body bgcolor=\"white\">\n");
		rep.append_to_body("<center><h1>" + err + " " + Response::_status[err] + "</h1></center>\n");
		rep.append_to_body("<hr><center>");
		rep.append_to_body(SERV_NAME);
		rep.append_to_body("</center>\n");
		rep.append_to_body("</body>\n");
		rep.append_to_body("</html>\n");
		rep.append_to_header("Content-Type: text/html");
		rep.append_to_header("Connection: close");
		return (rep);
	}	
}

Response	Response::create_redirect(Response::status_code_t const & red, std::string const & location)
{
	Response	rep;

	rep.start_header(red);
	rep.append_to_body("<html>\n");
	rep.append_to_body("<head><title>" + red + " " + Response::_status[red] + "</title></head>\n");
	rep.append_to_body("<body bgcolor=\"white\">\n");
	rep.append_to_body("<center><h1>" + red + " " + Response::_status[red] + "</h1></center>\n");
	rep.append_to_body("<hr><center>");
	rep.append_to_body(SERV_NAME);
	rep.append_to_body("</center>\n");
	rep.append_to_body("</body>\n");
	rep.append_to_body("</html>\n");
	rep.append_to_header("Content-Type: text/html");
	rep.add_content_length();
	rep.append_to_header("Location: " + location);
	return (rep);
}
	
Response	Response::create_index(std::string const & directory, Location const * location, std::string const & uri)
{
	Location::indexes_t const & indexes = location->getIndexes();
	File						current;

	for (Location::indexes_t::const_iterator it = indexes.begin(); it != indexes.end(); it++)
	{
		current.setPath(directory + *it);
		if (current.is_regular())
			return (Response::create_redirect("301", uri + *it));
	}
	Response						rep;
	std::vector<std::string> const	files = File::currentFiles(directory);

	rep.start_header("200");
	rep.append_to_body("<html>\n");
	rep.append_to_body("<head><title>Index of /</title></head>\n");
	rep.append_to_body("<body bgcolor=\"white\">\n");
	rep.append_to_body("<h1>Index of " + uri + "</h1><hr><pre><a href=\"../\">../</a>\n"); // retirer directory[0]
	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
	{
		current.setPath(uri + *it);
		rep.append_to_body("<a href=\"" + *it + "\">" + *it + "</a>\t\t\t\t");
		rep.append_to_body(current.last_modification_str() + "\t\t");
		current.setPath(directory + *it);
		if (current.is_regular())
			rep.append_to_body(current.size_str() + "\n");
		else
			rep.append_to_body("-\n");
	}
	rep.append_to_body("</pre><hr></body>\n");
	rep.append_to_body("</html>\n");
	rep.append_to_header("Content-Type: text/html");
	rep.add_content_length();
	return (rep);
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
	{
		m_header = rhs.m_header;
		m_body = rhs.m_body;
	}
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

bool	Response::close(void) const
{
	return (m_header.find("Connection: close") != std::string::npos);
}

void	Response::append_to_header(std::string const & str)
{
	m_header.append(str + "\r\n");
}

void	Response::append_to_body(std::string const & str)
{
	m_body.append(str);
}

void	Response::add_content_length(void)
{
	std::stringstream	sstream;

	sstream << m_body.size();
	m_header += "Content-Length: " + sstream.str() + "\r\n";
}

std::string Response::getContent(void) const
{
	if (!m_header.empty())
		return (m_header + "\r\n" + m_body);
	else
		return (m_body);
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
	std::ifstream	fstream(file.data());
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
	time_t	now = time(NULL);
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