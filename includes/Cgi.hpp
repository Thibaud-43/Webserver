#ifndef CGI_HPP
# define CGI_HPP

# include "Headers.hpp"
# include "Client.hpp"
# include "Request.hpp"
# include "Server.hpp"
# include "Cluster.hpp"

/*		ENV LIST:

		AUTH_TYPE;
		CONTENT_LENGTH;
		CONTENT_TYPE;
		GATEWAY_INTERFACE;
		PATH_INFO;
		PATH_TRANSLATED;
		QUERY_STRING;
		REMOTE_ADDR;
		REQUEST_METHOD;
		SCRIPT_NAME;
		SERVER_NAME;
		SERVER_PORT;
		SERVER_PROTOCOL;
		SERVER_SOFTWARE;
*/

class Cgi
{
	public:
		typedef std::set<Cgi>						list_type;
		typedef int									fd_type;
		typedef std::map<std::string, std::string>	env_type;
		typedef struct epoll_event					event_type;

		Cgi();
		Cgi(Cgi const & src);
		Cgi(Request const & Request, std::string const & cgi_path);
		~Cgi();

		Cgi &				operator=(Cgi const & rhs);
		static bool			isCgiFd(fd_type & fd);
		static void			removeCgi(Cgi const & cgi);
		static Cgi const *	addCgi(Cgi const & cgi);
		static Cgi const *	getCgiFromFd(fd_type fd);
		static Cgi const *	getCgiFromClient(Client const * client);
		Client const *		getClient(void) const;
		int					getFd_out(void) const;
		pid_t				getPid(void) const;
		char **				getEnv(void) const;
		void				del_env(char **envp);
		bool				handle(std::string & buffer) const;

		bool				run(char const *cgi_path, char *const *args);
		bool				check_status(void) const;

		friend bool	operator<(Cgi const & lhs, Cgi const & rhs);
		friend bool	operator<=(Cgi const & lhs, Cgi const & rhs);
		friend bool	operator>=(Cgi const & lhs, Cgi const & rhs);
		friend bool	operator==(Cgi const & lhs, Cgi const & rhs);
		friend bool	operator!=(Cgi const & lhs, Cgi const & rhs);
		friend bool	operator>(Cgi const & lhs, Cgi const & rhs);

	private:
		pid_t   			m_pid;
		fd_type				m_fd_out;
		Client const *		m_client;
		env_type			m_env;

		static  list_type   _list;

};

#endif /* ********************************************************* Cgi_HPP */