#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Headers.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "Response.hpp"

class Request: public Client
{

	public:
		typedef std::map<std::string, std::string>	header_type;

		Request();
		Request(Request const & src);
		Request(Client const & src);
		~Request();

		virtual bool	execute(ASocket **ptr);

	protected:
		Server const *		m_server;
		Location const *	m_location;
		File				m_path;
		header_type			m_header;

		template<typename T>
		void		_convert(ASocket ** ptr)
		{
			T	*elem = new T(*this);

			if (ptr)
				*ptr = elem;
			ASocket::removeSocket(this);
			ASocket::addSocket(elem);
		}
		bool		_decrement(size_t const & n);

	private:
		void		_bufferToRequestLine(void);
		void		_bufferToHeader(void);
		void		_bufferToHeaderLine(std::string & token);
		void		_printHeader(void);
		bool		_checkHeader(void);
		bool		_checkRequestLine(void);
		bool		_checkHost(void);
		void		_linkLocation(void);
		void		_linkPath(void);
};

#endif /* ********************************************************* REQUEST_H */