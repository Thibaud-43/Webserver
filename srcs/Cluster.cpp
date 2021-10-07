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
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */