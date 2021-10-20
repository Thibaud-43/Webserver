#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

class Location
{

	public:
		typedef std::string					err_code_t;
		typedef std::string					path_t;
		typedef std::string					file_t;
		typedef size_t						body_size_t;
		typedef std::string					redirect_t;
		typedef std::vector<std::string>	methods_t;

		Location();
		Location( Location const & src );
		~Location();

		Location &								operator=( Location const & rhs );

		std::string const &							getUri(void) const;
		std::map<err_code_t, path_t> const &		getErrPages(void) const;
		body_size_t const &							getBodySize(void) const;
		path_t const &								getRoot(void) const;
		std::vector<file_t> const &					getIndexes(void) const;
		methods_t const &							getMethods(void) const;
		std::string									getStrMethods(void) const;
		redirect_t const &							getRedirectNum(void) const;
		std::string const &							getRedirectPath(void) const;
		bool										autoindex(void) const;
		file_t const &								getCGIPass(void) const;
		bool										getUpload(void) const;
		bool										isAllowed(std::string const & method) const;

	private:
		std::string							m_uri; // DEFAULT empty
		std::map<err_code_t, path_t>		m_error_pages; // DEFAULT empty-empty
		body_size_t							m_body_size; // DEFAULT 1 000 000
		path_t								m_root; // DEFAULT .
		std::vector<file_t>					m_indexes; // DEFAULT index.htm
		methods_t							m_methods; // DEFAULT GET HEAD POST DELETE PUT
		std::pair<redirect_t, std::string>	m_redirect; // DEFAULT empty-empty
		bool								m_autoindex; // DEFAULT true
		file_t								m_cgi_pass; // DEFAULT NONE
		bool								m_upload; // DEFAULT false. (on/off)
};

#endif /* ******************************************************** LOCATION_H */