#include "Location.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Location::Location()
{
}

Location::Location( const Location & src )
	: m_error_pages(src.m_error_pages), m_body_size(src.m_body_size), m_root(src.m_root)
	, m_indexes(src.m_indexes), m_methods(src.m_methods), m_redirect(src.m_redirect)
	, m_autoindex(src.m_autoindex), m_cgi_pass(src.m_cgi_pass), m_upload_path(src.m_upload_path)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Location::~Location()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Location &				Location::operator=( Location const & rhs )
{
	if ( this != &rhs )
	{
		m_error_pages = rhs.m_error_pages;
		m_body_size = rhs.m_body_size;
		m_root = rhs.m_root;
		m_indexes = rhs.m_indexes;
		m_methods = rhs.m_methods;
		m_redirect = rhs.m_redirect;
		m_autoindex = rhs.m_autoindex;
		m_cgi_pass = rhs.m_cgi_pass;
		m_upload_path = rhs.m_upload_path;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::map<Location::err_code_t, Location::path_t> const &	Location::getErrPages(void) const
{
	return (m_error_pages);
}

Location::body_size_t const &			Location::getBodySize(void) const
{
	return (m_body_size);
}

Location::path_t const &				Location::getRoot(void) const
{
	return (m_root);
}

std::vector<Location::file_t> const &	Location::getIndexes(void) const
{
	return (m_indexes);
}

std::vector<std::string> const &		Location::getMethods(void) const
{
	return (m_methods);
}

Location::redirect_t const &			Location::getRedirect(void) const
{
	return (m_redirect);
}

bool									Location::autoindex(void) const
{
	return (m_autoindex);
}

Location::file_t const &				Location::getCGIPass(void) const
{
	return (m_cgi_pass);
}

Location::path_t const &				Location::getUploadPath(void) const
{
	return (m_upload_path);
}

/* ************************************************************************** */