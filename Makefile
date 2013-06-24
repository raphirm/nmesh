CFLAGS=-g
LDFLAGS=-lpthread
CFLAGS=-pthread
CC=gcc
OBJECTS=main.o pkglisthandler.o buffer.o conn_client.o conn_io.o conn_server.o nodelist.o newConnection.o thread.o
TARGET=mesh

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	cp $(TARGET) test/$(TARGET)
	cp $(TARGET) test/$(TARGET)2

include depends

depends:
	$(CC) -MM $(OBJECTS:.o=.c) > depends

clean:
	rm ./$(TARGET) *.o 

