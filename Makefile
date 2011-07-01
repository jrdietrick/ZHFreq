CC=g++
CFLAGS=-I.
DEPS = ZiNode.h
OBJ = ZHFreq.o ZiNode.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ZHFreq: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
