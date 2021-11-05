#ifndef POST_HPP
# define POST_HPP

# include "Headers.hpp"
# include "Request.hpp"
# include "Unchunker.hpp"

class Post: public virtual Request
{

	public:
		Post();
		Post(Post const & src);
		Post(Request const & src);
		~Post();

		virtual bool	execute(ASocket ** ptr);

	protected:
		std::string	m_body;
		Unchunker	m_unchunker;
		bool		_fillBuffer(void);

};

#endif /* ************************************************************ POST_H */