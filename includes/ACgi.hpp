#ifndef ACGI_HPP
# define ACGI_HPP

# include "Headers.hpp"

class ACgi
{
	public:
		typedef std::set<ACgi const *>				list_type;
		typedef std::map<std::string, std::string>	env_type;

		ACgi();
		ACgi( ACgi const & src );
		~ACgi();
		
		int				getFdIn(void) const;
		int				getFdOut(void) const;
		char			**getEnv(void) const;
		void			del_env(char ** env) const;
		virtual bool	start(void) = 0;
		virtual bool	manage(int const & fd) const = 0;
		
		static ACgi const *	getCgi(int const & fd);
		static ACgi const *	addCgi(ACgi const * cgi);
		static void			removeCgi(ACgi const * cgi);

	protected:
		int			m_fd_in;
		int			m_fd_out;
		env_type	m_env;
		
		static	list_type	_list;
		
		virtual void	_setEnv(void) = 0;

};

#endif /* ************************************************************ ACGI_H */