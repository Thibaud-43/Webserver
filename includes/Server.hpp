#ifndef SERVER_HPP
# define SERVER_HPP

# include "Headers.hpp"
# include "Location.hpp"
# include "Listener.hpp"
# include "Client.hpp"
# include "Node.hpp"

# define PROTOCOL "HTTP/1.1"
class Server
{

	public:
		typedef int						fd_type;

		Server();
		Server( Server const & src );
		Server(std::string port, std::string ip);
		~Server();

		Server &						operator=( Server const & rhs );
		int								getSocketFd(void) const;
		int								run(fd_type epoll);

		std::string						getIp(void) const;
		std::string						getPort(void) const;
		std::vector<std::string>		getNames(void) const;
		Location const &				getParams(void) const;
		std::vector<Location>			getLocations(void) const;					
		Location const *				getLocation(std::string const & uri) const;	

		void	setIpandPort(std::vector<std::string> const & content);
		void	setNames(std::vector<std::string> const & content);

		void							fillServer(Node* node);
		void							fillLocation(Node* node);

		void	setUri(std::vector<std::string> const &content);
		void	setErrPages(std::vector<std::string> const &content);
		void	setBodySize(std::vector<std::string> const &content);
		void	setRoot(std::vector<std::string> const &content);
		void	setIndexes(std::vector<std::string> const &content);
		void	setMethods(std::vector<std::string> const &content);
		void	setRedirect(std::vector<std::string> const &content);
		void	setautoindex(std::vector<std::string> const &content);
		void	setUpload(std::vector<std::string> const &content);
		void	setCGIPass(std::vector<std::string> const &content);
		void	setValue(Node* node);


	private:
		std::string						m_ip;
		std::string						m_port;
		std::vector<std::string>		m_names;
		Location						m_params;
		std::vector<Location>			m_locations;
};

std::ostream &			operator<<( std::ostream & o, Server const & rhs );


#endif /* ********************************************************** SERVER_H */
