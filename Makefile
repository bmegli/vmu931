VMU931 = vmu931.o
EXAMPLES = vmu931-euler vmu931-read-all vmu931-calibrate vmu931-estimate-bias
CC = gcc
CXX = g++
DEBUG = 
CFLAGS = -O2 -Wall -c $(DEBUG)
CXX_FLAGS = -O2 -Wall -c $(DEBUG)
LFLAGS = -O2 -Wall $(DEBUG)

vmu931.o : vmu931.h vmu931.c
	$(CC) $(CFLAGS) vmu931.c
		
all : $(VMU931) $(EXAMPLES)
	
examples: $(EXAMPLES)
			
vmu931-euler : vmu931.o vmu931-euler.o
	$(CC) $(LFLAGS) vmu931.o vmu931-euler.o -o vmu931-euler
		
vmu931-euler.o : vmu931.h examples/vmu931-euler.c
	$(CC) $(CFLAGS) examples/vmu931-euler.c

vmu931-read-all : vmu931.o vmu931-read-all.o
	$(CC) $(LFLAGS) vmu931.o vmu931-read-all.o -o vmu931-read-all

vmu931-read-all.o : vmu931.h examples/vmu931-read-all.c
	$(CC) $(CFLAGS) examples/vmu931-read-all.c

vmu931-calibrate : vmu931.o vmu931-calibrate.o
	$(CC) $(LFLAGS) vmu931.o vmu931-calibrate.o -o vmu931-calibrate

vmu931-calibrate.o : vmu931.h examples/vmu931-calibrate.c
	$(CC) $(CFLAGS) examples/vmu931-calibrate.c

vmu931-estimate-bias : vmu931.o vmu931-estimate-bias.o
	$(CC) $(LFLAGS) vmu931.o vmu931-estimate-bias.o -o vmu931-estimate-bias

vmu931-estimate-bias.o : vmu931.h examples/vmu931-estimate-bias.c
	$(CC) $(CFLAGS) examples/vmu931-estimate-bias.c

		
clean:
	\rm -f *.o examples/*.o $(VMU931) $(EXAMPLES)
