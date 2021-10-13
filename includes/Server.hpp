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
		Server(std::string port, std::string ip);
		~Server();

		Server &						operator=( Server const & rhs );
		int								getSocketFd(void) const;
		int								run(fd_type epoll);

		std::string						getIp(void) const;
		std::string						getPort(void) const;
		std::vector<std::string>		getNames(void) const;
		Location						getParams(void) const;
		std::vector<Location>			getLocations(void) const;					

	private:
		std::string						m_ip;
		std::string						m_port;
		std::vector<std::string>		m_names;
		Location						m_params;
		std::vector<Location>			m_locations;
};

#endif /* ********************************************************** SERVER_H */
