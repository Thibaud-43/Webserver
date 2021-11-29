#include "File.hpp"

std::vector<std::string>	File::currentFiles(File::path_t const & directory)
{
	DIR *dir;
	struct dirent *diread;
    std::vector<std::string> files;

    if ((dir = opendir(directory.data())))
	{
        while ((diread = readdir(dir)))
		{
			if (diread->d_name[0] != '.')
				files.push_back(diread->d_name);
		}
        closedir (dir);
    } 
	return (files);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

File::File()
{
}

File::File( const File & src ): m_path(src.m_path)
{
}

File::File(File::path_t const & path): m_path(path)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

File::~File()
{

}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

File &				File::operator=( File const & rhs )
{
	if ( this != &rhs )
		m_path = rhs.m_path;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	File::exist(void) const
{
	return (!access(m_path.data(), F_OK) ? true : false);
}

bool	File::is_readable(void) const
{
	return (!access(m_path.data(), R_OK) ? true : false);
}

bool	File::is_writable(void) const
{
	return (!access(m_path.data(), W_OK) ? true : false);
}

bool	File::is_executable(void) const
{
	return (!access(m_path.data(), X_OK) ? true : false);
}

bool	File::is_directory(void) const
{
	struct stat	s;

	if (stat(m_path.data(), &s) == 0)
		return (s.st_mode & S_IFDIR);
	return (false);
}

bool	File::is_regular(void) const
{
	struct stat	s;

	if (stat(m_path.data(), &s) == 0)
		return (s.st_mode & S_IFREG);
	return (false);
}

off_t	File::size(void) const
{
	struct stat	s;

	if (stat(m_path.data(), &s) == 0)
		return (s.st_size);
	return (-1);
}

std::string	File::size_str(void) const
{
	off_t				s = size();
	std::stringstream	ss;

	ss << s;
	return (ss.str());	
}

time_t	File::last_access(void) const
{
	struct stat	s;

	if (stat(m_path.data(), &s) == 0)
		return (s.st_atime);
	return (-1);
}

time_t	File::last_modification(void) const
{
	struct stat	s;

	if (stat(m_path.data(), &s) == 0)
		return (s.st_mtime);
	return (-1);
}

std::string	File::last_modification_str(void) const
{
	time_t	tm = last_modification();
	std::string	ret(ctime(&tm));

	ret.resize(ret.size() - 1);
	return (ret);	
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

File::path_t const &	File::getPath(void) const
{
	return (m_path);
}

void	File::setPath(std::string const path)
{
	m_path = path;
}

/* ************************************************************************** */