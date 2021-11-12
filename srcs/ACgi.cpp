#include "ACgi.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ACgi::ACgi()
: m_fd_in(-1), m_fd_out(-1), m_pid(-1), m_env(env_type())
{
}

ACgi::ACgi( const ACgi & src )
: m_fd_in(src.m_fd_in), m_fd_out(src.m_fd_out), m_pid(src.m_pid), m_env(src.m_env)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ACgi::~ACgi()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void	ACgi::_close_pipes(int const pipefd_out[2])
{
	close(pipefd_out[0]);
	close(pipefd_out[1]);
}

void	ACgi::_close_pipes(int const pipefd_out[2], int const pipefd_in[2])
{
	close(pipefd_out[0]);
	close(pipefd_in[0]);
	close(pipefd_out[1]);
	close(pipefd_in[1]);
}

void	ACgi::del_env(char ** env) const
{
	size_t	i = 0;

	if (env)
	{	
		while (env[i])
		{
			delete [] env[i];
			i++;
		}
		delete [] env;
	}
}

void	ACgi::clear(void)
{
	if (m_pid >= 0)
		kill(m_pid, 0);
	if (m_fd_in >= 0)
		close(m_fd_in);
	if (m_fd_out >= 0)
		close(m_fd_out);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int		ACgi::getFdIn(void) const
{
	return (m_fd_in);
}

int		ACgi::getFdOut(void) const
{
	return (m_fd_out);
}

pid_t	ACgi::getPid(void) const
{
	return (m_pid);
}

char	**ACgi::getEnv(void) const
{
	char		**env = new char*[m_env.size() + 1];
	size_t		i = 0;
	std::string	buff;

	for (env_type::const_iterator it = m_env.begin(); it != m_env.end(); it++)
	{
		buff = it->first + "=" + it->second;
		env[i] = new char[buff.size() + 1];
		buff.copy(env[i], buff.size());
		env[i][buff.size()] = 0;
		i++;
	}
	env[i] = 0;
	return (env);
}


/* ************************************************************************** */