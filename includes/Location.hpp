#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Headers.hpp"
# include "Node.hpp"

class Location
{

	public:
		typedef std::string						err_code_t;
		typedef std::string						path_t;
		typedef std::string						file_t;
		typedef size_t							body_size_t;
		typedef std::string						redirect_t;
		typedef std::vector<std::string>		methods_t;
		typedef std::vector<std::string>		indexes_t;
		typedef std::map<std::string, path_t>	cgi_t;

		Location();
		Location(std::string const &flag);
		Location(Location const & src);
		~Location();

		Location &								operator=( Location const & rhs );

		std::string const &							getUri(void) const;
		std::map<err_code_t, path_t> const &		getErrPages(void) const;
		std::map<err_code_t, path_t> &				getErrPages(void);
		body_size_t const &							getBodySize(void) const;
		path_t const &								getRoot(void) const;
		indexes_t const &							getIndexes(void) const;
		methods_t const &							getMethods(void) const;
		std::string									getStrMethods(void) const;
		std::pair<redirect_t, std::string> const &	getRedirect(void) const;
		redirect_t const &							getRedirectNum(void) const;
		std::string const &							getRedirectPath(void) const;
		int											getAutoindex(void) const;
		int											getUpload(void) const;
		cgi_t const &								getCGIPass(void) const;
		cgi_t &										getCGIPass(void);
		bool										isAllowed(std::string const & method) const;
		
		void	setUri(std::string const &content);
		void	setErrPages(std::map<err_code_t, path_t> const &content);
		void	setBodySize(body_size_t const &content);
		void	setRoot(path_t const &content);
		void	setIndexes(indexes_t const &content);
		void	setMethods(methods_t const &content);
		void	setRedirect(std::pair<redirect_t, std::string> const &content);
		void	setAutoindex(int const &content);
		void	setUpload(int const &content);
		void	setCGIPass(cgi_t const &content);


		void	fillValue(Node* node);
		void	fillUri(std::vector<std::string> content);
		void	fillErrPages(std::vector<std::string> content);
		void	fillBodySize(std::vector<std::string> content);
		void	fillRoot(std::vector<std::string> content);
		void	fillIndexes(std::vector<std::string> content);
		void	fillMethods(std::vector<std::string> content);
		void	fillRedirect(std::vector<std::string> content);
		void	fillAutoindex(std::vector<std::string> content);
		void	fillUpload(std::vector<std::string> content);
		void	fillCGIPass(std::vector<std::string> content);


		void	fillLocation(Node* node);

	private:
		std::string							m_uri; // DEFAULT empty
		std::map<err_code_t, path_t>		m_error_pages; // DEFAULT empty-empty
		body_size_t							m_body_size; // DEFAULT MAX_SERVER_BODY_SIZE
		path_t								m_root; // DEFAULT .
		std::vector<file_t>					m_indexes; // DEFAULT index.htm
		methods_t							m_methods; // DEFAULT GET HEAD POST DELETE PUT
		std::pair<redirect_t, std::string>	m_redirect; // DEFAULT empty-empty
		int									m_autoindex; // DEFAULT true
		int									m_upload; // DEFAULT false. (on/off)
		cgi_t								m_cgi_pass; // DEFAULT NONE
};

std::ostream &			operator<<( std::ostream & o, Location const & rhs );

#endif /* ******************************************************** LOCATION_H */