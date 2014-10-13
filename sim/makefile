CC=gcc
CFLAGS=-c -g -pedantic -Wall
LDFLAGS=-pthread
OBJ=simulator.o Gantt.o LList.o proc.o rrsim.o simcalc.o sjfsim.o 


simulator: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o simulator

simulator.o : simulator.c ../rrsim.h ../sjfsim.h ../common.h ../LList.h
	$(CC) $(CFLAGS) simulator.c -o simulator.o

sjfsim.o : ../sjfsim.h ../sjfsim.c ../Gantt.c ../proc.h ../LList.h ../common.h ../simcalc.h
	$(CC) $(CFLAGS) ../sjfsim.c -o sjfsim.o

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
	rm $(OBJ) simulator 
