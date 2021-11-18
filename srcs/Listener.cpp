#include "Listener.hpp"
#include "FileDescriptor.hpp"
# include "Client.hpp"

Listener::list_type Listener::_list = list_type();

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Listener::Listener()
{
	memset(&m_addr, 0, sizeof(m_addr));
}

Listener::Listener( const Listener & src ): ASocket(src)
{
	m_addr.sin_addr.s_addr = src.m_addr.sin_addr.s_addr;
	m_addr.sin_family = src.m_addr.sin_family;
	m_addr.sin_port = src.m_addr.sin_port;
	memset(&m_addr.sin_zero, 0, sizeof(m_addr.sin_zero));
}

Listener::Listener(int const & fd, Server const * server, port_type const & port, ip_type const & ip): ASocket(fd, server)
{
    _initAddr(port, ip);
    _bind();
    m_fd.makeFdNonBlocking();
    _listen();
    m_fd.epollCtlAdd_r();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Listener::~Listener()
{
	//_list.erase(*this);
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Listener::alive(void) const
{
	return true;
}


bool	Listener::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
    for (;;)
	{
		struct sockaddr_in their_addr;
		socklen_t size = sizeof(struct sockaddr);
		int fd = accept(getFd(), (struct sockaddr*)&their_addr, &size);
		if (fd == -1) 
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK) 
			{
				break;
			} 
			else 
			{
				perror("accept()");
				return (true);
			}
		}
		else
		{
			std::cout << "[" << HTTPDate() << "]: server " << getFd() << " new client " << fd <<" connection " << std::endl;
            Client	*client = new Client(fd, m_server, their_addr);
			if (ptr)
				*ptr = client;
            ASocket::addSocket(client);
        }
	}
	return (true);
}


void            Listener::_bind(void)
{
    if (bind(getFd(), (struct sockaddr *)&m_addr, sizeof(address_type)) == -1) 
	{
        perror("bind");
			// COULD BE HERE
        throw std::exception();
    }

}

void            Listener::_initAddr(port_type const & port, ip_type const & ip)
{
    short   sport;

    std::istringstream(port) >> sport;
    m_addr.sin_family = AF_INET;         	
    m_addr.sin_port = htons(sport);    
    m_addr.sin_addr.s_addr = inet_addr(ip.data());
    memset(&(m_addr.sin_zero), '\0', 8); 	
}

void            Listener::_listen(void)
{
    if (listen(getFd(), 10) == -1) 
	{
        perror("listen");
        throw std::exception();
    }

}

bool	Listener::send_rep(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	return (true);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */