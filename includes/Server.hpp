#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include "Location.hpp"

class Server
{

	public:

		Server();
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );

	private:
		std::string						m_ip;
		std::string						m_port;
		std::string						m_name;
		std::vector<Location>			m_locations;

};

#endif /* ********************************************************** SERVER_H */