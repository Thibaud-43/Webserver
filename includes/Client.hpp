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
		Client(int const & fd);
		~Client();

		bool			alive(void) const;
		void			sendResponse(char const * response) const;
		virtual bool	execute(ASocket * ptr);
	
	protected:
		std::string		m_buff;


		bool			_fillBuffer(void);


};

#endif /* ********************************************************** CLIENT_H */