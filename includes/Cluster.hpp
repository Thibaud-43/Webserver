#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "Headers.hpp"
# include "Server.hpp"
# include "Tree.hpp"
# include "Node.hpp"
# include "Client.hpp"
//# include "Request.hpp"

class Cluster
{
	public:
		typedef int						fd_type;
		typedef struct epoll_event		event_type;

		Cluster( char const * configFilePath = "confFiles/webserv.conf" );
		Cluster( Cluster const & src );
		~Cluster();

		Cluster &						operator=( Cluster const & rhs );
		int								run(void);
		static fd_type					getEpollFd(void);
		std::vector<Server*> 			getServers(void) const;
		Tree				 			getTree(void) const;

	private:
		std::vector<Server*>			m_servers;
		Tree							m_tree;
		event_type						m_events[MAX_EVENTS];
		int								m_eventCount;

		void							_inheritDefaultLocation(Server &server, Location &location);
		void							_parseServerObject(void);
		void							_fillCluster(Node* node);
		void							_runServers(void);
		void							_epollWait(void);
		void							_epollExecute(void);
		void							_epollExecuteOnCgiConnection(fd_type & eventFd);
		void							_epollExecuteOnClientConnection(fd_type & eventFd);
		void							_closeEpoll(void);
};

std::ostream &			operator<<( std::ostream & o, Cluster const & rhs );

#endif
/* ********************************************************* CLUSTER_H */