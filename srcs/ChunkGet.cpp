#include "ChunkGet.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ChunkGet::ChunkGet()
: Get()
{
}

ChunkGet::ChunkGet(ChunkGet const & src)
: Get(src)
{
}

ChunkGet::ChunkGet(Get const & src)
: Get(src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ChunkGet::~ChunkGet()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	ChunkGet::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	m_stream.open(m_path.getPath().data());
	if (!m_stream.is_open())
	{
		m_rep = Response::create_error("500", m_location);
		return (m_fd.epollCtlSwitch_w());
	}
	m_rep.start_header("200");
	if (m_header.find("Connection") != m_header.end() && m_header.at("Connection") == "close")
		m_rep.append_to_header("Connection: close");
	else
		m_rep.append_to_header("Connection: keep-alive");
	m_rep.append_to_header("Transfer-Encoding: chunked");
	_add_read();
	return (m_fd.epollCtlSwitch_w());
}

bool	ChunkGet::_add_read(void)
{
	char	buff[READ_SIZE + 1];

	try
	{
		m_stream.read(buff, READ_SIZE);	
	}
	catch(const std::exception& e)
	{
		m_rep = Response::create_error("500", m_location);
		return (false);
	}
	buff[m_stream.gcount()] = 0;
	_add_chunk_size(m_stream.gcount());
	m_rep.append_to_body(buff);
	m_rep.append_to_body("\r\n");
	return (true);
}

void	ChunkGet::_add_chunk_size(std::streamsize const & size)
{
	std::stringstream stream;

	stream << std::hex << size << "\r\n";
	m_rep.append_to_body(stream.str());
}

bool	ChunkGet::send_rep(ASocket ** ptr)
{
	std::string	content = m_rep.getContent();
	int			ret;

	ret = send(getFd(), content.c_str(), content.size(), 0);
	if (ret == 0 || ret == -1 || m_rep.close())
		return (false);
	if (!m_stream.gcount())
	{
		m_stream.close();
		ret = m_fd.epollCtlSwitch_r();
		if (ret)
			_convert<Client>(ptr);
		return (ret);
	}
	return (true);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */