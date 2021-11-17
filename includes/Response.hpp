#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Headers.hpp"
# include "Location.hpp"
# include "File.hpp"

class Response
{

	public:
		typedef	std::string									status_code_t;
		typedef std::string									status_phrase_t;
		typedef std::string									file_t;
		typedef std::map<status_code_t, status_phrase_t>	status_t;

		Response();
		Response(Response const & src);
		~Response();

		std::string		getContent(void) const;
		bool			close(void) const;
		void			start_header(status_code_t const & status);
		void			append_to_header(std::string const & str);
		void			append_to_body(std::string const & str);
		void			add_content_length(void);
		void			fill_body(file_t const & file);
		void			clear(void);
		void			debug(void)const;
		Response &		operator=(Response const & rhs);
		static Response	create_error(status_code_t const & err, Location const * location);
		static Response	create_redirect(status_code_t const & red, std::string const & location);
		static Response	create_index(std::string const & directory, Location const * location, std::string const & uri);

	private:
		static status_t	_status;
		std::string		m_header;
		std::string		m_body;
		
		static status_t	_createStatus(void);
};

std::ostream &	operator<<( std::ostream & o, Response const & i );
std::string		HTTPDate(void);
std::string		OSName(void);

#endif /* ******************************************************** RESPONSE_H */