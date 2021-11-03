#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"
# include "ASocket.hpp"

# define LIFETIME 60 // SECONDS

class Client: public ASocket
{

	public:
		Client();
		Client(Client const & src);
		~Client();

	protected:
		bool			alive(void) const;
		virtual bool	execute(void);

	private:
		Client &		operator=(Client const & rhs);

};

#endif /* ********************************************************** CLIENT_H */