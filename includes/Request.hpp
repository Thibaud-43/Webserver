#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <string>
# include <map>
# include "Server.hpp"
# include "Client.hpp"
# include "Response.hpp"
# include "Location.hpp"

class Request
{

	public:
		typedef std::list<Request>					list_type;
		typedef std::map<std::string, std::string>	header_type;
		typedef std::string							body_type;

		//Request();
		Request(Request const & src);
		Request(std::string const request);
		Request(Client const * client);
		~Request();

		Request &			operator=(Request const & rhs);
		bool				manage(std::string & buffer, std::vector<Server> const & servers);
		bool				execute(void);
		Client const *		getClient(void) const;
		bool				ready(void) const;
		
		static	Request *	getRequestFromClient(Client const & client);
		static	void		removeRequest(Request const & request);
		static void			checkRequestAdvancement(Request & request);
		header_type &		getHeader(void) ;
		body_type  &		getBody(void) ;

	private:
		header_type							m_header;
		std::string							m_body;
		Client const *						m_client;							
		Server const *						m_server;
		Location const *					m_location;
		std::string							m_path;
		bool								m_headerCompleted;
		bool								m_end;

		static list_type					_list;
		void								_linkServer(std::vector<Server> const & list);
		bool								_check_header(void);
		void								_parseRequestLine(std::string & buffer);				
		void								_parseHeaders(std::string & buffer);
		void								_parseBody(std::string & buffer);
		void								_parseLine(std::string & token);

		// DEBUG

		void								_printHeader(void);
		void								_printBody(void);
		void								_printHex(std::string & token);

};

std::ostream &								operator<<(std::ostream & o, Request const & i);

#endif /* ********************************************************* REQUEST_H */