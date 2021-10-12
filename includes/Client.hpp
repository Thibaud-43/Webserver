#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ASocket.hpp"
class Client: public ASocket
{

	public:

        typedef std::set<Client>		list_type;

		Client();
		Client( Client const & src );
		Client(fd_type client_fd, address_type & theirAddr, fd_type epoll);
		Client &		operator=(Client const & rhs);

		~Client();
        void					sendResponse(char const *response) const;
        static Client const *			getClientFromFd(fd_type fd);




	private:
		static list_type		_list;

};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ******************************************************** Client_H */