#include "Location.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Location::Location()
{
	m_methods.push_back("GET");
	m_methods.push_back("POST");
	m_methods.push_back("DELETE");
	m_body_size = 1000000;
	m_root = ".";
	m_autoindex = true;
	m_upload = false;
}

Location::Location( const Location & src )
	: m_error_pages(src.m_error_pages), m_body_size(src.m_body_size), m_root(src.m_root)
	, m_indexes(src.m_indexes), m_methods(src.m_methods), m_redirect(src.m_redirect)
	, m_autoindex(src.m_autoindex), m_cgi_pass(src.m_cgi_pass), m_upload(src.m_upload)
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
		m_upload = rhs.m_upload;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Location::isAllowed(std::string const & method) const
{
	for (Location::methods_t::const_iterator it = m_methods.begin(); it != m_methods.end(); it++)
	{
		if (*it == method)
			return (true);
	}
	return (false);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string const &						Location::getUri(void) const
{
	return (m_uri);
}

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

Location::indexes_t const &	Location::getIndexes(void) const
{
	return (m_indexes);
}

Location::methods_t const &		Location::getMethods(void) const
{
	return (m_methods);
}

std::string								Location::getStrMethods(void) const
{
	std::string	str;

	for (std::vector<std::string>::const_iterator it = m_methods.begin(); it != m_methods.end(); it++)
	{
		str += *it;
		if (it + 1 != m_methods.end())
			str += " ";
	}
	return (str);
}

Location::redirect_t const &	Location::getRedirectNum(void) const
{
	return (m_redirect.first);
}

std::string const &	Location::getRedirectPath(void) const
{
	return (m_redirect.second);
}

bool									Location::autoindex(void) const
{
	return (m_autoindex);
}

Location::cgi_t const &				Location::getCGIPass(void) const
{
	return (m_cgi_pass);
}

bool									Location::getUpload(void) const
{
	return (m_upload);
}

/* ************************************************************************** */