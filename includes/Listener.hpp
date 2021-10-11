#ifndef LISTENER_HPP
# define LISTENER_HPP

# include "ASocket.hpp"


class Listener: public ASocket
{

	public:

		Listener();
		Listener( Listener const & src );
		Listener(Server * server);
		~Listener();

		Listener &		operator=( Listener const & rhs );

	private:
        void	_create(void);
        void	_bind(void);
        void	_initAddr(std::string const & port, std::string const & ip);
        void	_listen(void);

};

std::ostream &			operator<<( std::ostream & o, Listener const & i );

#endif /* ******************************************************** LISTENER_H */