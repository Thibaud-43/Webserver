#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(): m_port("80"), m_ip("0.0.0.0")
{
	m_names.push_back("localhost");
}

Server::Server( const Server & src ): m_port(src.m_port), m_ip(src.m_ip), m_names(src.m_names)
{

}

Server::Server(std::string port, std::string ip): m_port(port), m_ip(ip)
{
	m_names.push_back("localhost");
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
    Listener	socket(epoll, m_port, m_ip);
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
	std::vector<std::string>	Server::getNames(void) const
	{
		return m_names;
	}


/* ************************************************************************** */
