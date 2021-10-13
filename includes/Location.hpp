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

		Location &								operator=( Location const & rhs );

		std::map<err_code_t, path_t> const &	getErrPages(void) const;
		body_size_t const &						getBodySize(void) const;
		path_t const &							getRoot(void) const;
		std::vector<file_t> const &				getIndexes(void) const;
		std::vector<std::string> const &		getMethods(void) const;
		redirect_t const &						getRedirect(void) const;
		bool									autoindex(void) const;
		file_t const &							getCGIPass(void) const;
		path_t const &							getUploadPath(void) const;

	private:
		std::map<err_code_t, path_t>	m_error_pages; // DEFAULT ?
		body_size_t						m_body_size; // DEFAULT ?
		path_t							m_root; // DEFAULT .
		std::vector<file_t>				m_indexes; // DEFAULT index.html
		std::vector<std::string>		m_methods; // DEFAULT GET POST DELETE
		redirect_t						m_redirect; // DEFAULT 0
		bool							m_autoindex; // DEFAULT true

		// SEE LATER
		file_t							m_cgi_pass; // DEFAULT NONE
		path_t							m_upload_path; // DEFAULT ?


};

#endif /* ******************************************************** LOCATION_H */