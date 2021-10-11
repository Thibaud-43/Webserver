#include "Listener.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Listener::Listener()
{
}

Listener::Listener( const Listener & src )
{
}

Listener::Listener(Server * server)
{

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Listener::~Listener()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Listener &				Listener::operator=( Listener const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Listener const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */