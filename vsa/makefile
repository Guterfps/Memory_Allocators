
CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra

TARGET = vsa

all: debug

debug: CFLAGS += -g
debug: $(TARGET)

release: CFLAGS += -DNDEBUG -O3
release: $(TARGET)

$(TARGET): $(TARGET).o $(TARGET)_test.o
		$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).o $(TARGET)_test.o
		
$(TARGET).o: $(TARGET).c $(TARGET).h
		$(CC) $(CFLAGS) -c $(TARGET).c

$(TARGET)_test.o: $(TARGET)_test.c $(TARGET).h
		$(CC) $(CFLAGS) -c $(TARGET)_test.c
clean:
		$(RM) $(TARGET).out *.o
