#include "Unchunker.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Unchunker::Unchunker()
: m_chunk_size("-1"), m_total_size(0), m_max_size(MAX_SERVER_BODY_SIZE), m_end(false)
{
}

Unchunker::Unchunker( const Unchunker & src )
: m_chunk_size(src.m_chunk_size), m_total_size(src.m_total_size), m_max_size(src.m_max_size), m_end(false)
{
}

Unchunker::Unchunker(size_t const & max_size)
: m_chunk_size("-1"), m_total_size(0), m_max_size(max_size), m_end(false)
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

bool	Unchunker::operator()(std::string & buffer, std::string & body)
{
	std::string		delimiter = "\r\n";
	size_t			pos;
	std::string		token;

	while (!m_end)
	{
		if (m_chunk_size == "-1")
		{
			pos = buffer.find(delimiter);
			if (pos == std::string::npos)
				return true ;
			m_chunk_size = buffer.substr(0, pos);
			if (!checkCharacters())
				return false;
			buffer.erase(0, pos + delimiter.length());
			m_total_size += getChunkSize() ;

		}
		else
		{
			if (buffer.size() >= getChunkSize() + delimiter.length())
			{
				token = buffer.substr(0, getChunkSize());
				buffer.erase(0, getChunkSize() + delimiter.length());
				body += token;
				if (!getChunkSize())
					m_end = true;
				else
					m_chunk_size = "-1";
			}
			else
				return true;
		}
	}
	return true;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool		Unchunker::checkCharacters(void) const
{
	std::string	hexadecimal = "0123456789abcdefABCDEF";
	for (std::string::const_iterator i = m_chunk_size.begin(); i != m_chunk_size.end(); i++)
	{
		if (hexadecimal.find(*i) == std::string::npos)
			return false;
	}
	return true;
}

size_t			Unchunker::getChunkSize(void)
{
	size_t   size;
    std::istringstream(m_chunk_size) >> std::hex >> size;
	return size;
}

bool const &	Unchunker::getEnd(void) const
{
	return m_end;
}

std::string const	Unchunker::getTotalSize(void) const
{
	std::stringstream ss;
	ss << m_total_size;
	std::string str = ss.str();
	return str;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */