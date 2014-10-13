CC=gcc
CFLAGS=-c -g -pedantic -Wall -Werror
LDFLAGS=
OBJ= rr.o LList.o proc.o Gantt.o simcalc.o rrsim.o

rr : $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o rr

rr.o : rr.c ../rrsim.h ../LList.h ../common.h
	$(CC) $(CFLAGS) rr.c

rrsim.o : ../rrsim.h ../rrsim.c ../LList.h ../proc.h ../simcalc.h ../common.h
	$(CC) $(CFLAGS) ../rrsim.c -o rrsim.o

simcalc.o : ../simcalc.h ../simcalc.c ../Gantt.h ../LList.h ../common.h ../proc.h
	$(CC) $(CFLAGS) ../simcalc.c -o simcalc.o

Gantt.o : ../Gantt.h ../Gantt.c ../LList.h ../common.h
	$(CC) $(CFLAGS) ../Gantt.c -o Gantt.o

proc.o : ../proc.h ../proc.c ../LList.h ../common.h
	$(CC) $(CFLAGS) ../proc.c -o proc.o

LList.o : ../LList.h ../LList.c ../common.h
	$(CC) $(CFLAGS) ../LList.c -o LList.o

clean :
	rm $(OBJ) rr
