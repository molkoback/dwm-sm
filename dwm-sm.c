/* dwm status monitor
 *
 * Copyright (c) 2018 molko <molkoback@gmail.com>
 * 
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

static const char *USAGE = 
"usage: dwm-sm <widgets>\n"
"\n"
"widgets:\n"
"c   clock\n";

#define LEN_STATUS 512
#define LEN_WIDGET 64
#define MAX_WIDGETS 8

static void (*widgets[MAX_WIDGETS])(char *);
static size_t nwidgets = 0;

static void clock_widget(char *wbuf)
{
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(wbuf, LEN_WIDGET, "[%H:%M:%S]", info);

}

static void widget_add(void (*widget)(char *))
{
	if (nwidgets < MAX_WIDGETS)
		widgets[nwidgets++] = widget;
}

static void sm_update(char *sbuf)
{
	char wbuf[LEN_WIDGET];
	memset(sbuf, 0, LEN_STATUS);
	for (size_t i = 0; i < nwidgets; i++) {
		memset(wbuf, 0, LEN_WIDGET);
		widgets[i](wbuf);
		strcat(sbuf, wbuf);
	}
}

static void sm_print(char *sbuf)
{
	char cmd[LEN_STATUS + 16];
	sprintf(cmd, "xsetroot -name %s", sbuf);
	system(cmd);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, USAGE);
		return 1;
	}
	
	size_t len = strlen(argv[1]);
	if (strlen(argv[1]) > MAX_WIDGETS) {
		fprintf(stderr, "error: too many widgets: %d\n", (int)len);
		return 1;
	}
	
	for (size_t i = 0; i < len; i++) {
		switch (argv[1][i]) {
		case 'c':
			widget_add(clock_widget);
			break;
		default:
			fprintf(stderr, "error: invalid widget: '%c'\n", argv[1][i]);
			return 1;
		}
	}
	
	char sbuf[LEN_STATUS];
	while (1) {
		sm_update(sbuf);
		sm_print(sbuf);
		sleep(1);
	}
	return 0;
}
