CC            = gcc
CFLAGS        = -O0 -Wall -I/usr/local/include
LDFLAGS       = -L/usr/local/lib
LIBS          = -lwiringPi
OBJS          = simpleAY.o
PROGRAM       = simpleAY

all:			$(PROGRAM)

$(PROGRAM):		$(OBJS)
				$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

clean:;			rm -f *.o *~ $(PROGRAM)
