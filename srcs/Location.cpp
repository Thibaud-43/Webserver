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
	m_uri = "/";
	m_root = "/var/www/";
	m_autoindex = true;
	m_upload = false;
}

Location::Location( const Location & src )
	: m_error_pages(src.m_error_pages), m_body_size(src.m_body_size), m_root(src.m_root)
	, m_indexes(src.m_indexes), m_methods(src.m_methods), m_redirect(src.m_redirect)
	, m_autoindex(src.m_autoindex), m_upload(src.m_upload), m_cgi_pass(src.m_cgi_pass)
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

std::ostream &			operator<<( std::ostream & o, Location const & rhs )
{
	std::map<std::string, std::string> 				tmpErrPages = rhs.getErrPages();
	std::map<std::string, std::string> 				tmpCGI = rhs.getCGIPass();
	std::vector<std::string>						tmpIndexes = rhs.getIndexes();
	std::vector<std::string>						tmpMethods = rhs.getMethods();

	std::map<std::string, std::string>::iterator	it1;

	std::cout << std::endl;
	o << "\t\tUri: " << rhs.getUri() << std::endl;
	for (it1 = tmpErrPages.begin(); it1 != tmpErrPages.end(); it1++)
	{
		o << "\t\tErrorNum: " << it1->first << std::endl;
		o << "\t\tErrorPath: " << it1->second << std::endl;
	}
	o << "\t\tBody size: " << rhs.getBodySize() << std::endl;
	o << "\t\tRoot: " << rhs.getRoot() << std::endl;
	o << "\t\tIndexes: ";
	for (Location::indexes_t::const_iterator it = tmpIndexes.begin(); it != tmpIndexes.end(); it++)
	{
		o << *it << " ";
	}
	o << std::endl;
	o << "\t\tMethods: ";
	for (Location::methods_t::const_iterator it = tmpMethods.begin(); it != tmpMethods.end(); it++)
	{
		o << *it << " ";
	}
	o << std::endl;
	o << "\t\tRedirect: " << rhs.getRedirectNum() << "\t" <<rhs.getRedirectPath() <<std::endl;
	if (rhs.getAutoindex() == true)
		o << "\t\tAutoindex: on" << std::endl;
	else
		o << "\t\tAutoindex: off" << std::endl;
	if (rhs.getUpload() == true)
		o << "\t\tUpload: on" << std::endl;
	else
		o << "\t\tUpload: off" << std::endl;
	for (it1 = tmpCGI.begin(); it1 != tmpCGI.end(); it1++)
	{
		o << "\t\tCGI: " << it1->first << " " << it1->second << std::endl;
	}
	return o;
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

std::string const &				Location::getUri(void) const
{
	return (m_uri);
}

std::map<Location::err_code_t, Location::path_t> const &	Location::getErrPages(void) const
{
	return (m_error_pages);
}

Location::body_size_t const &	Location::getBodySize(void) const
{
	return (m_body_size);
}

Location::path_t const &		Location::getRoot(void) const
{
	return (m_root);
}

Location::indexes_t const &		Location::getIndexes(void) const
{
	return (m_indexes);
}

Location::methods_t const &		Location::getMethods(void) const
{
	return (m_methods);
}

std::string						Location::getStrMethods(void) const
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

bool							Location::getAutoindex(void) const
{
	return (m_autoindex);
}

Location::cgi_t const &			Location::getCGIPass(void) const
{
	return (m_cgi_pass);
}

bool							Location::getUpload(void) const
{
	return (m_upload);
}

void	Location::fillLocation(Node* node)
{
	if (node != NULL)
		this->setValue(node);
	if (node->getLeft() != NULL)
		this->fillLocation(node->getLeft());
}

void	Location::setValue(Node* node)
{
	std::string					type = node->getType();
	std::vector<std::string>	content = node->getContent();

	if (type == "error_page")
		this->setErrPages(content);
	else if (type == "client_max_body_size")
		this->setBodySize(content);
	else if (type == "root")
		this->setRoot(content);
	else if (type == "index")
		this->setIndexes(content);
	else if (type == "methods")
		this->setMethods(content);
	else if (type == "redirect")
		this->setRedirect(content);
	else if (type == "autoindex")
		this->setautoindex(content);
	else if (type == "upload")
		this->setUpload(content);
	else if (type == "cgi")
		this->setCGIPass(content);
}


void	Location::setUri(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	this->m_uri = *it;
}

void	Location::setErrPages(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	m_error_pages[*it] = *(it + 1);
}

void	Location::setBodySize(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator	it = content.begin();
	std::istringstream							iss(*it);
	size_t										i;

	iss >> i;
	this->m_body_size = i;
}

void	Location::setRoot(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	this->m_root = *it;
}

void	Location::setIndexes(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();
	std::vector<std::string>::const_iterator  ite = content.end();

	while (it != ite)
		this->m_indexes.push_back(*(it++));
}

void	Location::setMethods(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();
	std::vector<std::string>::const_iterator  ite = content.end();

	if (content.size() > 0)
		this->m_methods.clear();
	while (it != ite)
		this->m_methods.push_back(*(it++));
}

void	Location::setRedirect(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	m_redirect = std::make_pair(*it, *(it + 1));
}

void	Location::setautoindex(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	if (*it == "on")
		this->m_autoindex = true;
	else
		this->m_autoindex = false;
}

void	Location::setUpload(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	if (*it == "on")
		this->m_upload = true;
	else
		this->m_upload = false;
}

// WHAT S HAPPEN IF SAME MAP
void	Location::setCGIPass(std::vector<std::string> content)
{
	std::vector<std::string>::const_iterator  it = content.begin();

	m_cgi_pass[*it] = *(it + 1);
}


/* ************************************************************************** */