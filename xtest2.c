/*
 * File: xtest1.c
 *
 *   Simple Xlib application drawing a box in a window.
 *   
 *   To Compile: 
 *       gcc -O2 -Wall -o xtest2 xtest2.c -L /usr/X11R6/lib -lX11 -lm
 *   
 *   Credits: 
 *      https://www.lemoda.net/c/xlib-text-box/
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

/* The window which contains the text. */

struct {
    int width;
    int height;
    char * text;
    int text_len;

    /* X Windows related variables. */

    Display *display;
    int screen;
    Window root;
    Window window;
    GC gc;
    XFontStruct * font;
    unsigned long black_pixel;    
    unsigned long white_pixel;
} text_box;



static void deal_with_keys( XKeyEvent *event){
	
    int count;
    int buffer_size = 80;
    char buffer[80];
    KeySym keysym;

    /* XComposeStatus compose; is not used, though it's in some books */
   count = XLookupString(event,buffer,buffer_size, &keysym);
   
   if ((keysym >= XK_space) && (keysym <= XK_asciitilde)){
	   
      printf ("Ascii key:- ");
      if (event->state & ShiftMask)
             printf("(Shift) %s\n", buffer);
      else if (event->state & LockMask)
             printf("(Caps Lock) %s\n", buffer);
      else if (event->state & ControlMask)
             printf("(Control) %c\n", 'a'+ buffer[0]-1) ;
      else printf("%s\n", buffer) ;
      
      
   }
   else if ((keysym >= XK_Shift_L) && (keysym <= XK_Hyper_R)){
      printf ("modifier key:- ");
      switch (keysym){
      case XK_Shift_L: printf("Left Shift\n"); break;
      case XK_Shift_R: printf("Right Shift\n");break;
      case XK_Control_L: printf("Left Control\n");break;
      case XK_Control_R: printf("Right Control\n");	break;
      case XK_Caps_Lock: printf("Caps Lock\n");	break;
      case XK_Shift_Lock: printf("Shift Lock\n");break;
      case XK_Meta_L: printf("Left Meta\n");	break;
      case XK_Meta_R: printf("Right Meta\n");	break;

      }
      
      
    }
   else if ((keysym >= XK_Left) && (keysym <= XK_Down)){
      printf("Arrow Key:-");
      switch(keysym){
      case XK_Left: printf("Left\n");break;
      case XK_Up: printf("Up\n");break;
      case XK_Right: printf("Right\n");break;
      case XK_Down: printf("Down\n");break;	
      }
      
      
    }
   else if ((keysym >= XK_F1) && (keysym <= XK_F35)){
      printf ("function key %d pressed\n", keysym - XK_F1);

      if (buffer == NULL)
         printf("No matching string\n");
      else
         printf("matches <%s>\n",buffer);
   
   
   }
   else if ((keysym == XK_BackSpace) || (keysym == XK_Delete)){
      printf("Delete\n");
   
   }
   else if ((keysym >= XK_KP_0) && (keysym <= XK_KP_9)){
       printf("Number pad key %d\n", keysym -  XK_KP_0);
   
   }
   else if (keysym == XK_Break) {
        printf("closing display\n"); 
        XCloseDisplay (text_box.display);
        //XCloseDisplay(display); 
        exit (0);
   
   }else{
      printf("Not handled\n");
   }
}




/* Connect to the display, set up the basic variables. */

static void x_connect (){
	
    text_box.display = XOpenDisplay (NULL);
    
    if ( !text_box.display )
    {
        fprintf (stderr, "Could not open display.\n");
        exit (1);
    }
    
    text_box.screen = DefaultScreen (text_box.display);
    text_box.root = RootWindow (text_box.display, text_box.screen);
    text_box.black_pixel = BlackPixel (text_box.display, text_box.screen);
    text_box.white_pixel = WhitePixel (text_box.display, text_box.screen);
}


/* Create the window. */

static void create_window (){
	
    text_box.width = 640;
    text_box.height = 320;
    
    text_box.window = XCreateSimpleWindow (text_box.display, text_box.root,
                             1, /* x */
                             1, /* y */
                             text_box.width, text_box.height,
                             0, /* border width */
                             text_box.black_pixel, /* border pixel */
                             text_box.white_pixel  /* background */ );
                             
    
    //#todo: Validation
    
    XSelectInput (text_box.display, text_box.window, ExposureMask);
    
    XMapWindow (text_box.display, text_box.window);
}


/* Set up the GC (Graphics Context). */

static void set_up_gc (){
	
    text_box.screen = DefaultScreen (text_box.display);
    
    text_box.gc = XCreateGC (text_box.display, text_box.window, 0, 0);
    
    XSetBackground (text_box.display, text_box.gc, text_box.white_pixel); 
    XSetForeground (text_box.display, text_box.gc, text_box.black_pixel); 
}


/* Set up the text font. */

static void set_up_font (){
	
    const char * fontname = "-*-helvetica-*-r-*-*-14-*-*-*-*-*-*-*";
    
    text_box.font = XLoadQueryFont (text_box.display, fontname);
    
    /* If the font could not be loaded, revert to the "fixed" font. */
    
    if (! text_box.font)
    {
        fprintf (stderr, "unable to load font %s: using fixed\n", fontname );
        text_box.font = XLoadQueryFont (text_box.display, "fixed");
    }
    
    XSetFont (text_box.display, text_box.gc, text_box.font->fid);
}


/* Draw the window. */

static void draw_screen (){
	
    int x;
    int y;
    
    int direction;
    int ascent;
    int descent;
    
    XCharStruct overall;

    /* Centre the text in the middle of the box. */

    XTextExtents ( text_box.font, text_box.text, text_box.text_len,
        &direction, &ascent, &descent, &overall );
        
    x = (text_box.width - overall.width) / 2;
    y = text_box.height / 2 + (ascent - descent) / 2;
	
    XClearWindow (text_box.display, text_box.window);
    
	XDrawString (text_box.display, text_box.window, text_box.gc,
        x, y, text_box.text, text_box.text_len );
}


static void draw_char ( int x, int y )
{
	XDrawString (text_box.display, text_box.window, text_box.gc,
        x, y, text_box.text, text_box.text_len );
}

/* Loop over events. */

static void event_loop ()
{
    int count;
    int buffer_size = 80;
    char buffer[80];
    KeySym keysym;

	
	
    while (1) 
	{
        XEvent e;
        XNextEvent (text_box.display, &e);
        
		//XNextEvent ( d, &e );
		
		
		if (e.type == Expose) 
		{
			text_box.text = "x";
            //text_box.text_len = strlen (text_box.text);			
            text_box.text_len = 1;
            
            // Coloca a string na tela. (char ou string)
            //draw_screen ();
            draw_char ( 2*16, 2*16 );
        }
        
        
        //if (e.type == KeyRelease) 
			//deal_with_keys(&event);
			  
		if (e.type == KeyPress)
		{
            /* XComposeStatus compose; is not used, though it's in some books */
            count = XLookupString(e,buffer,buffer_size, &keysym);			
			//deal_with_keys(&e);
		    
		    //if (keysym == XK_F1)
		    //{
			    text_box.text = "XK_F1";
			    //text_box.text_len = 1;
			    text_box.text_len = strlen (text_box.text);
			    draw_char ( 4*16, 4*16 );	
			//}
		
		}	  
			
		/*	        
		if (e.type == KeyPress) 
		{

			
			text_box.text = "x";
            //text_box.text_len = strlen (text_box.text);			
            text_box.text_len = 1;
            
            // Coloca a string na tela. (char ou string)
            //draw_screen ();
            draw_char ( 2*16, 2*16 );
        }*/
        
        // Handle Windows Close Event
        if( e.type == ClientMessage )
            break;        
    }
}


int main (int argc, char **argv){
	
    text_box.text = "Hello World";
    text_box.text_len = strlen (text_box.text);
    
    x_connect ();
    create_window ();
    set_up_gc ();
    set_up_font ();
    event_loop ();
    
     /* destroy our window */
    XDestroyWindow ( text_box.display, text_box.window );
 
    /* close connection to server */
    XCloseDisplay (text_box.display);       
    
    return 0;
}
