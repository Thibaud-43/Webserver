#ifndef CGIGET_HPP
# define CGIGET_HPP

# include "Headers.hpp"
# include "ACgi.hpp"
# include "Get.hpp"

class CgiGet: public Get, public ACgi
{
	public:
		CgiGet();
		CgiGet(CgiGet const & src);
		CgiGet(Get const & src);
		~CgiGet();
		
		virtual bool	execute(ASocket ** ptr);
		virtual bool	manage(ACgi ** ptr, int const & fd);
		virtual bool	start(void);
		char **			getArgs(void) const;
		bool			checkStatus(void);

	private:
		bool			_fillBuffer(void);
		bool			_handle(void);
		void			_bufferToHeader(std::map<std::string, std::string> & header);
		void			_bufferToHeaderLine(std::map<std::string, std::string> & header, std::string & token);
		void			_checkStatus(std::map<std::string, std::string> &	header, std::string & status);

		void			_setEnv(void);

};

#endif /* ********************************************************** CGIGET_H */