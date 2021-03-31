CC = gcc
TARGET = hw1
OBJECTS = sfpsave.o hw1.o

$(TARGET) : hw1.o sfpsave.o
	$(CC) -o $(TARGET) $(OBJECTS)

hw1.o : hw1.c
	$(CC) -c -o hw1.o hw1.c

sfpsave.o : sfpsave.c
	$(CC) -c -o sfpsave.o sfpsave.c

clean :
	rm *.o $(TARGET)
