#ifndef GET_HPP
# define GET_HPP

# include "Headers.hpp"
# include "Request.hpp"

class Get: public Request
{
	public:
		Get();
		Get(Get const & src);
		Get(Request const & src);
		~Get();

		virtual bool execute(ASocket ** ptr);
	
	protected:
		Location::file_t const *	m_cgi_pass; 
		
		std::string					_ltostr(size_t const & len) const;

	private:
		bool						_check(void);
		Location::file_t const *	_cgiPass(void) const; 
		bool						_manageDir();
		bool						_start_cgi(ASocket ** ptr);
		bool						_get(ASocket ** ptr);
		bool						_addFile(void);

	
};

#endif /* ************************************************************* GET_H */