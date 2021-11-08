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
	return (true);
}

bool	CgiGet::manage(ACgi **ptr, int const & fd)
{
	if (fd == m_fd_out)
	{
		// SI FD == fd_out -> construct & response -- send rep -- convert to client
	}
	return (true);
}

bool	CgiGet::start(void)
{
	char	**envp;
	char	**args;
	int		pipefd_out[2];

	if (pipe(pipefd_out))
		return (false);
	m_fd_out = pipefd_out[0];
	if (fcntl(m_fd_out, F_SETFL, O_NONBLOCK) == -1)
	{
		_close_pipes(pipefd_out);
		return (false);
	}
	envp = getEnv();
	args = getArgs();
	m_pid = fork();
	if (m_pid < 0)
	{
		_close_pipes(pipefd_out);
		del_env(envp);
		del_env(args);
		return (false);
	}
	else if (!m_pid)
	{
		if (dup2(pipefd_out[1], STDOUT_FILENO) < 0)
			exit(1);
		_close_pipes(pipefd_out);
		if (execve(args[0], args, envp) < 0)
			exit(1);
	}
	close(pipefd_out[1]);
	del_env(envp);
	del_env(args);
	return (true);
}

void	CgiGet::_setEnv(void)
{
	char *addr = inet_ntoa(m_remote_addr.sin_addr);

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
	m_env["REDIRECT_STATUS"] = "200";
}

bool	CgiGet::checkStatus(void)
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

char **	CgiGet::getArgs(void) const
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