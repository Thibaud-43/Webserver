#ifndef CGIPOST_HPP
# define CGIPOST_HPP

# include "Headers.hpp"
# include "ACgi.hpp"
# include "Post.hpp"

class CgiPost: public ACgi, public Post
{
	public:
		CgiPost();
		CgiPost(CgiPost const & src);
		CgiPost(Post const & src);
		~CgiPost();

		virtual bool	execute(ASocket ** ptr);
		virtual bool	entry(ASocket ** ptr);
		virtual bool	manage(ACgi ** ptr, int const & fd);
		virtual bool	start(void);
		char **			getArgs(void) const;
		bool			checkStatus(void);

	private:
		void	_setEnv(void);
		bool	_fillBuffer(void);
		bool	_handle(void);
		void	_bufferToHeader(std::map<std::string, std::string> & header);
		void	_bufferToHeaderLine(std::map<std::string, std::string> & header, std::string & token);
		void	_checkStatus(std::map<std::string, std::string> &	header, std::string & status);

};

#endif /* ********************************************************* CGIPOST_H */