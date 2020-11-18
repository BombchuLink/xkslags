#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/extensions/record.h>
#include <X11/extensions/XTest.h>



/*syntax stuff

print current group level
xkslags -g

set keyboard group to 1
xkslags -g 1

increase group level by 1. If above max (4), set to 0.
xkslags -g +1-4

decrease group level by 1. If below 0 set to 4.
xkslags -g -1-4

debug output
xkslags -d
If no args are provided, print out current group level
  */

int main (int argc, char *argv[])
{
	bool verbose = false;
	bool info = false;
	bool printgroup = false;
	bool gethelp = false;
	bool isneg = false;

	char Groupstring[4];
	char GroupMath;
	char GroupMax;
	int c;
	//get starting args in first - or set of -
	while ((c = getopt (argc, argv, ":hipvg:")) != -1)
	{
		switch (c)
		{
			case 'g':
				//load string
				for ( int a=0; a<4; a++) {
					Groupstring[a] = argv[2][a];
				}

				Display* dpy = XOpenDisplay(NULL);
				XkbStateRec kbd;
				XkbGetState ( dpy, XkbUseCoreKbd, &kbd );
				char currentgroup = kbd.group;

				//add or sub from group level
				if (Groupstring[0] == '-' || Groupstring[0] == '+') {

					//convert from ascii to num
					GroupMath = Groupstring[1] - 48;
					GroupMax = Groupstring[3] - 48;

					if (verbose) {
						printf ("Group Max to %d\n", GroupMax);
					}

					if (Groupstring[0] == '-') {
						isneg = true;
						GroupMath -= GroupMath*2;
					}
					if (verbose && isneg)
						printf ("Number to Decrease by is %d\n", GroupMath);
					if (verbose && !(isneg))
						printf ("Number to Increase by is %d\n", GroupMath);

					//actual sauce
					char SetGroup = currentgroup+GroupMath;
					//set underflow
					if (SetGroup < 0) {
						SetGroup = GroupMax + (SetGroup+1);
						printf ("underflow");
					}
					//set overflow
					if (SetGroup > GroupMax){
						SetGroup = 0;
						printf ("overflow");
					}
					XkbLockGroup (dpy, XkbUseCoreKbd, SetGroup);
					//if (XkbLockGroup (dpy, XkbUseCoreKbd, SetGroup))
						//printf ("Successfully added %d to Group to make it %d\n", GroupMath, SetGroup);
					if (verbose) {
						printf ("Setting Group to %d\n", SetGroup);
					}

				}
				//set group level
				else {
					//oops group math isn't inited here
					GroupMath = Groupstring[0] - 48;
					//latch group = deadkey
					//XkbLatchGroup (dpy, XkbUseCoreKbd, GroupMath);
					XkbLockGroup (dpy, XkbUseCoreKbd, GroupMath);
					if (verbose) {
						printf ("Setting Group to %d\n", GroupMath);
					}

				}

				XCloseDisplay(dpy);
				break;
			case 'h':
				gethelp = true;
				break;
			case 'i':
				info = true;
				break;
			case 'p':
				printgroup = true;
				break;
			case 'v':
				verbose = true;
				break;
			default:
				abort ();
		}
	}

	int currentgroup;
	Display* dpy = XOpenDisplay(NULL);
	XkbStateRec kbd;
	XkbGetState ( dpy, XkbUseCoreKbd, &kbd );
	currentgroup = kbd.group;
	if (printgroup) {
		fprintf( stdout, "%d\n", currentgroup );
	}
	else {
		if (info)
			fprintf (stdout, "%d", currentgroup);
		if (gethelp)
			fprintf (stdout, "Help is being sent%d\n", currentgroup);
	}
	XCloseDisplay(dpy);
}
