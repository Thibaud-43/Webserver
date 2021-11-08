#ifndef ACGI_HPP
# define ACGI_HPP

# include "Headers.hpp"

class ASocket;

class ACgi
{
	public:
		typedef std::map<std::string, std::string>	env_type;

		ACgi();
		ACgi( ACgi const & src );
		~ACgi();
		
		int				getFdIn(void) const;
		int				getFdOut(void) const;
		pid_t			getPid(void) const;
		char			**getEnv(void) const;
		void			del_env(char ** env) const;
		void			clear(void);
		virtual bool	start(void) = 0;
		virtual bool	manage(ACgi ** ptr, int const & fd) = 0;
		virtual bool	checkStatus(void) = 0;

	protected:
		int			m_fd_in;
		int			m_fd_out;
		pid_t		m_pid;
		env_type	m_env;
		
		virtual void	_setEnv(void) = 0;
		void			_close_pipes(int const pipefd_out[2]);
		void			_close_pipes(int const pipefd_out[2], int const pipefd_in[2]);
};

#endif /* ************************************************************ ACGI_H */