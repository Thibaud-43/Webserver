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
    while(1) {  // main accept() loop
        sin_size = sizeof(struct sockaddr_in);
        if ((m_newsocket_fd = accept(sockfd, (struct sockaddr *)&m_their_addr,
                                                       &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n",
                                           inet_ntoa(m_their_addr.sin_addr));
        /*if (send(m_newsocket_fd, "Hello, world!\n", 14, 0) == -1)
            perror("send");*/
		if (recv(m_newsocket_fd, buff, sizeof(buff), 0) == -1)
            perror("send");
		std::cout << 
        close(m_newsocket_fd);
    }
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */