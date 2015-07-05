PROGNAME = wordofmouth
CXX?= clang++

# If clang not installed, fall back to gcc
ifeq (, $(shell which clang++ 2>/dev/null))
CXX?= g++
endif

# Note the requirement of C++11. Your compiler may have a different flag for it.
CFLAGS += -ggdb -std=c++11 -Wall -I/usr/local/include #-Rpass=inline
LDFLAGS += -L/usr/local/lib
LIBS = -lnettle

# Test to determine libNettle version - 3 has a header file not present in 2
ifeq (, $(shell find -f /usr/local/include /usr/include -path "*/nettle/*" -a -name "version.h" -a -depth 2 2>/dev/null))
CFLAGS += -DOLD_NETTLE
endif

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
