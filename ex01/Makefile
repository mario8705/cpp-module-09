SRCS = main.cpp RPN.cpp
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
TARGET = RPN

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp RPN.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
