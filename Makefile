NAME		=		./bin/epichess

CC		=		g++

RM		=		rm -f

SRC		=		./src

INC		=		./includes

CXXFLAGS	=		-I./$(INC) --std=c++11 -lboost_system

SRCS		=		$(SRC)/server.cpp

OBJ		=		$(SRCS:.cpp=.o)

all:				$(NAME)

$(NAME):			$(OBJ)
				$(CC) $(OBJ) $(CXXFLAGS) -o $(NAME)

clean:
				$(RM) $(OBJ)

fclean:				clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:				all clean fclean re
