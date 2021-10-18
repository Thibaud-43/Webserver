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
    m_eventCount = epoll_wait(m_epoll_fd, m_events, MAX_EVENTS, 30000);
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
				// we processed all of the connections
				break;
			} 
			else 
			{
				perror("accept()");
				return ;
			}
		}
		else
        	Client	socket(client, their_addr, m_epoll_fd);
    }
    
}

void							Cluster::_epollExecuteOnClientConnection(fd_type & eventFd)
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
            Client const 	*client = Client::getClientFromFd(eventFd);
            Request			*request = Request::getRequestFromClient(*client);

            request->parse(buff);
            //request->linkServer(m_servers);
            // EXECUTION
            break;
        }
    }
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