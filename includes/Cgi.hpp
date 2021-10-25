#ifndef CGI_HPP
# define CGI_HPP

#include "Headers.hpp"
#include "Request.hpp"
#include "Client.hpp"

class Cgi
{
	public:
		typedef std::set<Cgi>		list_type;
		typedef int					fd_type;
		typedef	std::string			envVariable_t;

		Cgi();
		Cgi(Cgi const & src);
		Cgi(Request const &  Request);
		~Cgi();

		Cgi &						    operator=(Cgi const & rhs);
		static	bool					isCgiFd(fd_type & fd);
		static Cgi const *				getCgiFromFd(fd_type fd);
		static	void					removeCgi(Cgi const & cgi);
		Client const *					getClient(void) const;
		bool							handle(std::string & buffer) const;


	private:
		int     		m_pipe[2];
		pid_t   		m_pid;
		Client	*		m_client;

		char			**environement;
		
		envVariable_t	CONTENT_LENGTH;
		envVariable_t	CONTENT_TYPE;
		envVariable_t	GATEWAY_INTERFACE;
		envVariable_t	PATH_INFO;
		envVariable_t	PATH_TRANSLATED;
		envVariable_t	QUERY_STRING;
		envVariable_t	REMOTE_ADDR;
		envVariable_t	REQUEST_METHOD;
		envVariable_t	SCRIPT_NAME;
		envVariable_t	SERVER_NAME;
		envVariable_t	SERVER_PORT;
		envVariable_t	SERVER_PROTOCOL;
		envVariable_t	SERVER_SOFTWARE;
		envVariable_t	REDIRECT_STATUS;


		static  list_type   _list;
};
#endif /* ********************************************************* Cgi_H */