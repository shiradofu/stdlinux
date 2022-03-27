%: %.c
	gcc -Wall -o $* $*.c

%,g: %.c
	gcc -Wall -g -o $* $*.c
