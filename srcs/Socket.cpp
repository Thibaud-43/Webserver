#include "ASocket.hpp"

ASocket::list_type ASocket::m_list = list_type();

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ASocket::ASocket()
{

}

ASocket::ASocket( const ASocket & src ): m_server(src.m_server), m_fd(src.m_fd), m_addr(src.m_addr)
{

}


// CLIENT & LISTENER
/*ASocket::ASocket(Server * server, socket_type type): m_server(server), m_type(type)
{
	if (m_type == listener)
	{
		_initAddr(m_server->getPort(), m_server->getIp());
		_create();
		_bind();
		_listen();
	}
	m_list.insert(*this);
}
*/

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ASocket::~ASocket()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ASocket &				ASocket::operator=( ASocket const & rhs )
{
	if ( this != &rhs )
	{
		m_server = rhs.m_server;
        m_fd = rhs.m_fd;
		m_addr = rhs.m_addr;
        m_list = rhs.m_list;

	}
	return *this;
}

bool                    operator<(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd < rhs.m_fd;
}

bool                    operator<=(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd <= rhs.m_fd;
}

bool                    operator>(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd > rhs.m_fd;
}

bool                    operator>=(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd >= rhs.m_fd;
}

bool                    operator==(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd == rhs.m_fd;
}

bool                    operator!=(ASocket & lhs, ASocket & rhs)
{
	return lhs.m_fd != rhs.m_fd;
}

ASocket::operator fd_type(void) const
{
	return m_fd;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void            ASocket::_create(void)
{
    int yes=1;

	if ((m_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
	{
        perror("socket");
        exit(1);
    }
    if (setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) 
	{
        perror("setsockopt");
        exit(1);
    }
}

void            ASocket::_bind(void)
{
    if (bind(m_fd, (struct sockaddr *)&m_addr, sizeof(address_type)) == -1) 
	{
        perror("bind");
        exit(1);
    }
}

void            ASocket::_initAddr(std::string const & port, std::string const & ip)
{

    m_addr.sin_family = AF_INET;         	
    m_addr.sin_port = htons(80);     		// a changer
    m_addr.sin_addr.s_addr = INADDR_ANY; 	// a changer
    memset(&(m_addr.sin_zero), '\0', 8); 	
}

void            ASocket::_listen(void)
{
    if (listen(m_fd, 10) == -1) 
	{
        perror("listen");
        exit(1);
    }
}

void			ASocket::_makeFdNonBlocking(void)
{
	int flags, s;

	flags = fcntl (m_fd, F_GETFL, 0);
	if (flags == -1)
	{
		perror ("fcntl");
		exit(1);

	}
	flags |= O_NONBLOCK;
	s = fcntl (m_fd, F_SETFL, flags);
	if (s == -1)
	{
		perror ("fcntl");
		exit(1);

	}
	return ;
}

void	ASocket::destroy(void)
{
	if(close(m_fd))
    {
        std::cerr << "Failed to close epoll file descriptor" << std::endl;
        return ;
    }
	m_list.erase(this);
	delete (this);
}

void                   ASocket::sendResponse(char *response)
{
    if((send(m_fd, response, strlen(response), 0)) == -1)
	{
		perror ("send");
		exit(1);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

ASocket *       ASocket::getASocketFromFd(fd_type fd)
{
	for (list_type::iterator it = m_list.begin(); it != m_list.end(); it++)
	{
		if ((*it)->m_fd == fd)
			return (*it);
	}
	return (NULL);
}

Server const *        ASocket::getServer(void)
{
	return (m_server);
}

ASocket::fd_type	ASocket::getFd(void)
{
	return (m_fd);
}

ASocket::address_type	ASocket::getAddr(void)
{
	return (m_addr);
}

/* ************************************************************************** */