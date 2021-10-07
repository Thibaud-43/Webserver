#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/wait.h>
# include <signal.h>
# include "Location.hpp"

# define PROTOCOL "HTTP/1.1"

class Server
{

	public:
		Server();
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );
		int				run(void);

	private:
		std::string						m_ip;
		std::string						m_port;
		std::string						m_name;
		Location				m_params;
		std::vector<Location>			m_locations;

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
