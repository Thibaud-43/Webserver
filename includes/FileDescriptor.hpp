#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include "Headers.hpp"

class FileDescriptor
{

	public:
		typedef struct epoll_event		event_type;

		FileDescriptor(void);
		FileDescriptor(FileDescriptor const & src);
		FileDescriptor(int const fd);
		~FileDescriptor();

		int						value(void) const;

		static void				setEpollFd(void);
		static int const & 		getEpollFd(void);
		
		void					epollCtlAdd(void);
		void					epollCtlAdd_w(void);
		void					epollCtlDel(void);
		static void				epollCtlDel(int const & fd);
		void					makeFdNonBlocking(void);

	private:
		int const	m_fd;

		static	int	_epoll_fd;
};

#endif /* ************************************************** FILEDESCRIPTOR_H */