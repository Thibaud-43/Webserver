#ifndef LISTENER_HPP
# define LISTENER_HPP

# include "Headers.hpp"
# include "ASocket.hpp"
# include "Server.hpp"

class Listener: public ASocket
{
	public:
		Listener();
		Listener( Listener const & src );
		Listener(int const & fd, Server const * server, port_type const & port, ip_type const & ip);
		~Listener();

		bool			execute(ASocket **ptr);
		virtual bool	send_rep(ASocket ** ptr);
		bool			alive(void) const;

	private:
		void							_bind(void);
		void							_initAddr(port_type const & port, ip_type const & ip);
		void							_listen(void);
		static list_type				_list;

        address_type					m_addr;
};

#endif /* ******************************************************** LISTENER_H */