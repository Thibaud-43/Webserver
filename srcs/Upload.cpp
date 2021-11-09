#include "Upload.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Upload::Upload()
: Post(), m_stream_size(0)
{
	m_stream.open(m_path.getPath(), std::fstream::out | std::fstream::trunc);
}

Upload::Upload(Upload const & src)
: Post(src), m_stream_size(src.m_stream_size)
{
	m_stream.open(m_path.getPath(), std::fstream::out | std::fstream::app);
}

Upload::Upload(Post const & src)
: Post(src), m_stream_size(0)
{
	m_stream.open(m_path.getPath(), std::fstream::out | std::fstream::trunc);
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
	if (!Post::_fillBuffer())
		return (false);
	return (entry(ptr));
}

bool	Upload::entry(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	if (!m_stream.is_open())
		return (_send(Response::create_error("403", m_location)));
	if (m_header.find("Content-Length") != m_header.end())
	{
		m_stream << m_buff;
		m_stream_size += m_buff.size();
		m_buff.clear();
		if (m_stream_size >= _strToSize(m_header["Content-Length"]))
		{
			m_stream.close();
			if (!_send_created())
				return (false);
			_convert<Client>(ptr);
		}
		return (true);
	}
	else if (m_header.find("Transfer-Encoding") != m_header.end() && m_header["Transfer-Encoding"] == "chunked")
	{
		m_unchunker(m_buff, m_body);
		m_stream << m_body;
		m_buff.clear();
		m_body.clear();
		if (m_unchunker.getEnd())
		{
			m_stream.close();
			if (!_send_created())
				return (false);
			_convert<Client>(ptr);
		}
		return (true);
	}
	else
		return (false);
}

bool	Upload::_send_created(void) const
{
	Response	rep;
	bool		ret = true;

	rep.start_header("201");
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
	{
		ret = false;
		rep.append_to_header("Connection: close");
	}
	else
		rep.append_to_header("Connection: keep-alive");
	if (!_send(rep))
		return (false);
	return (ret);
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */