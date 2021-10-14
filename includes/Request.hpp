#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include "Server.hpp"
# include "Client.hpp"
# include "Response.hpp"


class Request
{

	public:
		//Request();
		Request(Request const & src);
		Request(std::string const request);
		Request(std::string buffer, Client const * client);
		~Request();
		Request &		operator=(Request const & rhs);
		void			parse(void);
		void			execute(void);
		void			linkServer(std::vector<Server> & list);

	private:
		std::string							m_buffer;
		std::map<std::string, std::string>	m_header;
		std::string							m_body;
		Client const *						m_client;							
		Server *							m_server;

		void								_parseRequestLine(void);				
		void								_parseHeaders(void);
		void								_parseBody(void);
		void								_parseLine(std::string & token);

		// DEBUG

		void								_printHeader(void);
		void								_printBody(void);
		void								_printHex(std::string & token);

};

std::ostream &								operator<<(std::ostream & o, Request const & i);

#endif /* ********************************************************* REQUEST_H */