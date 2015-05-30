PROGNAME = wordofmouth
CXX = clang++

# If clang not installed, fall back to gcc
ifeq (, $(shell which clang++))
CC=g++
endif

# Note the requirement of C++11. Your compiler may have a different flag for it.
CFLAGS += -ggdb -std=c++11 -Wall -I/usr/local/include 
LDFLAGS += -L/usr/local/lib
LIBS = -lpthread -lcrypto

OBJS = main.o daemonize.o log.o message.o

all: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROGNAME)
