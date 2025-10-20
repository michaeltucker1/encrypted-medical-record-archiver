CC = gcc
CFLAGS = -Wall -std=c90
OBJS = main.o record.o encrypt.o compress.o
TARGET = medical_archiver

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c record.h encrypt.h compress.h
	$(CC) $(CFLAGS) -c main.c

record.o: record.c record.h
	$(CC) $(CFLAGS) -c record.c

encrypt.o: encrypt.c encrypt.h
	$(CC) $(CFLAGS) -c encrypt.c

compress.o: compress.c compress.h
	$(CC) $(CFLAGS) -c compress.c

clean:
	rm -f $(OBJS) $(TARGET)
