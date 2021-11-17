#include "Upload.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Upload::Upload()
: Post(), m_stream_size(0)
{
	m_stream.open(m_path.getPath().data(), std::fstream::out | std::fstream::trunc);
}

Upload::Upload(Upload const & src)
: Post(src), m_stream_size(src.m_stream_size)
{
	m_stream.open(m_path.getPath().data(), std::fstream::out | std::fstream::app);
}

Upload::Upload(Post const & src)
: Post(src), m_stream_size(0)
{
	m_stream.open(m_path.getPath().data(), std::fstream::out | std::fstream::trunc);
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
		return (m_fd.epollCtlSwitch_w());
	return (entry(ptr));
}

bool	Upload::entry(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	if (!m_stream.is_open())
	{
		m_rep = Response::create_error("403", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
	if (m_header.find("Content-Length") != m_header.end())
	{
		m_stream << m_buff;
		m_stream_size += m_buff.size();
		m_buff.clear();
		if (m_stream_size >= _strToSize(m_header["Content-Length"]))
		{
			m_stream.close();
			_created();
			return (m_fd.epollCtlSwitch_w());
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
			_created();
			return (m_fd.epollCtlSwitch_w());
		}
		return (true);
	}
	else
	{
		m_rep = Response::create_error("411", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
}

void	Upload::_created(void)
{
	m_rep.start_header("201");
	m_rep.append_to_body("<html>\n");
	m_rep.append_to_body("<head><title>201 Created</title></head>\n");
	m_rep.append_to_body("<body bgcolor=\"white\">\n");
	m_rep.append_to_body("<center><h1>201 Created</h1></center>\n");
	m_rep.append_to_body("<hr><center>");
	m_rep.append_to_body(SERV_NAME);
	m_rep.append_to_body("</center>\n");
	m_rep.append_to_body("</body>\n");
	m_rep.append_to_body("</html>\n");
	m_rep.append_to_header("Content-Type: text/html");
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
		m_rep.append_to_header("Connection: close");
	else
		m_rep.append_to_header("Connection: keep-alive");
	m_rep.add_content_length();
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */