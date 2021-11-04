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
		std::string			m_path;
		header_type			m_header;

		bool		_send(Response const & rep) const;
		void		_convertToClient(ASocket ** ptr);

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

		void		_requestToDelete(ASocket **ptr);
		void		_requestToGet(ASocket **ptr);
		void		_requestToPost(ASocket **ptr);

};

#endif /* ********************************************************* REQUEST_H */