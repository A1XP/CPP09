NAME = PmergeMe
SRCS = main.cpp PmergeMe.cpp
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++23 -g

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: clean all

.PHONY: all clean fclean re