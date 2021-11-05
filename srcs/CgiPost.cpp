#include "CgiPost.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CgiPost::CgiPost()
: ACgi(), Post()
{
}

CgiPost::CgiPost(const CgiPost & src)
: ACgi(src), Post(src)
{
}

CgiPost::CgiPost(const Post & src)
: ACgi(), Post(src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CgiPost::~CgiPost()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	CgiPost::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	// RCV to buffer
	// UNCHUNK ?
}

bool	CgiPost::manage(int const & fd) const
{
	// SI FD == fd_in -> write buffer to fd_in
	// SI FD == fd_out -> construct & response -- send rep -- convert to client
}

bool	CgiPost::start(void)
{
	// Start Cgi -- pipes - fork - execve
}

void	CgiPost::_setEnv(void)
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