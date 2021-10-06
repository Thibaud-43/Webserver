#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
    m_ip = "0.0.0.0";
	m_port = "80";
	m_name = "localhost";
}

Server::Server( const Server & src )
{

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int				Server::run(void)
{


    int yes=1;

    if ((m_socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(m_socket_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    m_my_addr.sin_family = AF_INET;         	// host byte order
    m_my_addr.sin_port = htons(80);     		// short, network byte order
    m_my_addr.sin_addr.s_addr = INADDR_ANY; 	// automatically fill with my IP
    memset(&(m_my_addr.sin_zero), '\0', 8); 	// zero the rest of the struct

    if (bind(m_socket_fd, (struct sockaddr *)&m_my_addr, sizeof(struct sockaddr))
                                                                   == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(m_socket_fd, 10) == -1) {
        perror("listen");
        exit(1);
    }
    return 1;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
	int				Server::getSocketFd(void) const
    {
        return m_socket_fd;
    }


/* ************************************************************************** */
