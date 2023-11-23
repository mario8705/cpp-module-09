SRCS = main.cpp BitcoinExchange.cpp
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
TARGET = btc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $<

%.o: %.cpp BitcoinExchange.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
