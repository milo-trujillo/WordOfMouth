PROGNAME = wordofmouth
CXX = clang++
CFLAGS += -g -Wall
LIBS = #-lm

OBJS = main.o

all: $(OBJS)
	$(CXX) $(CFLAGS) $(LIBS) -o $(PROGNAME) $(OBJS)

$(OBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(PROGNAME)
