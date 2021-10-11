#ifndef ASOCKET_HPP
# define ASOCKET_HPP

# include <iostream>
# include <string>
# include <set>
# include <fcntl.h>
# include "Server.hpp"

class ASocket
{
	public:
        typedef std::set<ASocket *>		list_type;
        typedef struct sockaddr_in		address_type;
        typedef int						fd_type;

		ASocket();
    	ASocket(ASocket const & src);
		virtual ~ASocket() = 0;

		ASocket &				operator=(ASocket const & rhs);
        Server const *			getServer(void);
        fd_type					getFd(void);
        address_type			getAddr(void);
        static ASocket *		getASocketFromFd(fd_type fd);
        void					destroy(void);


		// CLIENT_SOCKET
        void					sendResponse(char *response);

        operator				fd_type(void) const;
        friend bool				operator<(ASocket & lhs, ASocket & rhs);
        friend bool				operator<=(ASocket & lhs, ASocket & rhs);
        friend bool				operator>(ASocket & lhs, ASocket & rhs);
        friend bool				operator>=(ASocket & lhs, ASocket & rhs);
        friend bool				operator==(ASocket & lhs, ASocket & rhs);
        friend bool				operator!=(ASocket & lhs, ASocket & rhs);

	protected:
        Server const *			m_server;
        fd_type					m_fd;
		address_type			m_addr;
        static list_type		m_list;

        void					_makeFdNonBlocking(void);
	
	private:
		virtual	void abstract(void) = 0;
};

#endif /* ********************************************************* ASocket_H */