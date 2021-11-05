#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"
# include "ASocket.hpp"
# include "Client.hpp"

# define LIFETIME 60 // SECONDS

class Client: public ASocket
{

	public:
		Client();
		Client(Client const & src);
		Client(int const & fd, Server const * server);
		~Client();

		bool			alive(void) const;
		virtual bool	execute(ASocket ** ptr);

	protected:
		std::string		m_buff;

		bool			_send(Response const & rep) const;
		bool			_send(std::string const & rep) const;
		virtual bool	_fillBuffer(void);


};

#endif /* ********************************************************** CLIENT_H */