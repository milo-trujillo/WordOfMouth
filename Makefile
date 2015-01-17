PROGNAME = wordofmouth
CXX = clang++
# Apple is moving from OpenSSL to their own Crypto, so any use of OpenSSL
# throws 'deprecated' warnings on their systems, hence -Wno-deprecated
CFLAGS += -g -Wall -Wno-deprecated -I/usr/local/include
LDFLAGS += -L/usr/local/lib
LIBS = -lpthread -lcrypto -lgcrypt -lgpg-error

OBJS = main.o daemonize.o relay.o messages.o cypher.o keygen.o hash.o NextNodeInfo.o log.o

all: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROGNAME)
