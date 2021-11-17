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
		
		bool					epollCtlAdd_r(void);
		bool					epollCtlAdd_w(void);
		bool					epollCtlDel(void);
		bool					epollCtlSwitch_w(void);
		bool					epollCtlSwitch_r(void);
		static bool				epollCtlDel(int const & fd);
		bool					makeFdNonBlocking(void);

	private:
		int const	m_fd;

		static	int	_epoll_fd;
};

#endif /* ************************************************** FILEDESCRIPTOR_H */