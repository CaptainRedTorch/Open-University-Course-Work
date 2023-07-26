CFLAGS = -ansi -Wall -pedantic -g

prog: preAsmblr.o macro_table.o tables.o helper_func.o firstPass.o main.o errorHandle.o secondPass.o
	gcc $(CFLAGS) preAsmblr.o macro_table.o tables.o helper_func.o errorHandle.o main.o firstPass.o secondPass.o -o prog
main.o: main.c preAsmblr.h firstPass.h constants.h
	gcc $(CFLAGS) -c main.c -o main.o
preAsmblr.o: preAsmblr.c preAsmblr.h macro_table.h helper_func.h errorHandle.h
	gcc $(CFLAGS) -c preAsmblr.c -o preAsmblr.o
macro_table.o: macro_table.c macro_table.h
	gcc $(CFLAGS) -c macro_table.c -o macro_table.o
tables.o: tables.c tables.h helper_func.h constants.h globals.h
	gcc $(CFLAGS) -c tables.c -o tables.o
firstPass.o: firstPass.c firstPass.h helper_func.h tables.h errorHandle.h constants.h globals.h
	gcc $(CFLAGS) -c firstPass.c -o firstPass.o
secondPass.o:	secondPass.c secondPass.h tables.h errorHandle.h constants.h globals.h
	gcc $(CFLAGS) -c secondPass.c -o secondPass.o
helper_func.o: helper_func.c helper_func.h constants.h globals.h
	gcc $(CFLAGS) -c helper_func.c -o helper_func.o
errorHandle.o: errorHandle.c errorHandle.h constants.h globals.h
	gcc $(CFLAGS) -c errorHandle.c -o errorHandle.o

clean:
