CC = gcc
CFLAGS =  -Wall -O1 -g

OBJS = mdriver.o mm.o memlib.o fsecs.o fcyc.o clock.o ftimer.o
OBJS_TEST = test.o mm.o memlib.o 

mdriver: $(OBJS)
	$(CC) $(CFLAGS) -o mdriver $(OBJS)

test: $(OBJS_TEST)
	$(CC) $(CFLAGS) -o mtest $(OBJS_TEST)



mm.o: mm.c mm.h memlib.h
test.o: mm.c mm.h test.c memlib.h

clean:
	rm -f *~ mm.o mdriver mtest


