#include "Cgi.hpp"

Cgi::list_type Cgi::_list = list_type();

bool		Cgi::isCgiFd(fd_type & fd)
{
    for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
    {
        if (it->m_fd_out == fd)
            return (true);
    }
    return (false);
}

void		Cgi::removeCgi(Cgi const & cgi)
{
	close(cgi.m_fd_out);
	_list.erase(cgi);
}

Cgi	const *	Cgi::getCgiFromFd(fd_type fd)
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (it->m_fd_out == fd)
			return (&(*it));
	}
	return (NULL);
}

Cgi const *	Cgi::getCgiFromClient(Client const * client)
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (it->m_client == client)
			return (&(*it));
	}
	return (NULL);
}

Cgi const *Cgi::addCgi(Cgi const & cgi)
{
	return (&(*_list.insert(cgi).first));
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cgi::Cgi(): m_pid(-1), m_fd_out(-1), m_client(NULL), m_env(env_type())
{
}

Cgi::Cgi( const Cgi & src )
	: m_pid(src.m_pid), m_fd_out(src.m_fd_out), m_client(src.m_client), m_env(src.m_env)
{

}

Cgi::Cgi(Request const & request, std::string const & cgi_path)
	: m_pid(-1), m_fd_out(-1), m_client(request.getClient()), m_env(Cgi::env_type())
{
	Request::header_type const &	header = request.getHeader();

	if (header.find("Authorization") != header.end())
		m_env["AUTH_TYPE"] = header.at("Authorization");
	if (header.find("Content-Length") != header.end())
		m_env["CONTENT_LENGTH"] = header.at("Content-Length");
	if (header.find("Content-Type") != header.end())
		m_env["CONTENT_TYPE"] = header.at("Content-Type");
	m_env["GATEWAY_INTERFACE"] = CGI_VERSION;
	m_env["PATH_INFO"] = header.at("uri");
	m_env["PATH_TRANSLATED"] = request.getPath();
	m_env["QUERY_STRING"] = header.at("query_string");
	m_env["REMOTE_ADDR"] = request.getServer()->getIp();
	m_env["REQUEST_METHOD"] = header.at("method");
	m_env["SCRIPT_NAME"] = cgi_path;
	m_env["SERVER_NAME"] = header.at("Host");
	m_env["SERVER_PORT"] = request.getServer()->getPort();
	m_env["SERVER_PROTOCOL"] = PROTOCOL;
	m_env["SERVER_SOFTWARE"] = SERV_NAME;
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

Cgi &				Cgi::operator=( Cgi const & rhs )
{
	if ( this != &rhs )
	{
		m_pid = rhs.m_pid;
		m_fd_out = rhs.m_fd_out;
		m_client = rhs.m_client;
		m_env = rhs.m_env;
	}
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Cgi::handle(std::string & buffer) const
{
	/*Response		rep;
	std::string 	delimiter = "\r\n";
	std::string 	delimiter2 = "\r\n\r\n";
	size_t 			pos = 0;
	std::string		token;
	std::string		status = "Status: 500 Internal Server Error";

	if (pos = buffer.find(status))
	{
		rep.send_error("500", m_client, Request::getRequestFromClient(*m_client)->getLocation());
		return (true);
	}
	rep.start_header("200");
	while ((pos = buffer.find(delimiter)) != std::string::npos && pos != buffer.find(delimiter2)) 
	{
		token = buffer.substr(0, pos);
		rep.append_to_header(token);
		buffer.erase(0, pos + delimiter.length());	}
	if (buffer.find(delimiter2) != std::string::npos)
	{
		token = buffer.substr(0, pos);
		rep.append_to_header(token);
		buffer.erase(0, pos + delimiter2.length());
	}*/
	std::cout << buffer << std::endl;
	return (true);
}

bool	Cgi::run(char const *cgi_path, char *const *args)
{
	int		pipefd[2];
	char	**envp;

	if (pipe(pipefd))
		return (false);
	m_fd_out = pipefd[0];
	if (fcntl(m_fd_out, F_SETFL, O_NONBLOCK) == -1)
		return (false);
	envp = getEnv();
	m_pid = fork();
	if (m_pid < 0)
	{
		del_env(envp);
		return (false);
	}
	else if (!m_pid)
	{
		dup2(STDOUT_FILENO, pipefd[1]);
		if (execve(cgi_path, args, envp) < 0)
			exit(1);
	}
	else
		del_env(envp);
	return (true);
}

bool	Cgi::check_status(void) const
{
	int	status;
	int	ret = waitpid(m_pid, &status, WNOHANG);
	Request const *	request = Request::getRequestFromClient(*m_client);

	if (ret < 0)
	{
		Response::send_error("500", m_client, request->getLocation());
		Request::removeRequest(*request);
		return (true);
	}
	else if (!ret)
		return (false);
	else
	{
		if (!WIFEXITED(status))
		{
			Response::send_error("500", m_client, request->getLocation());
			Request::removeRequest(*request);
			return (true);
		}
		ASocket::epollCtlAdd(Cluster::getEpollFd(), m_fd_out);
		return (false);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

Client const *	Cgi::getClient(void) const
{
	return (m_client);
}

Cgi::fd_type	Cgi::getFd_out(void) const
{
	return (m_fd_out);
}

pid_t			Cgi::getPid(void) const
{
	return (m_pid);
}

char	**Cgi::getEnv(void) const
{
	char		**env = new char*[m_env.size() + 1];
	size_t		i = 0;
	std::string	buff;

	for (Cgi::env_type::const_iterator it = m_env.begin(); it != m_env.end(); it++)
	{
		buff = it->first + "=" + it->second;
		env[i] = new char[buff.size() + 1];
		buff.copy(env[i], buff.size());
		env[i][buff.size()] = 0;
		i++;
	}
	env[i] = 0;
	return (env);
}

void	Cgi::del_env(char **envp)
{
	size_t	i = 0;

	while (envp[i])
	{
		delete [] envp[i];
		i++;
	}
	delete [] envp;
}

/* ************************************************************************** */

bool	operator<(Cgi const & lhs, Cgi const & rhs)
{
	return (lhs.m_client < rhs.m_client);
}

bool	operator<=(Cgi const & lhs, Cgi const & rhs)
{
	return (lhs.m_client <= rhs.m_client);
}

bool	operator>(Cgi const & lhs, Cgi const & rhs)
{
	return (lhs.m_client > rhs.m_client);
}

bool	operator>=(Cgi const & lhs, Cgi const & rhs)
{
	return (lhs.m_client >= rhs.m_client);
}

bool	operator==(Cgi const & lhs, Cgi const & rhs)
{
	return (lhs.m_client == rhs.m_client);
}

bool	operator!=(Cgi const & lhs, Cgi const & rhs)
{
	return (lhs.m_client != rhs.m_client);
}