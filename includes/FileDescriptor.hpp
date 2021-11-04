#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include <iostream>

class FileDescriptor
{

	public:

		FileDescriptor(int const fd = -1);
		FileDescriptor(FileDescriptor const & src);
		~FileDescriptor();

		FileDescriptor &		operator=( FileDescriptor const & rhs );
		int						value(void) const;

		static void				setEpollFd(int const fd);
		
		// AJOUTEZ METHODES

	private:
		int const	m_fd;

		static	int	_epoll_fd;
};

#endif /* ************************************************** FILEDESCRIPTOR_H */