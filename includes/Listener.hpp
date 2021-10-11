#ifndef LISTENER_HPP
# define LISTENER_HPP

# include "ASocket.hpp"


class Listener: public ASocket
{

	public:

		Listener();
		Listener( Listener const & src );
		Listener(fd_type epoll, port_type port, ip_type ip);
		~Listener();

		Listener &		operator=( Listener const & rhs );

	private:
        void	_create(void);
        void	_bind(void);
        void	_initAddr(port_type port, ip_type ip);
        void	_listen(void);

};

std::ostream &			operator<<( std::ostream & o, Listener const & i );

#endif /* ******************************************************** LISTENER_H */