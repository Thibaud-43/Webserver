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

	private:
		bool	_check(void) const;
		bool	_manageDir(void);
		bool	_get(void) const;
	
};

#endif /* ************************************************************* GET_H */