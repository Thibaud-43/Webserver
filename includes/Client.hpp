#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ASocket.hpp"

class Client: public ASocket
{

	public:

		Client();
		Client( Client const & src );
		Client(fd_type client_fd, address_type & theirAddr, fd_type epoll);
		~Client();
        void					sendResponse(char const *response) const;

		Client &		operator=( Client const & rhs );

	private:


};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ******************************************************** Client_H */