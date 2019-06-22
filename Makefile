



all:

	# xtest1
	gcc -O2 -Wall -o xtest1 xtest1.c -L /usr/X11R6/lib -lX11 -lm
	gcc -O2 -Wall -o xtest2 xtest2.c -L /usr/X11R6/lib -lX11 -lm
	gcc -O2 -Wall -o xtest3 xtest3.c -L /usr/X11R6/lib -lX11 -lm	
	
	
	
	
	
