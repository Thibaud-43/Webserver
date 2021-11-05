#ifndef CGIPOST_HPP
# define CGIPOST_HPP

# include "Headers.hpp"
# include "ACgi.hpp"
# include "Post.hpp"

class CgiPost: public ACgi, public Post
{
	public:
		CgiPost();
		CgiPost(CgiPost const & src );
		CgiPost(Post const & src );
		~CgiPost();

		virtual bool	execute(ASocket ** ptr);
		virtual bool	manage(int const & fd) const;
		virtual bool	start(void);

	private:
		void	_setEnv(void);

};

#endif /* ********************************************************* CGIPOST_H */