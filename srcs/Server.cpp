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
    ASocket	*socket = new Listener(this);

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
/* ************************************************************************** */
