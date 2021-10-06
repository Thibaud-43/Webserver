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

};

#endif /* ********************************************************* CLUSTER_H */