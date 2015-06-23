CXX = clang++

# If clang not installed, fall back to gcc
ifeq (, $(shell which clang++))
CXX = g++
endif

# Note the requirement of C++11. Your compiler may have a different flag for it.
CFLAGS += -ggdb -std=c++11 -Wall -I/usr/local/include #-D VERBOSE
LDFLAGS += -L/usr/local/lib
LIBS = -lnettle

all:
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o encode.test encode.cpp ../encode.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o encode-messages.test encode-messages.cpp ../encode.cpp ../message.cpp ../log.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o hash.test hash.cpp ../hash.cpp ../encode.cpp

test:
	@echo "==== Beginning tests ===="
	@./encode.test
	@./encode-messages.test
	@./hash.test
	@echo "==== Done with tests ===="

clean:
	rm -f *.test
