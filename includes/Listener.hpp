#ifndef LISTENER_HPP
# define LISTENER_HPP

# include "Headers.hpp"
# include "ASocket.hpp"
# include "Client.hpp"

class Listener: public ASocket
{

	public:
		Listener();
		Listener( Listener const & src );
		Listener(int const & fd, port_type const & port, ip_type const & ip);
		~Listener();

		bool	execute(void);

	private:
		void							_bind(void);
		void							_initAddr(port_type const & port, ip_type const & ip);
		void							_listen(void);
		static list_type				_list;

        address_type					m_addr;

		Listener &						operator=( Listener const & rhs );
};

#endif /* ******************************************************** LISTENER_H */