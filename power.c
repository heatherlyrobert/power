/*============================[[--- beg-code ---]]============================*/
#include    "power.h"



tACCESSOR   my;



/*============================--------------------============================*/
/*===----                              fonts                           ----===*/
/*============================--------------------============================*/
static void      o___FONTS___________________o (void) {;}

char
FONT_load            (void)
{
   my.font  = yFONT_load(my.face);
   if (my.font <  0) {
      fprintf (stderr, "Problem loading %s\n", my.face);
      exit    (1);
   }
   return 0;
}

char
FONT_free            (void)
{
   yFONT_free (my.font);
   return 0;
}



/*============================--------------------============================*/
/*===----                        opengl drawing                        ----===*/
/*============================--------------------============================*/
static void      o___DRAWING_________________o (void) {;}

char
DRAW_init          (void)
{
   glClearColor  (1.0f, 1.0f, 1.0f, 1.0f);
   glClearDepth  (1.0f);
   glShadeModel  (GL_SMOOTH);
   glEnable      (GL_TEXTURE_2D);    /* NEW */
   glEnable      (GL_DEPTH_TEST);
   glEnable      (GL_ALPHA_TEST);
   glAlphaFunc   (GL_GEQUAL, 0.0125);
   glEnable      (GL_BLEND);
   glBlendFunc   (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint        (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /* this causes diagonal line artifacts in polygons on nvidia (not ati) */
   /*> glEnable      (GL_POLYGON_SMOOTH);                                             <*/
   glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
   glHint        (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   glFlush       ();
   /*---(complete)-----------------------*/
   return 0;
}

char
DRAW_resize        (uint a_w, uint a_h)
{
   if (a_h == 0) a_h = 1;
   glViewport(0,  0, my.w_wide, my.w_tall);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(        0,   my.w_wide, 0, my.w_tall, -100.0,  100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   return 0;
}

char       /* PURPOSE : place the texture in the window ----------------------*/
DRAW_main          (void)
{
   /*---(locals)-------------------------*/
   float     z             = 0;
   float     x_red, x_grn, x_blu;
   float     x_inc         = 0.0;
   float     x_top         = 0.0;
   float     x_bot         = 0.0;
   int       i             = 0;
   char      msg           [100];
   /*---(prepare drawing)----------------*/
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   /*---(draw status background)---------*/
   yCOLOR_deg2color (my.level * 10);
   glBegin(GL_POLYGON); {
      glVertex3f   (    0,     0,    z);
      glVertex3f   (  100,     0,    z);
      glVertex3f   (  100,  my.w_tall,   z);
      glVertex3f   (    0,  my.w_tall,   z);
   } glEnd();
   /*---(draw scale squares)-------------*/
   z += 5;
   x_inc = (double) my.w_tall / (double) my.ncolor;
   /*> printf ("win height = %3d\n", my.w_tall);                                          <*/
   /*> printf ("win inc    = %4.1f\n", x_inc);                                        <*/
   for (i = 0; i < my.ncolor; ++i) {
      /*> printf ("color index= %3d\n", my.ncolor - i);                                <*/
      yCOLOR_deg2color (i * 10);
      x_top = (i + 1) * x_inc;
      x_bot = (i    ) * x_inc;
      /*> printf ("x_top      = %4.1f\n", x_top);                                     <*/
      /*> printf ("x_bot      = %4.1f\n", x_bot);                                     <*/
      glBegin (GL_POLYGON); {
         glVertex3f   (        0, x_top,  z);
         glVertex3f   (my.w_wide / 3, x_top,  z);
         glVertex3f   (my.w_wide / 3, x_bot,  z);
         glVertex3f   (        0, x_bot,  z);
      } glEnd();
   }
   /*---(print big percent)--------------*/
   glTranslatef( my.w_wide / 2, my.w_tall / 2 + 3,  15.0);
   sprintf (msg, "%4.1lf", my.percent);
   glColor4f (0.0, 0.0, 0.0, 1.0);
   yFONT_print (my.font, 16, YF_MIDCEN, msg);
   /*---(print adapter status)-----------*/
   glColor4f (0.0, 0.0, 0.0, 1.0);
   glTranslatef( 0, -my.w_tall / 2,   0.0);
   glRotatef(  90.0,  0.0,  0.0,  1.0);
   glTranslatef(   5, 0.0,   0.0);
   if      (my.status == 'b')    strcpy (msg, "battery");
   else if (my.status == 'p')    strcpy (msg, "adapter");
   else if (my.status == 'f')    strcpy (msg, "full");
   else                       strcpy (msg, "unknown");
   yFONT_print (my.font, 12, YF_MIDLEF, msg);
   /*---(print battery status)-----------*/
   glTranslatef( 230,  0,   0.0);
   if      (my.exist  == 'y')    strcpy (msg, "present");
   else if (my.exist  == 'n')    strcpy (msg, "missing");
   else                       strcpy (msg, "unknown");
   yFONT_print (my.font, 12, YF_MIDRIG, msg);
   /*---(force the redraw)---------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   /*---(complete)-------------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                         acpi battery                         ----===*/
/*============================--------------------============================*/
static void      o___BATTERY_________________o (void) {;}

char
ACPI_max           (void)
{
   char        recd        [1000];
   int         len         = 0;
   char       *p           = NULL;
   char       *s           = NULL;
   FILE       *info        = NULL;
   FILE       *acpi        = NULL;
   /*---(open info file)-----------------*/
   info = fopen ("/proc/acpi/battery/BAT0/info" , "r");
   if (info == NULL) {
      printf ("FATAL, can not read battery info file\n");
      return -1;
   }
   /*---(read for capacity)--------------*/
   while (1) {
      fgets (recd, 1000, info);
      if (recd == NULL)  break;
      len = strlen (recd);
      p = strtok_r (recd, ":", &s);
      if (p == NULL)  continue;
      if (strcmp (p, "design capacity") != 0) continue;
      p = strtok_r (NULL, " ", &s);
      my.capacity = atoi (p);
      /*> printf ("capacity  = <<%s>> = %d\n", p, my.capacity);                          <*/
      break;
   }
   fclose (info);
   if (my.capacity == 0) {
      printf ("FATAL, can not get design capacity from info file\n");
      return -2;
   }
}

char
ACPI_status        (void)
{
   char        recd        [1000];
   int         len         = 0;
   char       *p           = NULL;
   char       *s           = NULL;
   FILE       *info        = NULL;
   FILE       *acpi        = NULL;
   int         remaining   = 0;
   int         value       = 0;
   /*---(open state file)----------------*/
   acpi = fopen ("/proc/acpi/battery/BAT0/state", "r");
   if (acpi == NULL) {
      printf ("FATAL, can not read battery state file\n");
      return -3;
   }
   /*---(read for state)-----------------*/
   my.status = 'b';
   my.exist  = 'n';
   while (1) {
      fgets (recd, 1000, acpi);
      if (feof (acpi)) break;
      if (recd == NULL)  break;
      len = strlen (recd);
      p = strtok_r (recd, ":", &s);
      if (p == NULL)  continue;
      if (strcmp (p, "remaining capacity") == 0) {
         p = strtok_r (NULL, " ", &s);
         remaining = atoi (p);
         /*> printf ("remaining = <<%s>> = %d\n", p, remaining);                         <*/
      }
      if (strcmp (p, "charging state") == 0) {
         p = strtok_r (NULL, " ", &s);
         p [strlen(p) - 1] = '\0';
         if        (strcmp (p, "charging") == 0) {
            my.status = 'p';
         } else if (strcmp (p, "charged") == 0) {
            my.status = 'f';
         }
         /*> printf ("remaining = <<%s>> = %d\n", p, remaining);                         <*/
      }
      if (strcmp (p, "present") == 0) {
         p = strtok_r (NULL, " ", &s);
         p [strlen(p) - 1] = '\0';
         if (strcmp (p, "yes") == 0) {
            my.exist  = 'y';
         }
         /*> printf ("remaining = <<%s>> = %d\n", p, remaining);                         <*/
      }
   }
   fclose (acpi);
   if (remaining == 0) {
      printf ("FATAL, can not get remaining capacity from info file\n");
      return -4;
   }
   /*---(calculate percent)--------------*/
   my.percent = ((float) remaining / (float) my.capacity) * 100;
   my.level   = (my.ncolor - 1) * my.percent / 100;
   /*> printf ("s_percent  = %4.1f\n", my.percent);                                    <*/
   /*> printf ("my.level    = %3d\n", my.level);                                        <*/
   return 0;
}



/*============================--------------------============================*/
/*===----                           drivers                            ----===*/
/*============================--------------------============================*/
static void      o___DRIVERS_________________o (void) {;}

char        /* PURPOSE : process the xwindows event stream                    */
PROG_event    (void)
{
   /*---(locals)--------------------------------*/
   XKeyEvent *key_event;
   char       the_key[5];
   int        the_bytes;
   /*---(event loop)----------------------------*/
   while (1) {
      if (XPending(DISP)) {
         ++my.events;
         XNextEvent(DISP, &EVNT);
         /*---(start)----------------------------*/
         switch(EVNT.type) {

         case Expose:
            break;

         case ConfigureNotify:
            break;

         case KeyPress:
            /*---(get key)------------------*/
            key_event  = (XKeyEvent *) &EVNT;
            the_bytes = XLookupString ((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
            if (the_bytes < 1) break;
            /*---(vi mode changes)----------*/
            switch (the_key[0]) {
            case 'Q': return 1; break;
            }
         }
      } else {
         /*---(check for automation)--------*/
         ACPI_status ();
         ++my.updates;
         DRAW_main ();
         sleep (1);
      }
   }
   /*---(complete)------------------------------*/
   return 0;
}

int
main               (void)
{
   /*---(graphics)-----------------------*/
   my.w_wide =  50;
   my.w_tall = 250;
   strcpy    (my.w_title, "power_dot");
   yXINIT_start (my.w_title, my.w_wide, my.w_tall, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   DRAW_init    ();
   DRAW_resize  (my.w_wide, my.w_tall);
   ACPI_max     ();
   my.updates  = 0;
   my.events   = 0;
   strcpy    (my.face, "clarity");
   my.ncolor  = yCOLOR_init  (YCOLOR_SMALL);
   FONT_load    ();
   PROG_event   ();
   FONT_free    ();
   yXINIT_end   ();
   /*---(complete)-----------------------*/
   return 0;
}


/*============================[[--- end-code ---]]============================*/
