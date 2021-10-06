
SRCS = 	Cluster.cpp \
		Location.cpp \
		Server.cpp 

SERVER	= server

CXX		= clang++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM		= rm -rf

SRCS_PATH			=	./srcs/
OBJS_PATH			=	.objs/
INC					= -I includes/


OBJS	= $(addprefix $(OBJS_PATH),$(SRCS.cpp=.o))

all: $(SERVER)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@ mkdir -p $(OBJS_PATH)
	$(CXX) $(INC) $(CXXFLAGS) -c $< -o $@

$(SERVER): $(OBJS)
	$(CXX) $(OBJS) $(INC) -o $(SERVER)

clean: 
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(SERVER)

re: clean all

.PHONY: all clean fclean re