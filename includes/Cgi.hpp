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

		Cgi();
		Cgi(Cgi const & src);
		Cgi(Request const &  Request);
		~Cgi();

		Cgi &						    operator=(Cgi const & rhs);
		static	bool					isCgiFd(fd_type & fd);
		static	void					removeCgi(Cgi const & cgi);
		Client const *					getClient(void) const;


	private:
		int     	m_pipe[2];
		pid_t   	m_pid;
		Client	*	m_client;


		static  list_type   _list;
};
#endif /* ********************************************************* Cgi_H */