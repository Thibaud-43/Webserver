#ifndef POST_HPP
# define POST_HPP

# include "Headers.hpp"
# include "Request.hpp"

class Post: public virtual Request
{

	public:
		Post();
		Post(Post const & src);
		Post(Request const & src);
		~Post();

		virtual bool	execute(ASocket ** ptr);

	protected:
		Location::file_t const *	m_cgi_pass; 
		bool						_fillBuffer(void);
	
	private:
		bool						_check(void) const;
		Location::file_t const *	_cgiPass(void) const;

};

#endif /* ************************************************************ POST_H */