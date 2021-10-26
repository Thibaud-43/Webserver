#include "Cluster.hpp"

Cluster::fd_type Cluster::_epoll_fd = -1;

Cluster::fd_type	Cluster::getEpollFd(void)
{
	return _epoll_fd;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cluster::Cluster(char const * configFilePath): m_tree(configFilePath), m_eventCount(0)
{
    try
    {
		m_tree.getTokens().createTokensList();
        m_tree.parseTokensList();
		// printBT(m_tree.getRoot());
        this->_fillCluster(m_tree.getRoot());
		this->_parseServerObject();
        std::cout << *this << std::endl;
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

	_createCluster();
	_createEpoll();
	_runServers();
	while(running)
	{
		_epollWait();
		_epollExecute();
	}
	_closeEpoll();
	return 0;
}

void							Cluster::_createEpoll(void)
{
	_epoll_fd = epoll_create1(0);
	if(_epoll_fd < 0)
	{
		std::cerr << strerror(errno) << "    " << "Failed to create epoll file descriptor\n";
		return ;
	}
}

void							Cluster::_createCluster(void)
{
	Server		test("100", "0.0.0.0");
	Server      test2("90", "0.0.0.0");
	Server      test3("80", "0.0.0.0");

	// m_servers.push_back(test);
	// m_servers.push_back(test2);
	// m_servers.push_back(test3);
}

void							Cluster::_runServers(void)
{
	for (std::vector<Server*>::iterator i = m_servers.begin(); i != m_servers.end(); i++)
	{
		(*(*i)).run(_epoll_fd);
	}
}

void							Cluster::_epollWait(void)
{
	m_eventCount = epoll_wait(_epoll_fd, m_events, MAX_EVENTS, 30000);
	if (m_eventCount == -1)
	{
		std::cerr <<  "Failed epoll_wait\n";
		return ;
	}
}

void							Cluster::_epollExecute(void)
{
	for( int i = 0; i < m_eventCount; i++)
	{
		if (Listener::isListenerFd(m_events[i].data.fd))
		{
			_epollExecuteOnListenerConnection(m_events[i].data.fd);
		}
		else if (Cgi::isCgiFd(m_events[i].data.fd))
		{
			_epollExecuteOnCgiConnection(m_events[i].data.fd);
		}
		else
		{
			_epollExecuteOnClientConnection(m_events[i].data.fd);
		}
	}
}

void							Cluster::_epollExecuteOnListenerConnection(fd_type & eventFd)
{
	for (;;)
	{
		struct sockaddr_in their_addr;
		socklen_t size = sizeof(struct sockaddr);

		int client = accept(eventFd, (struct sockaddr*)&their_addr, &size);
		if (client == -1) 
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK) 
			{
				break;
			} 
			else 
			{
				perror("accept()");
				return ;
			}
		}
		else
			Client	socket(client, their_addr, _epoll_fd);
	}
	
}

void							Cluster::_epollExecuteOnCgiConnection(fd_type & eventFd)
{
	size_t              bytes_read;
	char                read_buffer[READ_SIZE + 1];
	size_t              read_buffer_size = sizeof(read_buffer);
	std::string         buff = "";


	for (;;)
	{
		memset(read_buffer, 0, read_buffer_size);
		bytes_read = recv(eventFd, read_buffer, read_buffer_size, 0);
		if (bytes_read < 0)
		{
			Cgi const 	*cgi = Cgi::getCgiFromFd(eventFd);
			Cgi::removeCgi(*cgi);
			close(eventFd);
			break;
		}
		else if (bytes_read == read_buffer_size)
		{
			read_buffer[bytes_read] = 0;
			buff += read_buffer;
		}
		else
		{ 
			read_buffer[bytes_read] = 0;

			buff += read_buffer;

			Cgi const 	*cgi = Cgi::getCgiFromFd(eventFd);
			if (cgi && cgi->handle(buff))
				Cgi::removeCgi(*cgi);
			break;
		}
	}
}

void							Cluster::_epollExecuteOnClientConnection(fd_type & eventFd)
{
	size_t              bytes_read;
	char                read_buffer[READ_SIZE + 1];
	size_t              read_buffer_size = sizeof(read_buffer);
	std::string         buff = "";

	Client const 	*client = Client::getClientFromFd(eventFd);
	Request			*request = Request::getRequestFromClient(*client);
	Cgi const		*cgi = Cgi::getCgiFromClient(client);

	if (cgi)
	{
		
	}

	for (;;)
	{
		memset(read_buffer, 0, read_buffer_size);
		bytes_read = recv(eventFd, read_buffer, read_buffer_size, 0);
		if (bytes_read < 0)
		{
			Request::removeRequest(*request);
			close(eventFd);
			break;
		}
		else if (bytes_read == read_buffer_size)
		{
			read_buffer[bytes_read] = 0;
			buff += read_buffer;
		}
		else
		{ 
			read_buffer[bytes_read] = 0;

			buff += read_buffer;

			if (!request && buff != "\r\n")
				request = Request::createRequest(*client);
			if (request && request->manage(buff, m_servers))
				 Request::removeRequest(*request);
			break;
		}
	}
}

void							Cluster::_closeEpoll(void)
{
	if(close(_epoll_fd))
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