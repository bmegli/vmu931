VMU931 = vmu931.o
#EXAMPLES = vmu931-test
CC = gcc
CXX = g++
DEBUG = 
CFLAGS = -O2 -Wall -c $(DEBUG)
CXX_FLAGS = -O2 -Wall -c $(DEBUG)
LFLAGS = -O2 -Wall $(DEBUG)

vmu931.o : vmu931.h vmu931.c
	$(CC) $(CFLAGS) vmu931.c
		
all : $(VMU931) # $(EXAMPLES)
	
#examples: $(EXAMPLES)
			
#vmu931-test : vmu931.o vmu931-test.o
#	$(CC) $(LFLAGS) vmu931.o vmu931-test.o -o vmu931-test
		
#vmu931-test.o : vmu931.h examples/vmu931-test.c
#	$(CC) $(CFLAGS) examples/vmu931-test.c
		
clean:
	\rm -f *.o examples/*.o $(VMU931) # $(EXAMPLES)
