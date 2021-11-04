#ifndef LISTENER_HPP
# define LISTENER_HPP

# include "Headers.hpp"
# include "ASocket.hpp"

class Listener: public ASocket
{

	public:
		Listener();
		Listener( Listener const & src );
		Listener(int fd, port_type port, ip_type ip);
		~Listener();


	private:
		void							_bind(void);
		void							_initAddr(port_type port, ip_type ip);
		void							_listen(void);
		static list_type				_list;

        address_type					m_addr;

		Listener &						operator=( Listener const & rhs );
};

#endif /* ******************************************************** LISTENER_H */