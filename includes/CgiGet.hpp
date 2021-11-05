#ifndef CGIGET_HPP
# define CGIGET_HPP

# include "Headers.hpp"
# include "ACgi.hpp"
# include "Get.hpp"

class CgiGet: public ACgi, public Get
{
	public:
		CgiGet();
		CgiGet(CgiGet const & src);
		CgiGet(Get const & src);
		~CgiGet();
		
		virtual bool	execute(ASocket ** ptr);
		virtual bool	manage(int const & fd);
		virtual bool	start(void);
		char **			getArgs(void) const;
		bool			checkStatus(void);

	private:
		void	_setEnv(void);

};

#endif /* ********************************************************** CGIGET_H */