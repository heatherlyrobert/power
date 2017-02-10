/*============================--- beg-of-code ---=============================*/


/*===[[ SUMMARY ]]=============================================================*

 *   khronos is a fast, simplified, modernized, and technical version of the
 *   classic posix-defined crond time-based process scheduler which combines
 *   crond and crontab to allow deeper verification, verbosity, and traceablity.
 *
 */

#include   <yX11.h>                    /* heatherly xlib/glx setup            */
#include   <yFONT.h>                   /* heatherly text display for opengl   */
#include   <yCOLOR.h>                  /* heatherly opengl color handling     */

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <unistd.h>



/* rapidly evolving version number to aid with visual change confirmation     */
#define     YFONT_VER_NUM   "2.0b"
#define     YFONT_VER_TXT   "moved to REDBLU color scheme in yCOLOR library"



#define     LEN_LABEL     20

typedef     struct cACCESSOR  tACCESSOR;
struct cACCESSOR {
   char        face        [LEN_LABEL];
   char        font;
   char        status;
   char        exist;
   int         updates;
   int         events;
   int         capacity;
   int         ncolor;
   double      percent;
   int         level;
   char        w_title     [LEN_LABEL];
   int         w_wide;
   int         w_tall;
};
extern   tACCESSOR   my;


/*============================----end-of-code ---=============================*/
