# This is the makefile for the the RPI master application
# it uses:
## g++ as the compiler
## automatic dependency generation (-include $(OBJ:.o=.d) and -MD flag
#  -lwiringPi to include the RPI wiring libraries, only available on RPI 
#
# use target: all to build applicatin
# use target: test to build csv writer test, and uncomment -include line at end
# 

CC=g++
CFLAGS=-lwiringPi -MD
CTESTFLAGS=-MD
OBJ =  master.o  spi.o utilities.o

all: master


master: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp 
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.d

-include $(OBJ:.o=.d)

