CC=gcc
CFLAGS=-c -g -pedantic -Wall -Werror
LDFLAGS=
OBJ= sjf.o LList.o proc.o Gantt.o simcalc.o sjfsim.o

sjf : $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o sjf

sjf.o : sjf.c ../sjfsim.h ../LList.h ../common.h
	$(CC) $(CFLAGS) sjf.c

sjfsim.o : ../sjfsim.h ../sjfsim.c ../Gantt.c ../proc.h ../LList.h ../common.h ../simcalc.h
	$(CC) $(CFLAGS) ../sjfsim.c -o sjfsim.o

simcalc.o : ../simcalc.h ../simcalc.c ../Gantt.h ../LList.h ../common.h ../proc.h
	$(CC) $(CFLAGS) ../simcalc.c -o simcalc.o

Gantt.o : ../Gantt.h ../Gantt.c ../LList.h ../common.h
	$(CC) $(CFLAGS) ../Gantt.c -o Gantt.o

proc.o : ../proc.h ../proc.c ../LList.h ../common.h
	$(CC) $(CFLAGS) ../proc.c -o proc.o

LList.o : ../LList.h ../LList.c ../common.h
	$(CC) $(CFLAGS) ../LList.c -o LList.o

clean :
	rm $(OBJ) sjf
