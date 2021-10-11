#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(): m_port("80"), m_ip("0.0.0.0"), m_name("localhost")
{
}

Server::Server( const Server & src ): m_port(src.m_port), m_ip(src.m_ip), m_name(src.m_name)
{

}

Server::Server(std::string port, std::string ip, std::string name ): m_port(port), m_ip(ip), m_name(name)
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

int				Server::run(fd_type epoll)
{
    ASocket	*socket = new Listener(epoll, m_port, m_ip);  
    return 1;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


	std::string				Server::getIp(void) const
    {
        return m_ip;
    }
	std::string				Server::getPort(void) const
    {
        return m_port;
    }

	bool		Server::isServerFd(fd_type fd)
	{
		Listener *socket = dynamic_cast<Listener *>(ASocket::getASocketFromFd(fd));
		if (socket == NULL)
			return false;
		else
			return true;
		
	}
/* ************************************************************************** */
