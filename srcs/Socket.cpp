#include "Socket.hpp"

Socket::list_type Socket::m_list = list_type();

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket()
{

}

Socket::Socket( const Socket & src ): m_server(src.m_server), m_type(src.m_type), m_fd(src.m_fd), m_addr(src.m_addr)
{

}

Socket::Socket(Server * server, socket_type type): m_server(server), m_type(type)
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


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Socket &				Socket::operator=( Socket const & rhs )
{
	if ( this != &rhs )
	{
		m_server = rhs.m_server;
        m_type = rhs.m_type;
        m_fd = rhs.m_fd;
		m_addr = rhs.m_addr;
        m_list = rhs.m_list;

	}
	return *this;
}

bool                    operator<(Socket & lhs, Socket & rhs)
{
	return lhs.m_fd < rhs.m_fd;
}

bool                    operator<=(Socket & lhs, Socket & rhs)
{
	return lhs.m_fd <= rhs.m_fd;
}

bool                    operator>(Socket & lhs, Socket & rhs)
{
	return lhs.m_fd > rhs.m_fd;
}

bool                    operator>=(Socket & lhs, Socket & rhs)
{
	return lhs.m_fd >= rhs.m_fd;
}

bool                    operator==(Socket & lhs, Socket & rhs)
{
	return lhs.m_fd == rhs.m_fd;
}

bool                    operator!=(Socket & lhs, Socket & rhs)
{
	return lhs.m_fd != rhs.m_fd;
}

Socket::operator fd_type(void) const
{
	return m_fd;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void            Socket::_create(void)
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

void            Socket::_bind(void)
{
    if (bind(m_fd, (struct sockaddr *)&m_addr, sizeof(address_type)) == -1) 
	{
        perror("bind");
        exit(1);
    }
}

void            Socket::_initAddr(std::string const & port, std::string const & ip)
{

    m_addr.sin_family = AF_INET;         	
    m_addr.sin_port = htons(80);     		// a changer
    m_addr.sin_addr.s_addr = INADDR_ANY; 	// a changer
    memset(&(m_addr.sin_zero), '\0', 8); 	
}

void            Socket::_listen(void)
{
    if (listen(m_fd, 10) == -1) 
	{
        perror("listen");
        exit(1);
    }
}

void			Socket::_makeFdNonBlocking(void)
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

void                   Socket::closeFd(fd_type fd)
{
	if(close(fd))
    {
        std::cerr << "Failed to close epoll file descriptor" << std::endl;
        return ;
    }
	m_list.erase(*getSocketFromFd(fd));
}

void                   Socket::sendResponse(char *response)
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

Socket const *       Socket::getSocketFromFd(fd_type fd)
{
	for (list_type::iterator it = m_list.begin(); it != m_list.end(); it++)
	{
		if ((*it).m_fd == fd)
		{
			return &(*it);
		}
	}
	return NULL;
}


/* ************************************************************************** */