#include "Unchunker.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Unchunker::Unchunker()
: m_chunk_size(-1), m_max_size(MAX_SERVER_BODY_SIZE)
{
}

Unchunker::Unchunker( const Unchunker & src )
: m_chunk_size(src.m_chunk_size), m_max_size(src.m_max_size)
{
}

Unchunker::Unchunker(size_t const & max_size)
: m_max_size(max_size) 
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Unchunker::~Unchunker()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

bool	Unchunker::operator()(std::string const & buffer)
{
	
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */