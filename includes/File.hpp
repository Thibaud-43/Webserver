#ifndef FILE_HPP
# define FILE_HPP

# include "Headers.hpp"

class File
{
	public:
		typedef std::string		path_t;

		File();
		File( File const & src );
		File( path_t const & path );
		~File();

		File &				operator=( File const & rhs );
		std::string	const &	getPath(void) const;
		void				setPath(std::string const path);

		off_t				size(void) const;
		std::string			size_str(void) const;
		bool				exist(void) const;
		bool				is_regular(void) const;
		bool				is_directory(void) const;
		bool				is_readable(void) const;
		bool				is_writable(void) const;
		bool				is_executable(void) const;
		time_t				last_access(void) const;
		time_t				last_modification(void) const;
		std::string			last_modification_str(void) const;

		static std::vector<std::string>	currentFiles(path_t const & directory);

	private:
		path_t	m_path;

};

#endif /* ************************************************************ FILE_H */