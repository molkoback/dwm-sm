/* See LICENSE for license details. 
 *
 * dwm-sm is a status manager for dwm window manager.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <X11/Xlib.h>

#include "config.h"

#define LEN_STATUS 512
#define LEN_WIDGET 32

static void battery_widget(char *wbuf);
static void time_widget(char *wbuf);

static void widget_run(char *wbuf, char widget);
static void status_update(char *sbuf, const char *fmt);
static void status_set(Display *dpy, char *sbuf);

int
readline(const char *fn, char *buf, unsigned int n)
{
	unsigned int i, c;
	FILE *fp = NULL;
	
	if (!(fp = fopen(fn, "rb")))
		return -1;
	for (i = 0; i < n-1; i++) {
		c = fgetc(fp);
		if (c == '\0' || c == '\n')
			break;
		else
			buf[i] = c;
	}
	buf[i] = '\0';
	fclose(fp);
	return 0;
}

void
battery_widget(char *wbuf)
{
	char buf[5];
	
	if (readline(battery_file, buf, 5) < 0)
		sprintf(wbuf, "AC");
	else
		sprintf(wbuf, "%s%%", buf);
}

void
time_widget(char *wbuf)
{
	time_t rawtime;
	struct tm *info;
	
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(wbuf, LEN_WIDGET, time_fmt, info);
}

void
widget_run(char *wbuf, char w)
{
	switch (w) {
	case 'b':
		battery_widget(wbuf);
		break;
	case 't':
		time_widget(wbuf);
		break;
	case '%':
		sprintf(wbuf, "%%");
		break;
	default:
		sprintf(wbuf, "%%%c", w);
	}
}

void
status_update(char *sbuf, const char *fmt)
{
	const char *p = fmt-1;
	char wbuf[LEN_WIDGET];
	
	memset(sbuf, 0, LEN_STATUS);
	while (*(++p)) {
		if (*p == '%') {
			widget_run(wbuf, *(++p));
			strcat(sbuf, wbuf);
		} else {
			sbuf[strlen(sbuf)] = *p;
		}
	}
}

void
status_set(Display *dpy, char *sbuf)
{
	XStoreName(dpy, DefaultRootWindow(dpy), sbuf);
	XSync(dpy, False);
}

int
main()
{
	char sbuf[LEN_STATUS];
	Display *dpy;
	
	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "couldn't open display\n");
		return EXIT_FAILURE;
	}
	for (;;) {
		status_update(sbuf, widgets);
		status_set(dpy, sbuf);
		sleep(1);
	}
	return EXIT_SUCCESS; /* NOTREACHED */
}
