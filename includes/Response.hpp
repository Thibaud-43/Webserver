#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <string>
# include "Server.hpp"

# define NB_STATUS 40

struct	Status
{
	typedef short	status_code_t;
	typedef char *	status_phrase_t;

	status_code_t	code;
	status_phrase_t	phrase;
};

class Response
{

	public:
		typedef	Status::status_code_t	status_code_t;
		typedef Status::status_phrase_t	status_phrase_t;

		Response();
		Response( Response const & src );
		~Response();

		Response &		operator=( Response const & rhs );

	private:
		static const Status	m_status[NB_STATUS];

};

std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */