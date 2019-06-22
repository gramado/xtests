#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ascii.h"
#include "vk.h"
#include "kbdabnt2.h"
 
int main()
{
    Display *display;
    Window window;
    XEvent event;
    int s;
    
    GC gc;
    
    char *text;
    int text_len;    
 
    char buffer[512];
    
    
    int x = 0;
    int y = 1;
    
    char SC = 0;
 
    /* open connection with the server */
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
 
    s = DefaultScreen(display);
 
    /* create window */
    window = XCreateSimpleWindow ( display, RootWindow(display, s), 
                 10, 10, 640, 320, 
                 1, BlackPixel(display, s), WhitePixel(display, s));
 
    /* select kind of events we are interested in */
    XSelectInput(display, window, KeyPressMask | KeyReleaseMask );
 
    /* map (show) the window */
    XMapWindow(display, window);
    
    
    gc = XCreateGC ( display, window, 0, 0);    
 
    /* event loop */
    while (1)
    {
        XNextEvent(display, &event);
 
        /* keyboard events */
        if (event.type == KeyPress)
        {        
            /* exit on ESC key press */
            if ( event.xkey.keycode != 0x09 )
            {
				
                text = &buffer[0];
                //SC = (event.xkey.keycode & 0x7f );
                SC = event.xkey.keycode;
                SC = SC - 8;
                buffer[0] = map_abnt2[SC]; 
                buffer[1] = 0;
			    //text_len = strlen (text);
			    text_len = 1;
			    //draw_char ( 4*16, 4*16 );	
			    XDrawString ( display, window, gc,
                    x*8, y*16, text, text_len );
                           
                x++;
                
                if (x > 40)
                {
					y++;
					x = 0;
				}				
				
				
				
			}else{
			    break;	
			}
                               
                
        }else if (event.type == KeyRelease){
			
                //printf( "KeyRelease: %x\n", event.xkey.keycode );
			    //text = "KeyRelease";
			    //text_len = strlen (text);
			    //draw_char ( 4*16, 4*16 );	
			    //XDrawString ( display, window, gc,
                //    4*16, 4*16, text, text_len );
        }
    }

    /* close connection to server */
    XCloseDisplay(display);
 
    return 0;
}
