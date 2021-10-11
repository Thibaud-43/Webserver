#ifndef SERVER_HPP
# define SERVER_HPP

# include "Headers.hpp"
# include "Location.hpp"
# include "Listener.hpp"
# include "Client.hpp"


class Server
{

	public:

		Server();
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );
		int				getSocketFd(void) const;
		int				run(void);

		std::string		getIp(void) const;
		std::string		getPort(void) const;



	private:
		std::string						m_ip;
		std::string						m_port;
		std::string						m_name;
		std::vector<Location>			m_locations;
};
class Listener;

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */