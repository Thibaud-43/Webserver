#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include "Server.hpp"

class Request
{

	public:

		Request();
		Request( Request const & src );
		Request(std::string const request);
		~Request();
		Request &		operator=( Request const & rhs );

		std::string		execute(Server const & server) const;

	private:
		std::map<std::string, std::string>	m_header;
		std::string							m_body;

};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */