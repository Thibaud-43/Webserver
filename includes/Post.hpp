#ifndef POST_HPP
# define POST_HPP

# include "Headers.hpp"
# include "Request.hpp"

class Post: public Request
{
	public:
		Post();
		Post(Post const & src);
		Post(Request const & src);
		~Post();

		virtual bool execute(ASocket ** ptr);
	
	protected:
		Location::file_t const *	m_cgi_pass; 

	private:
		bool						_check(void) const;
		Location::file_t const *	_cgiPass(void) const; 
		bool						_manageDir(ASocket ** ptr);
		bool						_start_cgi(ASocket ** ptr);
		bool						_Post(ASocket ** ptr);
	
};

#endif /* ************************************************************* Post_H */