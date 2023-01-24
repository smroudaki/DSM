CC := gcc
C_FLAGS := \
	-no-pie \
	-pthread

MAIN := main.c
EXECUTABLE := main

MASTER_IP := 0.0.0.0
CLIENT_IP := 0.0.0.0

master: build
	./$(EXECUTABLE) master $(MASTER_IP) $(CLIENT_IP)

client: build
	./$(EXECUTABLE) client $(MASTER_IP) $(CLIENT_IP)

build: dsm.o
	${CC} $(C_FLAGS) dsm.o $(MAIN) -o $(EXECUTABLE)

dsm.o: dsm.c dsm.h
	${CC} -c dsm.c

clean:
	rm -f *.o $(EXECUTABLE)
