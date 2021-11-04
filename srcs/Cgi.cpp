#include "Cgi.hpp"

Cgi::list_type Cgi::_list = Cgi::list_type();

Cgi const	*	Cgi::getCgi(int const & fd)
{
	for (list_type::const_iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (fd == (*it)->getFdIn() || fd == (*it)->getFdOut())
			return (*it);
	}
	return (NULL);
}

Cgi const *	Cgi::addCgi(Cgi const * cgi)
{
	return (*_list.insert(cgi).first);
}

void	Cgi::removeCgi(Cgi const * cgi)
{
	if (cgi->getFdIn() >= 0)
		close(cgi->getFdIn());
	if (cgi->getFdOut() >= 0)
		close(cgi->getFdOut());
	_list.erase(cgi);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi()
: Get(), Post(), m_fd_in(-1), m_fd_out(-1)
{
	_setEnv();
}

Cgi::Cgi( const Cgi & src )
: Get(src), Post(src), m_fd_in(-1), m_fd_out(-1)
{
	_setEnv();
}

Cgi::Cgi(Get const & src)
: Post(), Get(src), m_fd_in(-1), m_fd_out(-1)
{
	_setEnv();
}

Cgi::Cgi(Post const & src)
: Get(), Post(src), m_fd_in(-1), m_fd_out(-1)
{
	_setEnv();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cgi::~Cgi()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Cgi::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	// RCV to buffer
	// UNCHUNK ?
}

bool	Cgi::manage(int const & fd) const
{
	// SI FD == fd_in -> write buffer to fd_in (si unchunk)
	// SI FD == fd_out -> construct & response -- send rep -- convert to client
}

bool	Cgi::start(void)
{
	// Start Cgi -- pipes - fork - execve
}

void	Cgi::_setEnv(void)
{
	std::string const & 			method = m_header.at("method");
	char *addr = inet_ntoa(m_client->getAddr().sin_addr); // A MODIFER

	if (m_header.find("Authorization") != m_header.end())
		m_env["AUTH_TYPE"] = m_header.at("Authorization");
	if (method == "POST")
	{
		if (m_header.find("Content-Length") != m_header.end())
			m_env["CONTENT_LENGTH"] = m_header.at("Content-Length");
		if (m_header.find("Content-Type") != m_header.end())
			m_env["CONTENT_TYPE"] = m_header.at("Content-Type");
	}
	m_env["GATEWAY_INTERFACE"] = CGI_VERSION;
	m_env["SERVER_NAME"] = m_header.at("Host");
	m_env["SERVER_PORT"] = request.getServer()->getPort(); // A MODIFIER
	m_env["SERVER_PROTOCOL"] = PROTOCOL;
	m_env["SERVER_SOFTWARE"] = SERV_NAME;
	m_env["QUERY_STRING"] = m_header.at("query_string");
	m_env["REQUEST_METHOD"] = m_header.at("method");
	m_env["PATH_INFO"] = m_header.at("uri");
	m_env["SCRIPT_FILENAME"] = request.getPath();
	m_env["SCRIPT_NAME"] = "localhost"; // PAS SUR
	m_env["REMOTE_ADDR"] = std::string(addr);
	m_env["REDIRECT_STATUS"] = "200"; // PAS SUR
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */