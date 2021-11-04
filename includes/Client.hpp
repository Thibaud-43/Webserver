#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"
# include "ASocket.hpp"

# define LIFETIME 60 // SECONDS

class Client: public ASocket
{

	public:
		Client();
		Client(Client const & src);
		~Client();
		
		Client &		operator=(Client const & rhs);

	protected:
		std::string		m_buff;

		bool			alive(void) const;
		virtual bool	execute(void);

};

#endif /* ********************************************************** CLIENT_H */