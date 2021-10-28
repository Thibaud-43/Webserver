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

void			Cgi::_bufferToHeaderLine(std::map<std::string, std::string> & header, std::string & token) const
{
	std::string delimiter = ": ";
	size_t		pos = token.find(delimiter);
	header.insert(std::pair<std::string, std::string>(token.substr(0, pos), token.substr(pos + delimiter.length(), token.length() - 1)));
}

void			Cgi::_bufferToHeader(std::map<std::string, std::string> & header, std::string & buffer) const
{
	std::string delimiter = "\r\n";
	std::string delimiter2 = "\r\n\r\n";
	size_t pos = 0;
	std::string	token;

	if (buffer == delimiter)
	{
		buffer = "";
		return ;
	}
	while ((pos = buffer.find(delimiter)) != std::string::npos && pos != buffer.find(delimiter2)) 
	{
		token = buffer.substr(0, pos);
		_bufferToHeaderLine(header, token);
		buffer.erase(0, pos + delimiter.length());
	}
	if (buffer.find(delimiter2) != std::string::npos)
	{
		token = buffer.substr(0, pos);
		_bufferToHeaderLine(header, token);
		buffer.erase(0, pos + delimiter2.length());
	}
	header.insert(std::pair<std::string, std::string>("body", buffer));
	buffer.clear();
}

void			Cgi::_printHeader(std::map<std::string, std::string> &	header) const
{
	std::cout << std::endl << std::endl << "CGI HEADER" << std::endl;
	for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++)
	{
		std::cout << "[" << it->first << "]='" << it->second << "'" << std::endl; 
	}
}

void			Cgi::_checkStatus(std::map<std::string, std::string> &	header, std::string & status) const
{
	std::string delimiter = " ";
	if (header.find("Status") != header.end())
		status = header["Status"].substr(0, header["Status"].find(delimiter));
	else
		status = "200";
}


bool	Cgi::handle(std::string & buffer) const
{
	std::map<std::string, std::string>	header;
	Response							rep;
	std::string							status;
	_bufferToHeader(header, buffer);
	_checkStatus(header, status);
	if (status != "200" && status != "302")
	{
		Response::send_error(status, m_client, Request::getRequestFromClient(*m_client)->getLocation());
	}
	else if (status == "302")
	{
		Response::redirect("302", header["uri"] + "/" , m_client);
	}
	else
	{
		rep.start_header("200");
		for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++)
		{
			if (it->first != "body")
				rep.append_to_header(it->first + ": " + it->second);
			else
				rep.append_to_body(it->second);
		}
		rep.send_to_client(m_client);
	}
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