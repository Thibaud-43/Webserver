#include "Upload.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Upload::Upload()
: Request(), m_stream(std::fstream())
{
}

Upload::Upload(Upload const & src)
: Request(src), m_stream(std::fstream())
{
}

Upload::Upload(Request const & src)
: Request(src), m_stream(std::fstream())
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
		m_stream.open(m_path.getPath(), std::fstream::out | std::fstream::trunc);
		if (!m_stream.is_open())
			return (false);
	}
	// Unchunk ?
	// buff >> file
	// clear buff
	// END ? close & convert<Client>
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */