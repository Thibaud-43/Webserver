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
		typedef std::list<Request>		list_type;

		//Request();
		Request(Request const & src);
		Request(std::string const request);
		Request(Client const * client);
		~Request();
		Request &			operator=(Request const & rhs);
		void				parse(std::string & buffer);
		void				execute(void);
		void				linkServer(std::vector<Server> & list);
		
		Client const *		getClient(void) const;
		static	Request *	getRequestFromClient(Client const & client);
		static	void		removeRequestFromClient(Client const & client);
		static	void		removeRequestFromRequest(Request const & request);

	private:
		std::map<std::string, std::string>	m_header;
		std::string							m_body;
		Client const *						m_client;							
		Server const *						m_server;
		Location const *					m_location;
		std::string							m_path;
		bool								m_headerCompleted;

		static list_type					_list;

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