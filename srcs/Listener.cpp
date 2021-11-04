
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

Listener::Listener(int fd, port_type port, ip_type ip): ASocket(fd)
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
void            Listener::_bind(void)
{
    if (bind(getFd(), (struct sockaddr *)&m_addr, sizeof(address_type)) == -1) 
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