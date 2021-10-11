#ifndef SERVER_HPP
# define SERVER_HPP

# include "Headers.hpp"
# include "Location.hpp"
# include "Listener.hpp"
# include "Client.hpp"

# define PROTOCOL "HTTP/1.1"
class Server
{

	public:
        typedef int						fd_type;

		Server();
		Server( Server const & src );
		Server(std::string port, std::string ip, std::string name );
		~Server();

		Server &		operator=( Server const & rhs );
		int				getSocketFd(void) const;
		int				run(fd_type epoll);

		std::string		getIp(void) const;
		std::string		getPort(void) const;

		static bool		isServerFd(fd_type fd);

	private:
		std::string						m_ip;
		std::string						m_port;
		std::string						m_name;
		Location				m_params;
		std::vector<Location>			m_locations;
};
class Listener;

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
