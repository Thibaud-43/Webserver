#include "Cluster.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cluster::Cluster(): m_eventCount(0)
{
}

Cluster::Cluster( const Cluster & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Cluster::~Cluster()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Cluster &				Cluster::operator=( Cluster const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Cluster const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

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
    m_epoll_fd = epoll_create1(0);
    if(m_epoll_fd < 0)
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

	m_servers.push_back(test);
	m_servers.push_back(test2);
	m_servers.push_back(test3);
}

void							Cluster::_runServers(void)
{
	for (std::vector<Server>::iterator i = m_servers.begin(); i != m_servers.end(); i++)
	{
		(*i).run(m_epoll_fd);
	}
}

void							Cluster::_epollWait(void)
{
    //printf("\nPolling for input...\n");
    m_eventCount = epoll_wait(m_epoll_fd, m_events, MAX_EVENTS, 30000);
    if (m_eventCount == -1)
    {
        std::cerr <<  "Failed epoll_wait\n";
        return ;
    }
    //std::cerr << m_eventCount << " ready events\n";

}

void							Cluster::_epollExecute(void)
{
    for( int i = 0; i < m_eventCount; i++)
    {
        if (Server::isServerFd(m_events[i].data.fd))
        {
            _epollExecuteOnListenerConnection(m_events[i].data.fd);
        }
        else
        {
            _epollExecuteOnClientConnection(m_events[i].data.fd);
        }
    }
}

void							Cluster::_epollExecuteOnListenerConnection(fd_type & eventFd)
{
    struct sockaddr_in their_addr;
    socklen_t size = sizeof(struct sockaddr);

    int client = accept(eventFd, (struct sockaddr*)&their_addr, &size);
    Client	*socket = new Client(client, their_addr, m_epoll_fd);
}

void							Cluster::_epollExecuteOnClientConnection(fd_type & eventFd)
{
    struct sockaddr_in  their_addr;
    socklen_t           size = sizeof(struct sockaddr);
    size_t              bytes_read;
    char                read_buffer[READ_SIZE + 1];

    //std::cerr << "Reading file descriptor " << eventFd << std::endl;
    bytes_read = recvfrom(eventFd, read_buffer, sizeof(read_buffer), 0, (struct sockaddr*)&their_addr, &size);
    read_buffer[bytes_read] = '\0';

    Client  *client = dynamic_cast<Client *> (ASocket::getASocketFromFd(eventFd));

    Request             request(read_buffer, *client);
    request.parse();

    request.execute();


	delete client;
    close(eventFd);
}

void							Cluster::_closeEpoll(void)
{
    if(close(m_epoll_fd))
    {
        std::cerr << "Failed to close epoll file descriptor\n" << std::endl;
        return ;
    }
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */