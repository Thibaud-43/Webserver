#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include "Server.hpp"
# include "Client.hpp"

#define READ_SIZE 4096

class Request
{

	public:
		//Request();
		Request(Request const & src);
		Request(std::string const request);
		~Request();
		Request &		operator=(Request const & rhs);
		Request(std::string   buffer, Client & client);
		void			parse(void);
		std::string		execute(void) const;

	private:
		std::string							m_buffer;
		std::map<std::string, std::string>	m_header;
		std::string							m_body;
		Client				&				m_client;							
		Server								m_server;

		void								_parseRequestLine(void);				
		void								_parseHeaders(void);
		void								_parseBody(void);
		void								_parseLine(std::string & token);

		// DEBUG

		void								_printHeader(void);
		void								_printBody(void);
		void								_printHex(std::string & token);

};

std::ostream &			operator<<(std::ostream & o, Request const & i);

#endif /* ********************************************************* REQUEST_H */