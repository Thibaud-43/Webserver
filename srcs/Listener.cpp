#include "Listener.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Listener::Listener()
{
}

Listener::Listener( const Listener & src )
{
}

Listener::Listener(fd_type epoll, port_type port, ip_type ip)
{
    _initAddr(port, ip);
    _create();
    _bind();
    _makeFdNonBlocking();
    _listen();
    _epollCtlAdd(epoll);
    m_list.insert(this);
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Listener::~Listener()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Listener &				Listener::operator=( Listener const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Listener const & i )
{
	//o << "Value = " << i.getValue();
	return o;
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
    m_addr.sin_addr.s_addr = inet_addr(ip.data()); 	// a changer
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


/* ************************************************************************** */