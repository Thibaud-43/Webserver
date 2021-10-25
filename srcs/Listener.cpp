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

Listener::Listener(fd_type epoll, port_type port, ip_type ip)
{
    _initAddr(port, ip);
    _create();
    _bind();
    _makeFdNonBlocking();
    _listen();
    _epollCtlAdd(epoll);
    _list.insert(*this);
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
void            Listener::_create(void)
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

void            Listener::_bind(void)
{
    if (bind(m_fd, (struct sockaddr *)&m_addr, sizeof(address_type)) == -1) 
	{
        perror("bind");
        exit(1);
    }
}

void            Listener::_initAddr(port_type port, ip_type ip)
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
    if (listen(m_fd, 10) == -1) 
	{
        perror("listen");
        exit(1);
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

bool		Listener::isListenerFd(fd_type fd)
{
    for (std::set<Listener>::iterator it = _list.begin() ; it != _list.end(); it++)
    {
        if ((*it).m_fd == fd)
        {
            return true;
        }
    }
    return false;
}

/* ************************************************************************** */