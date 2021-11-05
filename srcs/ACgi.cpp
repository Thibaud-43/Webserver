#include "ACgi.hpp"

ACgi::list_type ACgi::_list = ACgi::list_type();

ACgi const	*	ACgi::getCgi(int const & fd)
{
	for (list_type::const_iterator it = _list.begin(); it != _list.end(); it++)
	{
		if (fd == (*it)->getFdIn() || fd == (*it)->getFdOut())
			return (*it);
	}
	return (NULL);
}

ACgi const *	ACgi::addCgi(ACgi const * cgi)
{
	return (*_list.insert(cgi).first);
}

void	ACgi::removeCgi(ACgi const * cgi)
{
	if (cgi->getFdIn() >= 0)
		close(cgi->getFdIn());
	if (cgi->getFdOut() >= 0)
		close(cgi->getFdOut());
	_list.erase(cgi);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ACgi::ACgi()
: m_fd_in(-1), m_fd_out(-1), m_env(env_type())
{
}

ACgi::ACgi( const ACgi & src )
: m_fd_in(src.m_fd_in), m_fd_out(src.m_fd_out), m_env(src.m_env)
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

/* ************************************************************************** */