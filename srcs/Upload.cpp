#include "Upload.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Upload::Upload()
: Request()
{
}

Upload::Upload(Upload const & src)
: Request(src)
{
}

Upload::Upload(Request const & src)
: Request(src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Upload::~Upload()
{
	if (m_stream.is_open())
		m_stream.close();
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

bool	Upload::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	if (!m_stream.is_open())
	{
		m_stream.open(m_path.getPath().data(), std::fstream::out | std::fstream::trunc);
		if (!m_stream.is_open())
			return (false);
	}
	if (m_header.find("Content-Length") != m_header.end())
	{
		if (!_fillBuffer())
			return false;
		if (m_header["Content-Length"] == "0")
		{
			m_body = m_buff;
			m_buff.clear();
		}
	}
	else if (m_header.find("Transfer-Encoding") != m_header.end() && m_header["Transfer-Encoding"] == "chunked")
	{
		if (!_fillBuffer())
			return false;
		m_unchunker(m_buff, m_body);
		m_header["Content-Length"] = m_unchunker.getTotalSize();
		// buff >> file
		// clear buff
	}
	// END ? close & convert<Client>
	return (true); // ?
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */