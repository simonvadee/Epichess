NAME		=		./bin/epichess

CC		=		g++

RM		=		rm -f

SRC		=		./server/src

INC		=		./server/includes

CXXFLAGS	=		-I./$(INC) --std=c++11 -g3

SRCS		=		$(SRC)/Server.cpp \
				$(SRC)/FEN.cpp \
				$(SRC)/Board.cpp \
				$(SRC)/ChessBoard.cpp \
				$(SRC)/BrainCore.cpp \
				$(SRC)/Session.cpp \
				$(SRC)/main.cpp

OBJ		=		$(SRCS:.cpp=.o)

all:				$(NAME)

$(NAME):			$(OBJ)
				$(CC) -o $(NAME) $(OBJ) -lboost_system

clean:
				$(RM) $(OBJ)

fclean:				clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:				all clean fclean re
