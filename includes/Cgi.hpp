#ifndef CGI_HPP
# define CGI_HPP

# include "Headers.hpp"
# include "Get.hpp"
# include "Post.hpp"

class Cgi: public Get, public Post
{

	public:
		typedef std::set<Cgi const *>				list_type;
		typedef std::map<std::string, std::string>	env_type;

		Cgi();
		Cgi(Cgi const & src);
		Cgi(Get const & src);
		Cgi(Post const & src);
		~Cgi();

		virtual bool		execute(ASocket ** ptr);
		int					getFdIn(void) const;
		int					getFdOut(void) const;
		bool				start(void);
		bool				manage(int const & fd) const;
		// check_status 

		static Cgi const *	getCgi(int const & fd);
		static Cgi const *	addCgi(Cgi const * cgi);
		static void			removeCgi(Cgi const * cgi);

	private:
		int			m_fd_in;
		int			m_fd_out;
		env_type	m_env;

		static	list_type	_list;

		void		_setEnv(void);
};

#endif /* ************************************************************* CGI_H */