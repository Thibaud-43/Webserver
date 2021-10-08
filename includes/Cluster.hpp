#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Server.hpp"
# include "ASocket.hpp"
# include "Client.hpp"


# include <stdio.h>     // for fprintf()
# include <unistd.h>    // for close(), read()
# include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
# include <string.h>    // for strncmp
# include <stdlib.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

#define MAX_EVENTS 5
#define READ_SIZE 4096
#define MYPORT 3490                                                             // the port users will be connecting to
#define BACKLOG 10                                                              // how many pending connections queue will hold

class Cluster
{

	public:

		Cluster();
		Cluster( Cluster const & src );
		~Cluster();

		Cluster &		operator=( Cluster const & rhs );
		int				run(void);

	private:
		std::vector<Server>	m_servers;

    	struct sockaddr_in 				m_their_addr;
		int								m_newsocket_fd;

};

std::ostream &			operator<<( std::ostream & o, Cluster const & i );

#endif /* ********************************************************* CLUSTER_H */