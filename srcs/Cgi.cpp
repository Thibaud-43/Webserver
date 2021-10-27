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

void		Cgi::checkChildsStatus(void)
{
	for (list_type::iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (!it->check_status())
		{
			removeCgi(*it);
		}
	}
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
	char *addr = inet_ntoa(m_client->getAddr().sin_addr);

	if (header.find("Authorization") != header.end())
		m_env["AUTH_TYPE"] = header.at("Authorization");
	if (header.find("Content-Length") != header.end())
		m_env["CONTENT_LENGTH"] = header.at("Content-Length");
	if (header.find("Content-Type") != header.end())
		m_env["CONTENT_TYPE"] = header.at("Content-Type");
	m_env["GATEWAY_INTERFACE"] = CGI_VERSION;
	m_env["SERVER_NAME"] = header.at("Host");
	m_env["SERVER_PORT"] = request.getServer()->getPort();
	m_env["SERVER_PROTOCOL"] = PROTOCOL;
	m_env["SERVER_SOFTWARE"] = SERV_NAME;
	m_env["QUERY_STRING"] = header.at("query_string");
	m_env["REQUEST_METHOD"] = header.at("method");
	m_env["PATH_INFO"] = request.getPath();
	m_env["SCRIPT_FILENAME"] = request.getPath();
	m_env["SCRIPT_NAME"] = cgi_path;
	m_env["REMOTE_ADDR"] = std::string(addr);
	m_env["REDIRECT_STATUS"] = "200";
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
	std::string		errorStatus = "Status: 500";
	std::string		locationStatus = "Status: 302";

	if (pos = buffer.find(errorStatus))
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
	}
	check_status();*/
	std::cout << buffer << std::endl;
	Response		rep;
	rep.start_header("200");
	rep.add_content_length();
	rep.debug();
	rep.send_to_client(m_client);
	ASocket::epollCtlDel(m_fd_out);

	return (false);
}

bool	Cgi::run(char *const *args)
{
	char	**envp;

	if (pipe(m_pipefd))
		return (false);
	m_fd_out = m_pipefd[0];
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
		close(m_pipefd[0]);
		if (dup2(m_pipefd[1], STDOUT_FILENO) < 0)
			exit(1);
		close(m_pipefd[1]);
		if (execve(args[0], args, envp) < 0)
			exit(1);
	}
	else
	{
		close(m_pipefd[1]);
		del_env(envp);
	}
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
		return (false);
	}
	else if (!ret)
	{
		return (true);
	}
	else
	{
		if (!WIFEXITED(status))
		{
			Response::send_error("500", m_client, request->getLocation());
			Request::removeRequest(*request);
			return (false);
		}
		ASocket::epollCtlAdd(m_fd_out);
		return (true);
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