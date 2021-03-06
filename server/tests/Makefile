# Use user-specified compiler, or default to clang if possible, else gcc
ifeq (, $(shell which clang++ 2>/dev/null))
CXX?= g++
else
CXX?= clang++
endif

# Note the requirement of C++11. Your compiler may have a different flag for it.
CFLAGS += -ggdb -std=c++11 -Wall -I/usr/local/include #-D VERBOSE
LDFLAGS += -L/usr/local/lib
LIBS = -lnettle

# Test to determine libNettle version - 3 has a header file not present in 2
ifeq (, $(shell find -f /usr/local/include /usr/include -path "*/nettle/*" -a -name "version.h" -a -depth 2 2>/dev/null))
CFLAGS += -DOLD_NETTLE
endif

all:
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o encode.test encode.cpp ../encode.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o encode-messages.test encode-messages.cpp ../encode.cpp ../message.cpp ../log.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o hash.test hash.cpp ../hash.cpp ../encode.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o hmac.test hmac.cpp ../hash.cpp ../cypher.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o hmac-messages.test hmac-messages.cpp ../hash.cpp ../cypher.cpp ../message.cpp ../log.cpp ../encode.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o aes.test aes.cpp ../hash.cpp ../cypher.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o aes-messages.test aes-messages.cpp ../hash.cpp ../cypher.cpp ../message.cpp ../log.cpp ../encode.cpp

test:
	@echo "==== Beginning tests ===="
	@echo "(Rebuild with '-D VERBOSE' in your CFLAGS for more information)"
	@./encode.test
	@./encode-messages.test
	@./hash.test
	@./hmac.test
	@./hmac-messages.test
	@./aes.test
	@./aes-messages.test
	@echo "==== Done with tests ===="

clean:
	@echo "Cleaning tests..."
	@rm -f *.test
	@rm -rf *.dSYM # Apple's compiler leaves some obnoxious test folders
