#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"
# include "ASocket.hpp"

class Client: public ASocket
{

	public:

		typedef std::set<Client>		list_type;

		Client();
		Client( Client const & src );
		Client(fd_type client_fd, address_type & theirAddr, fd_type epoll);
		~Client();

		Client &						operator=(Client const & rhs);

		void							sendResponse(char const *response) const;
		static Client const *			getClientFromFd(fd_type fd);
		static void						closeConnexion(Client const & client);

	private:
		static list_type				_list;

};

std::ostream &							operator<<( std::ostream & o, Client const & i );

#endif /* ******************************************************** Client_H */