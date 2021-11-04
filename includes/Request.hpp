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
		typedef void (*RequestFunction)(ASocket **ptr);

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

		static	std::map<std::string, RequestFunction>	functionMap;
		static	std::map<std::string, Request::RequestFunction>	_initMap(void);

		static void	_requestToGet(ASocket **ptr);
		static void	_requestToDelete(ASocket **ptr);
		static void	_requestToPost(ASocket **ptr);

};

#endif /* ********************************************************* REQUEST_H */