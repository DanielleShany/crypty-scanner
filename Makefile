CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

TARGET = find_sig
SRCS = src/main.cpp src/cryptyScanner.cpp 
OBJS = $(SRCS:.cpp=.o)
HDRS = include/cryptyScanner.hpp include/MagicIdentifier.hpp include/SignatureScanner.hpp

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo; echo "=== Compilation Successful ==="

test: all
	@echo; echo "=== Functional Test ==="
	@./find_sig test_files test_files/crypty.sig
	@echo; echo "=== Invalid-Input Test ==="
	@bash test_invalid_input.sh

clean:
	rm -f $(OBJS) $(TARGET)

