#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(): ASocket()
{
}

Client::Client( const Client & src ): ASocket(src)
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Client::alive(void) const
{
	return (time(NULL) - m_clock < LIFETIME);
}

bool	Client::execute(void)
{
	// SI RIEN A LIRE -> return true;
	// SINON CREER UNE REQUEST, ajout request a la list, return (request->execute);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */