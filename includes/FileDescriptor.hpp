#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include "Headers.hpp"

class FileDescriptor
{

	public:
		typedef struct epoll_event		event_type;

		FileDescriptor(int const fd = -1);
		FileDescriptor(FileDescriptor const & src);
		~FileDescriptor();

		FileDescriptor &		operator=( FileDescriptor const & rhs );
		int						value(void) const;

		static void				setEpollFd(void);
		
		void					epollCtlAdd(void);
		void					epollCtlAdd_w(void);
		void					epollCtlDel(void);
		void					makeFdNonBlocking(void);

	private:
		int const	m_fd;

		static	int	_epoll_fd;
};

#endif /* ************************************************** FILEDESCRIPTOR_H */