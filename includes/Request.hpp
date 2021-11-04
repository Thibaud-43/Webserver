#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Headers.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "Response.hpp"

class Request: public Client
{

	public:
		typedef std::map<std::string, std::string>	header_type;

		Request();
		Request(Request const & src);
		Request(Client const & src);
		~Request();

		virtual bool	execute(ASocket *ptr);

	protected:
		Server		*m_server;
		Location	*m_location;
		std::string	m_path;
		header_type	m_header;

};

#endif /* ********************************************************* REQUEST_H */