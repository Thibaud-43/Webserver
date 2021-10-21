#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include <ctime>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fstream>
# include "Client.hpp"
# include "Location.hpp"
# include "File.hpp"

# define PROTOCOL "HTTP/1.1"
# define SERV_NAME "Webserv/1.0.0"

class Response
{

	public:
		typedef	std::string									status_code_t;
		typedef std::string									status_phrase_t;
		typedef std::string									file_t;
		typedef std::map<status_code_t, status_phrase_t>	status_t;

		Response();
		Response(Response const & src);
		~Response();

		std::string	getContent(void) const;
		void		start_header(status_code_t const & status);
		void		append_to_header(std::string const & str);
		void		append_to_body(std::string const & str);
		void		send_to_client(Client const * client) const;
		void		add_content_length(void);
		void		fill_body(file_t const & file);
		void		clear(void);

		Response &		operator=(Response const & rhs);
		static bool		send_error(status_code_t const & err, Client const * client, Location const * location);
		static bool		send_error(Response::status_code_t const & err, Client const * client);
		static bool		redirect(status_code_t const & red, std::string const & location, Client const * client);
		static bool		send_index(std::string const & directory, Client const * client, Location const * location);

	private:
		static status_t	_status;
		std::string		m_header;
		std::string		m_body;
		
		static status_t	_createStatus(void);
};

std::ostream &	operator<<( std::ostream & o, Response const & i );
std::string		HTTPDate(void);
std::string		OSName(void);

#endif /* ******************************************************** RESPONSE_H */