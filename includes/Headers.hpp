#ifndef HEADER_HPP
# define HEADER_HPP

# include <algorithm> 
# include <cstring>
# include <ctime>
# include <fstream>
# include <iostream>
# include <iterator>
# include <map>
# include <list>
# include <vector>
# include <set>
# include <sstream>
# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <netinet/in.h>
# include <signal.h>
# include <arpa/inet.h>

# define MAX_SERVER_BODY_SIZE 1024000
# define SERV_NAME "Webserv/1.0.0"
# define CGI_VERSION "CGI/1.1"
# define READ_SIZE 1024
# define READ_SIZE_RECV 1024000
# define MAX_EVENTS 10
# define MYPORT 3490
# define PROTOCOL "HTTP/1.1"
# define HEADER_SIZE_LIMIT  20000
# define PROTOCOL "HTTP/1.1"
# define LIFETIME 60 // SECONDS

#endif