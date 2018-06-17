/* See LICENSE for license details. 
 *
 * dwm-sm is a status manager for dwm window manager.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "config.h"

#define LEN_STATUS 512
#define LEN_WIDGET 64
#define MAX_WIDGETS 8

static void battery_widget(char *wbuf);
static void clock_widget(char *wbuf);

static void die(const char *err);
static void widget_add(char widget);
static void sm_update(char *sbuf);
static void sm_print(char *sbuf);

static void (*widget_funcs[MAX_WIDGETS])(char *);
static unsigned int nwidgets = 0;

int
readline(const char *fn, char *buf, unsigned int n)
{
	FILE *fp = NULL;
	char *pos;
	
	if (!(fp = fopen(fn, "rb")))
		return -1;
	fgets(buf, n, fp);
	if ((pos = strchr(buf, '\n')))
		*pos = '\0';
	fclose(fp);
	return 0;
}

void
battery_widget(char *wbuf)
{
	char buf[5];
	
	if (readline(battery_file, buf, 5) < 0)
		sprintf(wbuf, "[AC]");
	else
		sprintf(wbuf, "[%s%%]", buf);
}

void
clock_widget(char *wbuf)
{
	time_t rawtime;
	struct tm *info;

	time(&rawtime);
	info = localtime(&rawtime);
	strftime(wbuf, LEN_WIDGET, "[%H:%M:%S]", info);
}

void
die(const char *err)
{
	fprintf(stderr, "error: %s\n", err);
	exit(EXIT_FAILURE);
}

void
widget_add(char widget)
{
	if (nwidgets >= MAX_WIDGETS)
		die("too many widgets");

	switch (widget) {
		case 'b':
			widget_funcs[nwidgets] = battery_widget;
			break;
		case 'c':
			widget_funcs[nwidgets] = clock_widget;
			break;
		default:
			die("invalid widget");
	}
	nwidgets++;
}

void
sm_update(char *sbuf)
{
	unsigned int i;
	char wbuf[LEN_WIDGET];

	memset(sbuf, 0, LEN_STATUS);
	for (i = 0; i < nwidgets; i++) {
		memset(wbuf, 0, LEN_WIDGET);
		widget_funcs[i](wbuf);
		strcat(sbuf, wbuf);
	}
}

void
sm_print(char *sbuf)
{
	char cmd[LEN_STATUS + 16];
	
	sprintf(cmd, "xsetroot -name %s", sbuf);
	system(cmd);
}

int
main()
{
	const char *wp = widgets;
	char sbuf[LEN_STATUS];
	
	while (*wp != '\0')
		widget_add(*(wp++));
	
	for (;;) {
		sm_update(sbuf);
		sm_print(sbuf);
		sleep(1);
	}
	return EXIT_SUCCESS; /* NOTREACHED */
}
