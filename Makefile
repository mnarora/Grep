grep: grep.o functions.o list.o
	cc functions.o grep.o list.o -o grep -Wall
grep.o: grep.c functions.h list.h
	cc -c grep.c -Wall
functions.o: functions.c functions.h
	cc -c functions.c
list.o: list.c list.h
	cc -c list.c -Wall
clean:
	rm *.o
