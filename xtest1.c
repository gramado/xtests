 
  /*
   Simple Xlib application drawing a box in a window.
   To Compile: gcc -O2 -Wall -o test test.c -L /usr/X11R6/lib -lX11 -lm
 */
 
 #include<X11/Xlib.h>
 #include<stdio.h>
 #include<stdlib.h> // prevents error for exit on line 18 when compiling with gcc

 int main() {
   Display *d;
   int s;
   Window w;
   XEvent e;
 
                        /* open connection with the server */
   d=XOpenDisplay(NULL);
   if(d==NULL) {
     printf("Cannot open display\n");
     exit(1);
   }
   s=DefaultScreen(d);
 
                        /* create window */
   w=XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
                         BlackPixel(d, s), WhitePixel(d, s));
 
   // Prosses Window Close Event through event handler so XNextEvent does Not fail
        Atom delWindow = XInternAtom( d, "WM_DELETE_WINDOW", 0 );
        XSetWMProtocols(d , w, &delWindow, 1);
 
                        /* select kind of events we are interested in */
   XSelectInput(d, w, ExposureMask | KeyPressMask);
 
                        /* map (show) the window */
   XMapWindow(d, w);
 
                        /* event loop */
   while(1) {
     XNextEvent(d, &e);
                        /* draw or redraw the window */
     if(e.type==Expose) {
       XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
     }
                        /* exit on key press */
     if(e.type==KeyPress)
       break;
 
     // Handle Windows Close Event
     if(e.type==ClientMessage)
        break;
   }
 
                        /* destroy our window */
   XDestroyWindow(d, w);
 
                        /* close connection to server */
   XCloseDisplay(d);
 
   return 0;
 }
