#include "CgiPost.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CgiPost::CgiPost()
: ACgi(), Post()
{
	_setEnv();
}

CgiPost::CgiPost(const CgiPost & src)
: ACgi(src), Post(src)
{
	_setEnv();
}

CgiPost::CgiPost(const Post & src)
: ACgi(), Post(src)
{
	_setEnv();
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


void			CgiPost::_checkStatus(std::map<std::string, std::string> &	header, std::string & status)
{
	std::string delimiter = " ";
	if (header.find("Status") != header.end())
		status = header["Status"].substr(0, header["Status"].find(delimiter));
	else
		status = "200";
}

void			CgiPost::_bufferToHeaderLine(std::map<std::string, std::string> & header, std::string & token)
{
	std::string delimiter = ": ";
	size_t		pos = token.find(delimiter);
	header.insert(std::pair<std::string, std::string>(token.substr(0, pos), token.substr(pos + delimiter.length(), token.length() - 1)));
}

void			CgiPost::_bufferToHeader(std::map<std::string, std::string> & header)
{
	std::string delimiter = "\r\n";
	std::string delimiter2 = "\r\n\r\n";
	size_t pos = 0;
	std::string	token;

	if (m_buff == delimiter)
	{
		m_buff = "";
		return ;
	}
	while ((pos = m_buff.find(delimiter)) != std::string::npos && pos != m_buff.find(delimiter2)) 
	{
		token = m_buff.substr(0, pos);
		_bufferToHeaderLine(header, token);
		m_buff.erase(0, pos + delimiter.length());
	}
	if (m_buff.find(delimiter2) != std::string::npos)
	{
		token = m_buff.substr(0, pos);
		_bufferToHeaderLine(header, token);
		m_buff.erase(0, pos + delimiter2.length());
	}
	header.insert(std::pair<std::string, std::string>("body", m_buff));
	m_buff.clear();
}

bool	CgiPost::_handle(void)
{
	std::map<std::string, std::string>	header;
	std::string							status;

	_bufferToHeader(header);
	_checkStatus(header, status);
	if (header["body"].empty() && status != "200" && status != "302")
	{
		m_rep = Response::create_error(status, m_location);
		return (false);
	}
	else if (header["body"].empty() && status == "302")
	{
		m_rep = Response::create_redirect("302", header["Location"]);
		close(m_fd_out);
		m_fd_out = -1;
		return (true);
	}
	else
	{
		m_rep.start_header(status);
		for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++)
		{
			if (it->first != "body")
				m_rep.append_to_header(it->first + ": " + it->second);
			else
				m_rep.append_to_body(it->second);
		}
		m_rep.add_content_length();
		close(m_fd_out);
		m_fd_out = -1;
		return (true);
	}
	return (false);
}

bool	CgiPost::_fillBuffer(void)
{
	size_t              bytes_read = 0;
	char                read_buffer[READ_SIZE_RECV + 1];


	bytes_read = read(m_fd_out, read_buffer, READ_SIZE_RECV);
	if (bytes_read < 0)
	{
		close(getFd());
		return false;
	}
	else if (bytes_read == 0)
		return true;
	else if (bytes_read == READ_SIZE_RECV)
	{
		read_buffer[bytes_read] = 0;
		m_buff += read_buffer;
		if (m_buff.size() >= HEADER_SIZE_LIMIT && m_buff.find("\r\n\r\n") == std::string::npos)
		{
			m_rep = Response::create_error("431", NULL);
			return false;
		}
	}
	else
	{ 
		read_buffer[bytes_read] = 0;
		m_buff += read_buffer;
		return true;
	}
	return (true);
}

bool	CgiPost::execute(ASocket ** ptr)
{
	if (!Post::_fillBuffer())
		return (m_fd.epollCtlSwitch_w());
	return (entry(ptr));
}

bool	CgiPost::entry(ASocket ** ptr)
{	
	if (ptr)
		*ptr = this;
	if (m_header.find("Content-Length") != m_header.end())
	{
		if (m_buff.size() == _strToSize(m_header["Content-Length"]))
		{
			m_body = m_buff;
			m_buff.clear();
			if (!start())
			{
				m_rep = Response::create_error("500", m_location);
				return (m_fd.epollCtlSwitch_w());
			}
		}
		return (true);
	}
	else if (m_header.find("Transfer-Encoding") != m_header.end() && m_header["Transfer-Encoding"] == "chunked")
	{
		m_unchunker(m_buff, m_body);
		if (m_unchunker.getEnd())
		{
			m_env["CONTENT_LENGTH"] = m_unchunker.getTotalSize();
			if (!start())
			{
				m_rep = Response::create_error("500", m_location);
				return (m_fd.epollCtlSwitch_w());
			}
		}
		return (true);
	}
	else
	{
		m_rep = Response::create_error("411", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
}

bool	CgiPost::manage(ACgi ** ptr, int const & fd)
{
	int	ret;

	if (ptr)
		*ptr = this;
	if (fd == m_fd_in)
	{
		ret = write(fd, m_body.data(), m_body.size());
		m_body.clear();
		close(m_fd_in);
		m_fd_in = -1;
		if (ret <= 0 )
		{
			m_rep = Response::create_error("500", m_location);
			return (m_fd.epollCtlSwitch_w());
		}
		return (true);
	}
	else if (fd == m_fd_out)
	{
		if (!_fillBuffer() || !_handle())
			close(m_fd_out);
		return (m_fd.epollCtlSwitch_w());
	}
	else
		throw std::exception();
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
	char *addr = inet_ntoa(m_remote_addr.sin_addr);

	if (m_header.find("Authorization") != m_header.end())
		m_env["AUTH_TYPE"] = m_header.at("Authorization");
	if (m_header.find("Content-Length") != m_header.end())
		m_env["CONTENT_LENGTH"] = m_header.at("Content-Length");
	if (m_header.find("Content-Type") != m_header.end())
		m_env["CONTENT_TYPE"] = m_header.at("Content-Type");
	m_env["GATEWAY_INTERFACE"] = CGI_VERSION;
	m_env["SERVER_NAME"] = m_header.at("Host");
	m_env["SERVER_PORT"] = m_server->getPort();
	m_env["SERVER_PROTOCOL"] = PROTOCOL;
	m_env["SERVER_SOFTWARE"] = SERV_NAME;
	if (m_header.find("query_string") != m_header.end())
		m_env["QUERY_STRING"] = m_header.at("query_string");
	m_env["REQUEST_METHOD"] = "POST";
	m_env["PATH_INFO"] = m_header.at("uri");
	m_env["SCRIPT_FILENAME"] = m_path.getPath();
	m_env["SCRIPT_NAME"] = m_header.at("Host");
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
		m_rep = Response::create_error("500", m_location);
		ACgi::clear();
		return (m_fd.epollCtlSwitch_w());
	}
	else if (!ret)
		return (true);
	else
	{
		if (!WIFEXITED(status))
		{
			m_rep = Response::create_error("500", m_location);
			m_pid = -1;
			ACgi::clear();
			return (m_fd.epollCtlSwitch_w());
		}
		else if (m_fd_in > 0)
		{
			close(m_fd_in);
			m_fd_in = -1;
		}
		fd_out.epollCtlSwitch_w();
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
	argv[1][m_path.getPath().size()] = 0;
	argv[2] = 0;
	return (argv);
}

/* ************************************************************************** */