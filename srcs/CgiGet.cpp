#include "CgiGet.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CgiGet::CgiGet()
: ACgi(), Get()
{
}

CgiGet::CgiGet(CgiGet const & src)
: ACgi(src), Get(src)
{
}

CgiGet::CgiGet(const Get & src)
: ACgi(), Get(src)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CgiGet::~CgiGet()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	CgiGet::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	// RCV to buffer
	// UNCHUNK ?
}

bool	CgiGet::manage(int const & fd) const
{
	// SI FD == fd_in -> write buffer to fd_in
	// SI FD == fd_out -> construct & response -- send rep -- convert to client
}

bool	CgiGet::start(void)
{
	// Start Cgi -- pipes - fork - execve
}

void	CgiGet::_setEnv(void)
{
	std::string const & 			method = m_header.at("method");
	char *addr = inet_ntoa(m_client->getAddr().sin_addr); // A MODIFER

	if (m_header.find("Authorization") != m_header.end())
		m_env["AUTH_TYPE"] = m_header.at("Authorization");
	m_env["GATEWAY_INTERFACE"] = CGI_VERSION;
	m_env["SERVER_NAME"] = m_header.at("Host");
	m_env["SERVER_PORT"] = m_server->getPort();
	m_env["SERVER_PROTOCOL"] = PROTOCOL;
	m_env["SERVER_SOFTWARE"] = SERV_NAME;
	m_env["QUERY_STRING"] = m_header.at("query_string");
	m_env["REQUEST_METHOD"] = m_header.at("method");
	m_env["PATH_INFO"] = m_header.at("uri");
	m_env["SCRIPT_FILENAME"] = m_path.getPath();
	m_env["SCRIPT_NAME"] = "localhost"; // PAS SUR
	m_env["REMOTE_ADDR"] = std::string(addr);
	m_env["REDIRECT_STATUS"] = "200"; // PAS SUR
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */