#ifndef CLUSTER_HPP
# define CLUSTER_HPP

# include "Headers.hpp"

# include "Server.hpp"
# include "Request.hpp"
# include "ASocket.hpp"
# include "Client.hpp"

#define READ_SIZE 10000
#define MAX_EVENTS 10
#define MYPORT 3490
class Cluster
{

	public:
		typedef int						fd_type;
		typedef struct epoll_event		event_type;

		Cluster();
		Cluster( Cluster const & src );
		~Cluster();

		Cluster &						operator=( Cluster const & rhs );
		int								run(void);

	private:
		std::vector<Server>	m_servers;

		struct sockaddr_in 				m_their_addr;
		fd_type							m_newsocket_fd;
		fd_type							m_epoll_fd;
		event_type						m_events[MAX_EVENTS];
		int								m_eventCount;

		void							_createCluster(void);
		void							_createEpoll(void);
		void							_runServers(void);
		void							_epollWait(void);
		void							_epollExecute(void);
		void							_epollExecuteOnListenerConnection(fd_type & eventFd);
		void							_epollExecuteOnClientConnection(fd_type & eventFd);
		void							_closeEpoll(void);
};

std::ostream &			operator<<( std::ostream & o, Cluster const & i );

#endif /* ********************************************************* CLUSTER_H */