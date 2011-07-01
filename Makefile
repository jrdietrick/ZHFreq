CC=g++
CFLAGS=-I.

DEP=ZiNode.h

OBJDIR=obj
_OBJ=ZHFreq.o ZiNode.o
OBJ=$(patsubst %,$(OBJDIR)/%,$(_OBJ))

TARGET=$(OBJDIR)/ZHFreq

all: $(TARGET)

$(OBJDIR)/%.o: %.cpp $(DEP) prepare
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/ZHFreq *~

purge: clean
	rm -rf $(OBJDIR)

prepare:
	mkdir -p $(OBJDIR)

rebuild: clean $(TARGET)
