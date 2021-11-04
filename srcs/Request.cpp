#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request()
: Client(), m_server(NULL), m_location(NULL), m_path(std::string()), m_header(header_type())
{
}

Request::Request(const Request & src)
: Client(src), m_server(src.m_server), m_location(src.m_location), m_path(src.m_path), m_header(src.m_header)
{
}

Request::Request(const Client & src)
: Client(src), m_server(NULL), m_location(NULL), m_path(std::string()), m_header(header_type())
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Request::execute(ASocket *ptr)
{
	// FILL buffer
	// IF /r/n/r/n in m_buff -> PARSING HEADER
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */