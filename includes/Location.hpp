#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

class Location
{

	public:
		typedef short					err_code_t;
		typedef std::string				path_t;
		typedef std::string				file_t;
		typedef size_t					body_size_t;
		typedef short					redirect_t;

		Location();
		Location( Location const & src );
		~Location();


	private:
		std::map<err_code_t, path_t>	m_error_pages; // DEFAULT ?
		body_size_t						m_body_size; // DEFAULT ?
		path_t							m_root; // DEFAULT .
		file_t							m_index; // DEFAULT index.html
		std::vector<std::string>		m_methods; // DEFAULT GET POST DELETE
		redirect_t						m_redirect; // DEFAULT 0
		bool							m_autoindex; // DEFAULT true
		file_t							m_cgi_pass; // DEFAULT NONE
		path_t							m_upload_path; // DEFAULT ?
		
		Location &		operator=( Location const & rhs );


};

std::ostream &			operator<<( std::ostream & o, Location const & i );

#endif /* ******************************************************** LOCATION_H */