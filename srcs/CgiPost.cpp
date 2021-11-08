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
	if (m_header.find("Content-Length") != m_header.end())
	{
		if (!_fillBuffer())
			return false;
		if (m_header["Content-Length"] == "0")
		{
			m_body = m_buff;
			m_buff.clear();
		}
	}
	else if (m_header.find("Transfer-Encoding") != m_header.end() && m_header["Transfer-Encoding"] == "chunked")
	{
		if (!_fillBuffer())
			return false;
		m_unchunker(m_buff, m_body);
	}
	return (true);
}

bool	CgiPost::manage(ACgi ** ptr, int const & fd)
{
	if (fd == m_fd_in)
	{
		write(fd, m_body.data(), m_body.size());
		m_body.clear();
	}
	else
	{
		// SI FD == fd_out -> construct & response -- send rep -- convert to client
	}
	return (true);
}

bool	CgiPost::start(void)
{
	char	**envp;
	char	**argv;
	int		pipefd_out[2];
	int		pipefd_in[2];

	if (pipe(pipefd_out))
		return (false);
	m_fd_out = pipefd_out[0];
	if (fcntl(m_fd_out, F_SETFL, O_NONBLOCK) == -1)
	{
		_close_pipes(pipefd_out);
		return (false);
	}
	if (pipe(pipefd_in))
	{
		_close_pipes(pipefd_out);
		return (false);
	}
		m_fd_in = pipefd_in[1];
	if (fcntl(m_fd_in, F_SETFL, O_NONBLOCK) == -1)
	{
		_close_pipes(pipefd_out, pipefd_in);
		return (false);
	}
	envp = getEnv();
	argv = getArgs();
	m_pid = fork();
	if (m_pid < 0)
	{
		_close_pipes(pipefd_out, pipefd_in);
		del_env(envp);
		del_env(argv);
		return (false);
	}
	else if (!m_pid)
	{
		if (dup2(pipefd_out[1], STDOUT_FILENO) < 0)
			exit(1);
		if (dup2(pipefd_in[0], STDIN_FILENO) < 0)
			exit(1);
		_close_pipes(pipefd_out, pipefd_in);
		if (execve(argv[0], argv, envp) < 0)
			exit(1);
	}
	else
	{
		FileDescriptor	f(m_fd_in);

		f.epollCtlAdd_w();
		close(pipefd_in[0]);
		close(pipefd_out[1]);
		del_env(envp);
		del_env(argv);
	}
	return (true);
}

void	CgiPost::_setEnv(void)
{
	std::string const & 			method = m_header.at("method");
	char *addr = inet_ntoa(m_remote_addr.sin_addr);

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
	m_env["REDIRECT_STATUS"] = "200";
}

bool	CgiPost::checkStatus(void)
{
	int				status;
	int				ret = waitpid(m_pid, &status, WNOHANG);
	FileDescriptor	fd_out(m_fd_out);
	
	if (ret < 0)
	{
		_send(Response::create_error("500", m_location));
		return (false);
	}
	else if (!ret)
		return (true);
	else
	{
		if (!WIFEXITED(status))
		{
			_send(Response::create_error("500", m_location));
			m_pid = -1;
			return (false);
		}
		else if (m_fd_in > 0)
		{
			close(m_fd_in);
			m_fd_in = -1;
		}
		fd_out.epollCtlAdd();
		m_pid = -1;
		return (true);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

char **	CgiPost::getArgs(void) const
{
	char 			**argv = new char*[3];
	
	argv[0] = new char[m_cgi_pass->size() + 1];
	m_cgi_pass->copy(argv[0], m_cgi_pass->size());
	argv[0][m_cgi_pass->size()] = 0;
	argv[1] = new char[m_path.getPath().size() + 1];
	m_path.getPath().copy(argv[1], m_path.getPath().size());
	argv[1][m_path.size()] = 0;
	argv[2] = 0;
	return (argv);
}

/* ************************************************************************** */