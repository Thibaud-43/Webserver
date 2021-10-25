#ifndef HEADER_HPP
# define HEADER_HPP

# include <algorithm> 
# include <arpa/inet.h>
# include <cstring>
# include <ctime>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>
# include <fstream>
# include <iostream>
# include <iterator>
# include <map>
# include <list>
# include <netinet/in.h>
# include <signal.h>
# include <set>
# include <sstream>
# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <vector>

# define MAX_SERVER_BODY_SIZE 1024
# define SERV_NAME "Webserv/1.0.0"
# define CGI_VERSION "CGI/1.1"
# define READ_SIZE 4096
# define MAX_EVENTS 10
# define MYPORT 3490
# define PROTOCOL "HTTP/1.1"

#endif