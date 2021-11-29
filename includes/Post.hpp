#ifndef POST_HPP
# define POST_HPP

# include "Headers.hpp"
# include "Request.hpp"
# include "Unchunker.hpp"

class Post: public Request
{

	public:
		Post();
		Post(Post const & src);
		Post(Request const & src);
		~Post();

		virtual bool	execute(ASocket ** ptr);

	protected:
		Location::file_t const *	m_cgi_pass; 
		std::string					m_body;
		Unchunker					m_unchunker;

		bool						_fillBuffer(void);
	
	private:
		bool						_check(void);
		Location::file_t const *	_cgiPass(void) const;
		bool						_start_cgi(ASocket ** ptr);
		bool						_upload(ASocket ** ptr);

};

#endif /* ************************************************************ POST_H */