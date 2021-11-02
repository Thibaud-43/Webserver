#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Headers.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "Response.hpp"
# include "File.hpp"

class Cgi;

typedef	enum	e_status
{
	BEGIN,
	REQUEST_LINE_AND_HOST_COMPLETED,
	HOST_COMPLETED,
	HEADER_COMPLETED,
	GET_BODY,
	START_UPLOAD,
	UPLOADING,
	BODY_COMPLETED
}				t_status;

class Request
{

	public:
		typedef std::list<Request>					list_type;
		typedef std::map<std::string, std::string>	header_type;
		typedef std::string							body_type;

		Request();
		Request(Request const & src);
		Request(std::string const request);
		Request(Client const * client);
		~Request();

		Request &			operator=(Request const & rhs);
		bool				manage(std::string & buffer, std::vector<Server*> const & servers);

		Client const *		getClient(void) const;
		bool				ready(void) const;
		
		
		static	Request *	getRequestFromClient(Client const & client);
		static  Request *	createRequest(Client const & client);
		static	void		removeRequest(Request const & request);
		header_type const &	getHeader(void) const;
		body_type const &	getBody(void) const;
		std::string const &	getPath(void) const;
		Server const *		getServer(void) const;
		Location const *	getLocation(void) const;
		bool				getStatus(void) const;

	private:
		header_type							m_header;
		std::string							m_body;
		Client const *						m_client;							
		Server const *						m_server;
		Location const *					m_location;
		std::string							m_path;
		t_status							m_status;

		static list_type					_list;
		void								_linkServer(std::vector<Server*> const & list);
		void								_linkPath(void);
		void								_linkLocation(void);
		bool								_checkHeader(std::vector<Server*> const & servers);
		
		bool								_checkRequestLine(void);
		bool								_checkHost(void);
		void								_bufferToRequestLine(std::string & buffer);				
		void								_bufferToHeader(std::string & buffer);
		void								_bufferToBody(std::string & buffer);
		void								_bufferToHeaderLine(std::string & token);
		void								_checkRequestAdvancement(void) ;
		void								_checkBodySize(void) ;
		void								_checkChunkAdvancement(void) ;
		bool								_checkBufferCharacters(std::string & str);
		bool								_unChunked(std::string & str);


		// TOM
		void								_execute(void) const;
		bool								_check_get(void) const;
		bool								_check_post(void) const;
		bool								_check_delete(void) const;
		void								_delete(void) const;	
		void								_get(void) const;
		Location::file_t const *			_get_cgi_path(void) const;
		void								_execute_cgi(Location::file_t const & cgi_path) const;
		void								_chunk_size_to_client(std::streamsize const & s) const;

		// DEBUG

		void								_printHeader(void);
		void								_printBody(void);
		void								_printHex(std::string & token);

};

std::ostream &								operator<<(std::ostream & o, Request const & i);

#endif /* ********************************************************* REQUEST_H */