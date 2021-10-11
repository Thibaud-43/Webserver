#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "Headers.hpp"

# include "Server.hpp"
# include "ASocket.hpp"
# include "Client.hpp"

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