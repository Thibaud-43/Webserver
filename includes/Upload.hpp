#ifndef UPLOAD_HPP
# define UPLOAD_HPP

# include "Headers.hpp"
# include "Request.hpp"

class Upload: public Request
{

	public:
		Upload();
		Upload( Upload const & src );
		Upload( Request const & src );
		~Upload();

		virtual bool	execute(ASocket ** ptr);

	private:
		std::fstream	m_stream;

};

#endif /* ********************************************************** UPLOAD_H */