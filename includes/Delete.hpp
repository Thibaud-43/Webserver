#ifndef DELETE_HPP
# define DELETE_HPP

# include "Headers.hpp"
# include "Request.hpp"
# include "File.hpp"

class Delete: public Request
{
	public:
		Delete();
		Delete(Delete const & src);
		Delete(Request const & src);
		~Delete();

	private:
		bool	_check(void) const;

};

#endif /* ********************************************************** DELETE_H */