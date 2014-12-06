PROGNAME = wordofmouth
CXX = clang++
CFLAGS += -g -Wall -Wno-deprecated
# Apple is moving from OpenSSL to their own Crypto, so any use of OpenSSL
# throws 'deprecated' warnings on their systems
LIBS = -lpthread -lcrypto -lgcrypt -lgpg-error

OBJS = main.o networking.o CipherAlias.o keygen.o hash.o

all: $(OBJS)
	$(CXX) $(CFLAGS) $(LIBS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(PROGNAME)
