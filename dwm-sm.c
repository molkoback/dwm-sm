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
#define LEN_WIDGET 32

static void battery_widget(char *wbuf);
static void clock_widget(char *wbuf);

static void widget_run(char *wbuf, char widget);
static void sm_update(char *sbuf, const char *fmt);
static void sm_print(char *sbuf);

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
		sprintf(wbuf, "AC");
	else
		sprintf(wbuf, "%s%%", buf);
}

void
clock_widget(char *wbuf)
{
	time_t rawtime;
	struct tm *info;
	
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(wbuf, LEN_WIDGET, "%H:%M:%S", info);
}

void
widget_run(char *wbuf, char w)
{
	switch (w) {
	case 'b':
		battery_widget(wbuf);
		break;
	case 'c':
		clock_widget(wbuf);
		break;
	case '%':
		sprintf(wbuf, "%%");
		break;
	default:
		sprintf(wbuf, "%%%c", w);
	}
}

void
sm_update(char *sbuf, const char *fmt)
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
sm_print(char *sbuf)
{
	char cmd[LEN_STATUS + 18];
	
	sprintf(cmd, "xsetroot -name \"%s\"", sbuf);
	system(cmd);
}

int
main()
{
	char sbuf[LEN_STATUS];
	
	for (;;) {
		sm_update(sbuf, widgets);
		sm_print(sbuf);
		sleep(1);
	}
	return EXIT_SUCCESS; /* NOTREACHED */
}
