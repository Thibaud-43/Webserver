#include "Post.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Post::Post()
: Request() 
{
}

Post::Post(Post const & src)
: Request(src)
{
}

Post::Post(Request const & src)
: Request(src)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Post::~Post()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Post::execute(ASocket ** ptr)
{
	if (ptr)
		*ptr = this;
	// check_post (peut-etre pas ici)
	// RECV -> buff
	// Unchunk ?
	// Upload ? -> convert to upload & exec
	// CGI ? wait for full body -- convert to CGI -- exec
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */