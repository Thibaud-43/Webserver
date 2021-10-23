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

std::ostream &			operator<<( std::ostream & o, Server const & rhs )
{
	std::vector<std::string>	tmpNames = rhs.getNames();
	std::vector<Location>		tmpLocation = rhs.getLocations();

	o << "\tIP: " << rhs.getIp() << std::endl;
	o << "\tPort: " << rhs.getPort() << std::endl;
	o << "\tNames: ";
	for (std::vector<std::string>::const_iterator it = tmpNames.begin(); it != tmpNames.end(); it++)
	{
		o << *it << " ";
	}
	o << std::endl;
	o << rhs.getParams();
	std::cout << "Nb of location: " << tmpLocation.size() << std::endl;
	for (std::vector<Location>::const_iterator it = tmpLocation.begin(); it != tmpLocation.end(); it++)
	{
		o << "\t\tLOCATION: ";
		o << *it;
	}
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void			Server::fillServer(Node* node)
{
    Location  tmpLocation;

	if (node != NULL)
	{
		if (node->getType() == "location")
		{
			tmpLocation.setUri(node->getContent());
			tmpLocation.fillLocation(node->getLeft());
			this->m_locations.push_back(tmpLocation);
		}
		else if (node->getType() == "listen")
			this->setIpandPort(node->getContent());
		else if (node->getType() == "server_name")
			this->setNames(node->getContent());
		else
			this->setValue(node);
	}
	// std::cout << "1 UPLOAD: "<< m_params.getUpload() << std::endl;
	// std::cout << "1 AUTOINDEX: "<< m_params.getAutoindex() << std::endl;
	if (node->getLeft() != NULL)
		this->fillServer(node->getLeft());
	if (node->getRight() != NULL)
		this->fillServer(node->getRight());
}


int				Server::run(fd_type epoll)
{
    Listener	socket(epoll, m_port, m_ip);
    return 1;
}

void	Server::setValue(Node* node)
{
	std::string					type = node->getType();
	std::vector<std::string>	content = node->getContent();

	if (type == "error_page")
		m_params.setErrPages(content);
	else if (type == "client_max_body_size")
		m_params.setBodySize(content);
	else if (type == "root")
		m_params.setRoot(content);
	else if (type == "index")
		m_params.setIndexes(content);
	else if (type == "methods")
		m_params.setMethods(content);
	else if (type == "redirect")
		m_params.setRedirect(content);
	else if (type == "autoindex")
		m_params.setautoindex(content);
	else if (type == "upload")
		m_params.setUpload(content);
	else if (type == "cgi")
		m_params.setCGIPass(content);
}


// void	Server::setUri(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();

// 	m_params.serUri(*it);
// }

// void	Server::setErrPages(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();

// 	m_error_pages[*it] = *(it + 1);
// }

// void	Server::setBodySize(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator	it = content.begin();
// 	std::istringstream							iss(*it);
// 	size_t										i;

// 	iss >> i;
// 	this->m_body_size = i;
// }

// void	Server::setRoot(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();

// 	this->m_root = *it;
// }

// void	Server::setIndexes(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();
// 	std::vector<std::string>::const_iterator  ite = content.end();

// 	while (it != ite)
// 		this->m_indexes.push_back(*(it++));
// }

// void	Server::setMethods(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();
// 	std::vector<std::string>::const_iterator  ite = content.end();

// 	while (it != ite)
// 		this->m_methods.push_back(*(it++));
// }

// void	Server::setRedirect(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();

// 	m_redirect = std::make_pair(*it, *(it + 1));
// }

// void	Server::setautoindex(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();

// 	if (*it == "on")
// 		this->m_autoindex = true;
// 	else
// 		this->m_autoindex = false;
// }

// void	Server::setUpload(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();

// 	if (*it == "on")
// 		this->m_upload = true;
// 	else
// 		this->m_upload = false;
// }

// // WHAT S HAPPEN IF SAME MAP
// void	Server::setCGIPass(std::vector<std::string> const &content)
// {
// 	std::vector<std::string>::const_iterator  it = content.begin();

// 	m_cgi_pass[*it] = *(it + 1);
// }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
void	Server::setIpandPort(std::vector<std::string> const & content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	if (content.size() == 2)
	{
		m_ip = *it;
		m_port = *(it + 1);
	}
	else if ((*it).size() >= 7)
	{
		m_ip = *it;
		m_port = "80";
	}
	else
	{
		m_ip = "0.0.0.0";
		m_port = *it;
	}
}

void	Server::setNames(std::vector<std::string> const & content)
{
	std::vector<std::string>::const_iterator  it = content.begin();
	std::vector<std::string>::const_iterator  ite = content.end();

	while (it != ite)
		this->m_names.push_back(*(it++));
}



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

Location const &			Server::getParams(void) const
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
