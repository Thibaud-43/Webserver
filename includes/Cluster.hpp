#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Server.hpp"

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