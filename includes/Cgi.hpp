#ifndef CGI_HPP
# define CGI_HPP

# include "Headers.hpp"
# include "Get.hpp"
# include "Post.hpp"

class Cgi: public Get, public Post
{

	public:
		typedef std::set<Cgi *>	list_type;

		Cgi();
		Cgi(Cgi const & src);
		Cgi(Get const & src);
		Cgi(Post const & src);
		~Cgi();

		virtual bool		execute(ASocket ** ptr);
		int					getFdIn(void) const;
		int					getFdOut(void) const;

		static Cgi const *	getCgi(int const & fd);
		static Cgi const *	addCgi(Cgi * cgi);
		static void			removeCgi(Cgi * cgi);

	private:
		int		m_fd_in;
		int		m_fd_out;

		static	list_type	_list;
};

#endif /* ************************************************************* CGI_H */