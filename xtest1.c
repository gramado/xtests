/*
 * File: xtest1.c
 *
 *   Simple Xlib application drawing a box in a window.
 *   
 *   To Compile: 
 *       gcc -O2 -Wall -o xtest1 xtest1.c -L /usr/X11R6/lib -lX11 -lm
 *   
 *   Credits: 
 *      http://xwindow.angelfire.com/page28.html
 */
 

// X
#include <X11/Xlib.h>

// libc

#include <stdio.h>
#include <stdlib.h> // prevents error for exit on line 18 when compiling with gcc



int main (int argc, char * argv[]){
	
    Display *d;
   int s;
   Window w;
   XEvent e;
 
    /* open connection with the server */
	
    d = XOpenDisplay (NULL);
	
    if ( d==NULL )
	{
        printf ("Cannot open display\n");
        exit (1);
    }
	
    s = DefaultScreen (d);
 
    /* create window */

    w = XCreateSimpleWindow ( d, RootWindow(d, s), 
            10, 10, 320, 200, 
            1, BlackPixel(d,s), WhitePixel(d,s) );

    // Prosses Window Close Event through event handler so XNextEvent does Not fail
    
	Atom delWindow = XInternAtom ( d, "WM_DELETE_WINDOW", 0 );
    
	XSetWMProtocols ( d, w, &delWindow, 1 );
 
   /* select kind of events we are interested in */
   XSelectInput ( d, w, ExposureMask | KeyPressMask );
 
   /* map (show) the window */
   XMapWindow ( d, w );
 
    //    
    // Event loop 
    //
	
	int screen_id = DefaultScreen (d);
	Window root = RootWindow (d, screen_id);
	
	GC gc;
	
	gc = XCreateGC ( d, root, 0, 0);
	
	int x = 1;
	int y = 1;
	
    while (1) 
    {
        XNextEvent ( d, &e );
		
        /* draw or redraw the window */
        
		if ( e.type == Expose )
		{
            XFillRectangle ( d, w, DefaultGC(d, s), 20, 20, 10, 10 );
        }
		
        /* exit on key press */
        if (e.type == KeyPress)
		{
			//Isso escreve no arquivo de saida e vai mostrar depois no terminal
			//printf ("test");
			
	        //XDrawString (text_box.display, text_box.window, text_box.gc,
            //     x, y, text_box.text, text_box.text_len);
			
	        //XDrawString ( d, root, gc, x, y, "Hello World", 11);
			//XDrawText ( d, 1, gc, x, y, "C", 1);
				
			//XDrawText(display, d, gc, x, y, items, nitems)
			//XDrawString(display, d, gc, x, y, string, length)	
			
			break;
		}
            
 
        // Handle Windows Close Event
        if( e.type == ClientMessage )
            break;
    };
 
    /* destroy our window */
    XDestroyWindow ( d, w );
 
    /* close connection to server */
    XCloseDisplay (d);
 
	//exit (0);
    return 0;
}




