
#include "Listener.hpp"
Listener::list_type Listener::_list = list_type();

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Listener::Listener()
{
}

Listener::Listener( const Listener & src ): ASocket(src)
{
    //_list = src._list;
}

Listener::Listener(int const & fd, port_type const & port, ip_type const & ip): ASocket(fd)
{
    _initAddr(port, ip);
    _bind();
    m_fd.makeFdNonBlocking();
    _listen();
    m_fd.epollCtlAdd();
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

Listener &				Listener::operator=( Listener const & rhs )
{
	if ( this != &rhs )
	{
        ASocket::operator=(rhs);
		this->_list = rhs._list;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Listener::execute(void)
{
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
				return ;
			}
		}
		else
		{
            Client	*client = new Client(fd);
            ASocket::addSocket(client);
        }

	}
}


void            Listener::_bind(void)
{
    if (bind(getFd(), (struct sockaddr *)&m_addr, sizeof(address_type)) == -1) 
	{
        perror("bind");
        exit(1);
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
        exit(1);
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */