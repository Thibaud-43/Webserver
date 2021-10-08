#include "Cluster.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Cluster::Cluster()
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
	Server		test;

	socklen_t 	sin_size;
	int			sockfd;
	char		buff[4096];

	m_servers.push_back(test);
	for (std::vector<Server>::iterator i = m_servers.begin(); i != m_servers.end(); i++)
	{
		(*i).run();
		sockfd = (*i).getSocketFd();
	}



	int running = 1, event_count, i;
    size_t bytes_read;
    char read_buffer[READ_SIZE + 1];
    struct epoll_event event, events[MAX_EVENTS];

    int epoll_fd = epoll_create1(0);                                                // create a file descriptor to a new epoll instance
    int server_fd = sockfd;
    if(epoll_fd == -1)
    {
        fprintf(stderr, "Failed to create epoll file descriptor\n");
        return 1;
    }

    event.events = EPOLLIN | EPOLLET;                                               // The associated file is available for read(2) operations.
    event.data.fd = server_fd;
    
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event))                       // add the file descriptor 0 to our epoll instance epoll_fd, EPOLL_CTL_ADD add the poll to the instance
    {
        fprintf(stderr, "Failed to add file descriptor to epoll\n");
        close(epoll_fd);
        return 1;
    }


    struct sockaddr_in their_addr;
    while(running)
    {
        printf("\nPolling for input...\n");
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
        printf("%d ready events\n", event_count);
        for(i = 0; i < event_count; i++)
        {
            socklen_t size = sizeof(struct sockaddr);
            if (events[i].data.fd == server_fd)
            {

                int new_fd = accept(server_fd, (struct sockaddr*)&their_addr, &size);
				Client	socket(new_fd, their_addr, (ASocket::getASocketFromFd(server_fd))->getServer());
                event.data.fd = new_fd;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl (epoll_fd, EPOLL_CTL_ADD, new_fd, &event);
            }
            else
            {
                printf("Reading file descriptor '%d' -- ", events[i].data.fd);
                bytes_read = recvfrom(events[i].data.fd, read_buffer, sizeof(read_buffer), 0, (struct sockaddr*)&their_addr, &size);
                printf("%zd bytes read.\n", bytes_read);
                read_buffer[bytes_read] = '\0';
                printf("Read \n'%s'\n", read_buffer);
               
               
                send(events[i].data.fd, "HTTP/1.1 200 OK\r\n\r\nHello", 24, 0);


                close(events[i].data.fd);
            }
        }
    }
    if(close(epoll_fd))
    {
        fprintf(stderr, "Failed to close epoll file descriptor\n");
        return 1;
    }
    return 0;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */