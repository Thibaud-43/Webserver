#ifndef GET_HPP
# define GET_HPP

# include "Headers.hpp"
# include "Request.hpp"

class Get: public virtual Request
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
		bool						_chunk_size_to_client(std::streamsize const & size) const;

	private:
		bool						_check(void) const;
		Location::file_t const *	_cgiPass(void) const; 
		bool						_manageDir(ASocket ** ptr);
		bool						_start_cgi(ASocket ** ptr);
		bool						_get(ASocket ** ptr);
		bool						_sendChunkedFile(void) const;
		bool						_sendFile(void) const;

	
};

#endif /* ************************************************************* GET_H */