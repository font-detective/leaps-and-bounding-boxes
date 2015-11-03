CC=g++
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`

PROG=textbox
OBJS=$(PROG).o

.PHONY: all clean $(PROG)
$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)
    
%.o: %.cpp
	$(CC) -c $(CFLAGS) $<
        
all: $(PROG)
        
clean:
	rm -f $(OBJS) $(PROG)
