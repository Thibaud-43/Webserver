#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Headers.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Location.hpp"

class Request: public Client
{

	public:

		Request();
		Request(Request const & src);
		Request(Client const & src);
		~Request();

		Request &		operator=( Request const & rhs );

	private:


};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */