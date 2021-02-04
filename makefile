CFLAGS = -Wall -Wextra -pedantic -ansi -O2 -lm -o awesome_out 
CC = gcc
LDLIBS = -lm
BASE =  main_code
BASE2 = helper
EXECS = main_code

main_code : $(BASE).c $(BASE).h $(BASE2).c $(BASE2).h
	$(CC) $(BASE).c $(BASE2).c $(CFLAGS) $(LDLIBS) -o $@
