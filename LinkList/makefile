CC=g++
CFLAGS= -g -O3 -Wall -Wextra

TARGET=main

OBJS=main.o LinkClass.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	
$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@ 

clean:
	rm $(TARGET)
	rm -rf *.o

cleanO:
	rm -rf *.o
