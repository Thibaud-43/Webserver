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

Listener::Listener(Server * server)
{
    _initAddr("80", "0.0.0.0");
    _create();
    _bind();
    _listen();
    _makeFdNonBlocking();
    m_server = server;
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

void            Listener::_initAddr(std::string const & port, std::string const & ip)
{

    m_addr.sin_family = AF_INET;         	
    m_addr.sin_port = htons(80);     		// a changer
    m_addr.sin_addr.s_addr = INADDR_ANY; 	// a changer
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