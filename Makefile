PROGNAME = wordofmouth
CXX = clang++
CFLAGS += -g -Wall
LIBS = -lpthread -lcrypto -lgcrypt -lgpg-error

OBJS = main.o networking.o CipherAlias.o

all: $(OBJS)
	$(CXX) $(CFLAGS) $(LIBS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(PROGNAME)
