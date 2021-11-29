#include "Server.hpp"
#include "Response.hpp"
# include "Listener.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server():  m_ip("0.0.0.0"), m_port("80"), m_params(Location("default"))
{
	m_names.push_back("localhost");
}

Server::Server( const Server & src ): m_ip(src.m_ip), m_port(src.m_port), m_names(src.m_names), m_params(src.m_params)
{}

Server::Server(std::string port, std::string ip): m_ip(ip), m_port(port), m_params(Location("default"))
{
	m_names.push_back("localhost");
}
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
	std::vector<Location*>::iterator it = m_locations.begin();
	std::vector<Location*>::iterator ite = m_locations.end();

	while (it != ite)
	{
		if (*it != NULL)
			delete *it;
		it++;
	}
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

std::ostream &			operator<<( std::ostream & o, Server const & rhs )
{
	std::vector<std::string>	tmpNames = rhs.getNames();
	std::vector<Location*>		tmpLocation = rhs.getLocations();

	o << "\tIP: " << rhs.getIp() << std::endl;
	o << "\tPort: " << rhs.getPort() << std::endl;
	o << "\tNames: ";
	for (std::vector<std::string>::const_iterator it = tmpNames.begin(); it != tmpNames.end(); it++)
	{
		o << *it << " ";
	}
	o << std::endl;
	o << rhs.getParams();
	for (std::vector<Location*>::const_iterator it = tmpLocation.begin(); it != tmpLocation.end(); it++)
	{
		o << std::endl << "\t\tLOCATION: ";
		o << *(*it);
	}
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void			Server::fillServer(Node* node)
{
	Location  *tmpLocation;

	if (node != NULL)
	{
		if (node->getType() == "location")
		{
			tmpLocation = new Location();
			tmpLocation->fillUri(node->getContent());
			if (node->getLeft() != NULL)
				tmpLocation->fillLocation(node->getLeft());
			this->m_locations.push_back(tmpLocation);
		}
		else if (node->getType() == "listen")
			this->setIpandPort(node->getContent());
		else if (node->getType() == "server_name")
			this->setNames(node->getContent());
		else
		{
			this->m_params.fillValue(node);
		}
	}
	if (node->getLeft() != NULL && node->getType() != "location")
		this->fillServer(node->getLeft());
	if (node->getRight() != NULL)
		this->fillServer(node->getRight());
}


int				Server::run()
{
	int yes=1;
	int fd;
	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
	{
        perror("socket");
        throw std::exception();
    }
    if (setsockopt(fd, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) 
	{
        perror("setsockopt");
        throw std::exception();
    }
	std::cout << "[" << HTTPDate() << "]: server " << fd << " created" << std::endl;
    Listener	*socket = new Listener(fd, this, m_port, m_ip);
	ASocket::addSocket(socket);
    return 1;
}



/*
** --------------------------------- ACCESSOR ---------------------------------
*/
void						Server::setIpandPort(std::vector<std::string> const & content)
{
	std::vector<std::string>::const_iterator	it = content.begin();
	std::istringstream							iss(*it);
	std::string									buffer;

	m_ip = "0.0.0.0";
	m_port = "80";
	while (getline(iss, buffer, ':'))
	{
		if (buffer.size() >= 7)
			m_ip = buffer;
		else
			m_port = buffer;
	}
}

void						Server::setNames(std::vector<std::string> const & content)
{
	std::vector<std::string>::const_iterator  it = content.begin();
	std::vector<std::string>::const_iterator  ite = content.end();

	while (it != ite)
		this->m_names.push_back(*(it++));
}

std::string					Server::getIp(void) const
{
	return m_ip;
}

std::string					Server::getPort(void) const
{
	return m_port;
}

std::vector<std::string>	Server::getNames(void) const
{
	return m_names;
}

Location const &			Server::getParams(void) const
{
	return (m_params);
}

std::vector<Location*>		Server::getLocations(void) const
{
	return (m_locations);
}

Location const *			Server::getLocation(std::string const & uri) const
{
	size_t									match = 0;
	std::vector<Location*>::const_iterator	it_ret;

	for (std::vector<Location*>::const_iterator it = m_locations.begin(); it != m_locations.end(); it++)
	{
		if (!uri.compare(0, (*it)->getUri().size(), (*it)->getUri()) && (*it)->getUri().size() > match)
		{
			it_ret = it;
			match = (*it)->getUri().size();
		}
	}
	return (match ? &(*(*it_ret)) : &m_params);
}

/* ************************************************************************** */
