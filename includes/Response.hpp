#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <map>
# include <ctime>
# include "Client.hpp"

# define PROTOCOL "HTTP/1.1"
# define SERV_NAME "Webserv/1.0.0"

class Response
{

	public:
		typedef	std::string									status_code_t;
		typedef std::string									status_phrase_t;
		typedef std::map<status_code_t, status_phrase_t>	status_t;

		Response();
		Response(Response const & src);
		~Response();

		std::string	getContent(void) const;
		void		start_header(status_code_t const & status);
		void		append_to_header(std::string const & str);
		void		append_to_body(std::string const & str);
		void		send_to_client(Client const & client) const;
		void		add_content_length(void);
		void		clear(void);

		Response &		operator=(Response const & rhs);

	private:
		static status_t	_status;
		std::string		m_header;
		std::string		m_body;
		
		static status_t	createStatus(void);
};

std::string	HTTPDate(void)
{
	char	buf[100];
	time_t	now = time(0);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now));
	return (std::string(buf));
}

std::string	OSName(void)
{
	#ifdef _WIN32
    return ("Windows 32-bit");
    #elif _WIN64
    return ("Windows 64-bit");
    #elif __APPLE__ || __MACH__
    return ("Mac OSX");
    #elif __linux__
    return ("Linux");
    #elif __FreeBSD__
    return ("FreeBSD");
    #elif __unix || __unix__
    return ("Unix");
    #else
    return ("Other");
    #endif
}

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */