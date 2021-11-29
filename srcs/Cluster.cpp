#include "Cluster.hpp"
#include "ACgi.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cluster::Cluster(char const * configFilePath): m_tree(configFilePath), m_eventCount(0)
{
    try
    {
		m_tree.getTokens().createTokensList();
        m_tree.parseTokensList();
        this->_fillCluster(m_tree.getRoot());
		this->_parseServerObject();
    }
    catch(const Tree::ParserFailException& e)
    {
        std::cerr << "Error - ";
        std::cerr << e.what();
		std::cerr << m_tree.getRoot()->getErrorMessage();
		std::cerr << std::endl;
    }
	catch(const std::exception& e)
    {
        std::cerr << "Error - ";
        std::cerr << e.what();
		std::cerr << std::endl;
    }
}

Cluster::Cluster( const Cluster & src ): m_servers(src.m_servers), m_tree(src.m_tree), m_eventCount(src.m_eventCount)
{
	for (size_t i = 0; i < MAX_EVENTS; i++)
	{
		m_events[i].data.fd = src.m_events[i].data.fd;
		m_events[i].data.ptr = src.m_events[i].data.ptr;
		m_events[i].data.u32 = src.m_events[i].data.u32;
		m_events[i].data.u64 = src.m_events[i].data.u64;
		m_events[i].events = src.m_events[i].events;
	}
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cluster::~Cluster()
{
	std::vector<Server*>::iterator it = m_servers.begin();
	std::vector<Server*>::iterator ite = m_servers.end();

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

Cluster &				Cluster::operator=( Cluster const & rhs )
{
	if ( this != &rhs )
	{
		m_servers = rhs.m_servers;
		m_tree = rhs.m_tree;
		for (size_t i = 0; i < MAX_EVENTS; i++)
		{
			m_events[i].data.fd = rhs.m_events[i].data.fd;
			m_events[i].data.ptr = rhs.m_events[i].data.ptr;
			m_events[i].data.u32 = rhs.m_events[i].data.u32;
			m_events[i].data.u64 = rhs.m_events[i].data.u64;
			m_events[i].events = rhs.m_events[i].events;;
		}
		m_eventCount = rhs.m_eventCount;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Cluster const & rhs )
{
	std::vector<Server*> tmpServer = rhs.getServers();
	std::vector<Server*>::const_iterator it = tmpServer.begin();
	std::vector<Server*>::const_iterator ite = tmpServer.end();
	size_t i = 0;

	while (it != ite)
	{
		o << "Server " << i++ << std::endl;
		o << *(*it);
		it++;
	}
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
void			Cluster::_parseServerObject(void)
{
	if (m_servers.empty() == true)
		return;
	std::vector<Server*>::const_iterator it = m_servers.begin();
	std::vector<Server*>::const_iterator ite = m_servers.end();

	for (; it !=ite; it++)
	{
		if ( (*it)->getLocations().empty() == false )
		{
			std::vector<Location*> tmpLocationVec = (*it)->getLocations();
			std::vector<Location*>::const_iterator it1 = tmpLocationVec.begin();
			std::vector<Location*>::const_iterator ite1 = tmpLocationVec.end();
			for (; it1 !=ite1; it1++)
			{
				this->_inheritDefaultLocation(**it, **it1);
			}
		}
	}
}

void			Cluster::_inheritDefaultLocation(Server &server, Location &location)
{
	if (location.getErrPages().empty() == true)
		location.setErrPages(server.getParams().getErrPages());
	else
		location.getErrPages().insert(server.getParams().getErrPages().begin(), server.getParams().getErrPages().end()); 
	if (location.getBodySize() == static_cast<Location::body_size_t>(-1))
		location.setBodySize(server.getParams().getBodySize());
	if (location.getRoot().empty() == true)
		location.setRoot(server.getParams().getRoot());
	if (location.getIndexes().empty() == true)
		location.setIndexes(server.getParams().getIndexes());
	if (location.getMethods().empty() == true)
		location.setMethods(server.getParams().getMethods());
	if (location.getRedirectNum().empty() == true)
		location.setRedirect(server.getParams().getRedirect());
	if (location.getAutoindex() == 2)
		location.setAutoindex(server.getParams().getAutoindex());
	if (location.getUpload() == 2)
		location.setUpload(server.getParams().getUpload());
	if (location.getCGIPass().empty() == true)
		location.setCGIPass(server.getParams().getCGIPass());
	else
		location.getCGIPass().insert(server.getParams().getCGIPass().begin(), server.getParams().getCGIPass().end());
}

void			Cluster::_fillCluster(Node* node)
{
	Server  *tmpServer;
	std::vector<std::string>::iterator it = node->getContent().begin();

	if (node == NULL) 
		return ;
	if (node->getType() == "Server" && it != node->getContent().end() && *it == "empty")
	{
		tmpServer = new Server();
		m_servers.push_back(tmpServer);
		if (node->getLeft() != NULL)
			this->_fillCluster(node->getLeft());
		return;
	}
	if (node->getLeft() != NULL)
	{
		tmpServer = new Server();
		tmpServer->fillServer(node->getLeft());
		m_servers.push_back(tmpServer);
	}
	if (node->getRight() != NULL)
	{
		this->_fillCluster(node->getRight());
	}
}

int				Cluster::run(void)
{
	int running = 1;

	FileDescriptor::setEpollFd();
	_runServers();
	while(running)
	{
		ASocket::clean();
		_epollWait();
		_epollExecute();
	}
	_closeEpoll();
	return 0;
}

void							Cluster::_runServers(void)
{
	for (std::vector<Server*>::iterator i = m_servers.begin(); i != m_servers.end(); i++)
	{
		(*(*i)).run();
	}
}

void							Cluster::_epollWait(void)
{
	m_eventCount = epoll_wait(FileDescriptor::getEpollFd(), m_events, MAX_EVENTS, 2000);
	if (m_eventCount == -1)
	{
		perror("failed epoll_wait");
		throw std::exception();
	}
}

void							Cluster::_epollExecute(void)
{
	ASocket *socket;
	ACgi	*cgi;

	for( int i = 0; i < m_eventCount; i++)
	{
		socket = ASocket::getSocket(m_events[i].data.fd);
		if (socket)
		{
			socket->resetClock();
			if (m_events[i].events & EPOLLOUT)
			{
				if (!socket->send_rep(&socket))
					ASocket::removeSocket(socket);
			}
			else
			{
				if (!socket->execute(&socket))
					ASocket::removeSocket(socket);
			}
		}
		else if ((cgi = ASocket::getCgi(m_events[i].data.fd)))
		{

			if (!cgi->manage(&cgi, m_events[i].data.fd))
				ASocket::removeCgi(cgi);
		}
		else
			close(m_events[i].data.fd);
	}
}

void							Cluster::_closeEpoll(void)
{
	if(close(FileDescriptor::getEpollFd()))
	{
		std::cerr << "Failed to close epoll file descriptor\n" << std::endl;
		return ;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::vector<Server*> Cluster::getServers(void) const
{
	return m_servers;
}

Tree                Cluster::getTree(void) const
{
	return m_tree;
}
/* ************************************************************************** */