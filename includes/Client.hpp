#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"
# include "ASocket.hpp"
# include "Response.hpp"

class Client: public ASocket
{

	public:
		Client();
		Client(Client const & src);
		Client(int const & fd, Server const * server, struct sockaddr_in const & remote_addr);
		~Client();

		bool			alive(void) const;
		virtual bool	execute(ASocket ** ptr);
		virtual bool	send_rep(ASocket ** ptr);

	protected:
		std::string			m_buff;
		struct sockaddr_in	m_remote_addr;
		Response			m_rep;

		template<typename T>
		void		_convert(ASocket ** ptr)
		{
			T	*elem = new T(*this);

			if (ptr)
				*ptr = elem;
			ASocket::addSocket(elem);
		}

		virtual bool	_fillBuffer(void);


};

#endif /* ********************************************************** CLIENT_H */