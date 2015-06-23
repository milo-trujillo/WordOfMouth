PROGNAME = wordofmouth
CXX = clang++

# If clang not installed, fall back to gcc
ifeq (, $(shell which clang++))
CXX = g++
endif

# Note the requirement of C++11. Your compiler may have a different flag for it.
CFLAGS += -ggdb -std=c++11 -Wall -I/usr/local/include 
LDFLAGS += -L/usr/local/lib
LIBS = -lnettle

OBJS = main.o daemonize.o log.o message.o cypher.o keypair.o hash.o encode.o

all: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

test:
	cd tests && $(MAKE)
	cd tests && $(MAKE) test

clean:
	cd tests && $(MAKE) clean
	rm -f *.o $(PROGNAME)
