/* See LICENSE for license details. */

/* our widgets */ 
/* %b = battery, %t = time */
const char widgets[] = "%b | %t";

/* time widget strftime format string */
const char time_fmt[] = "%a %b %d | %H:%M:%S";

/* battery widget information file */
const char battery_file[] = "/sys/class/power_supply/BAT0/capacity";
