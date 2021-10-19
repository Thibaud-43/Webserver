#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(): m_port("80"), m_ip("0.0.0.0")
{
	m_names.push_back("localhost");
}

Server::Server( const Server & src ): m_port(src.m_port), m_ip(src.m_ip), m_names(src.m_names), m_params(Location())
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
	if ( this != &rhs )
	{
		m_ip = rhs.m_ip;
		m_port = rhs.m_port;
		m_names = rhs.m_names;
		m_params = rhs.m_params;
		m_locations = rhs.m_locations;
	}
	return *this;
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

Location						Server::getParams(void) const
{
	return (m_params);
}

std::vector<Location>			Server::getLocations(void) const
{
	return (m_locations);
}

Location const *	Server::getLocation(std::string const & uri) const
{
	size_t									match = 0;
	std::vector<Location>::const_iterator	it_ret;

	for (std::vector<Location>::const_iterator it = m_locations.begin(); it != m_locations.end(); it++)
	{
		if (!uri.compare(0, it->getUri().size(), it->getUri()) && it->getUri().size() > match)
		{
			it_ret = it;
			match = it->getUri().size();
		}
	}
	return (match ? &(*it_ret) : &m_params);
}

/* ************************************************************************** */
