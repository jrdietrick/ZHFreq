CC=g++
CFLAGS=-I.

DEP=ZiNode.h

OBJDIR=obj
_OBJ=ZHFreq.o ZiNode.o
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: %.cpp $(DEP)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/ZHFreq: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/ZHFreq *~
