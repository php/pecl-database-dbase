/*
 * Copyright (c) 1991, 1992, 1993 Brad Eacker,
 *              (Music, Intuition, Software, and Computers)
 * All Rights Reserved
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include "dbf_misc.h"
#include "dbf_sdncal.h"

#include "php_reentrancy.h"

#if (defined(__APPLE__) || defined(__APPLE_CC__)) && (defined(__BIG_ENDIAN__) || defined(__LITTLE_ENDIAN__))
# if defined(__LITTLE_ENDIAN__)
#  undef WORDS_BIGENDIAN
# else
#  if defined(__BIG_ENDIAN__)
#   define WORDS_BIGENDIAN
#  endif
# endif
#endif

/*
 * routine to change little endian long to host long
 */
long get_long(char *cp)
{
	int ret;
	unsigned char *source = (unsigned char *)cp;

	ret = *source++;
	ret += ((*source++)<<8);
	ret += ((*source++)<<16);
	ret += ((*source++)<<24);

	return ret;
}

void put_long(char *cp, long lval)
{
	*cp++ = lval & 0xff;
	*cp++ = (lval >> 8) & 0xff;
	*cp++ = (lval >> 16) & 0xff;
	*cp++ = (lval >> 24) & 0xff;
}

/*
 * routine to change little endian short to host short
 */
int get_short(char *cp)
{
	int ret;
	unsigned char *source = (unsigned char *)cp;

	ret = *source++;
	ret += ((*source++)<<8);

	return ret;
}

void put_short(char *cp, int sval)
{
	*cp++ = sval & 0xff;
	*cp++ = (sval >> 8) & 0xff;
}

double get_double(char *cp)
{
	double ret;
	unsigned char *dp = (unsigned char *)&ret;
	int i;

#ifdef WORDS_BIGENDIAN
	for (i = 7; i >= 0; i--) {
#else
	for (i = 0; i <= 7; i++) {
#endif
		dp[i] = *cp++;
	}

	return ret;
}

void put_double(char *cp, double fval)
{
	unsigned char *dp = (unsigned char *)&fval;
	int i;

#ifdef WORDS_BIGENDIAN
	for (i = 7; i >= 0; i--) {
#else
	for (i = 0; i <= 7; i++) {
#endif
		cp[i] = *dp++;
	}
}

void copy_fill(char *dp, char *sp, int len)
{
	while (*sp && len > 0) {
		*dp++ = *sp++;
		len--;
	}
	while (len-- > 0)
		*dp++ = ' ';
}

void copy_crimp(char *dp, char *sp, int len)
{
	while (len-- > 0) {
		*dp++ = *sp++;
	}
	*dp = 0;
	for (dp-- ; *dp == ' '; dp--) {
		*dp = 0;
	}

}

void db_set_date(char *cp, int year, int month, int day)
{
	if (month > 12)
		month = 0;
	if (day > 31)
		day = 0;
	snprintf(cp, 9, "%04d%02d%02d", year, month, day);
}

int db_date_year(char *cp)
{
	int	year, i;

	for (year = 0, i = 0; i < 4; i++)
		year = year * 10 + (cp[i] - '0');
	return year;
}

int db_date_month(char *cp)
{
	int	month, i;

	for (month = 0, i = 4; i < 6; i++)
		month = month * 10 + (cp[i] - '0');
	return month;
}

int db_date_day(char *cp)
{
	int	day, i;

	for (day = 0, i = 6; i < 8; i++)
		day = day * 10 + (cp[i] - '0');
	return day;
}

void db_set_timestamp(char *cp, int jdn, int msecs)
{
	int year, month, day, hour, minute, second, millis;

	db_sdn_to_gregorian(jdn, &year, &month, &day);

	millis = msecs % 1000;
	msecs /= 1000;
	second = msecs % 60;
	msecs /= 60;
	minute = msecs % 60;
	msecs /= 60;
	hour = msecs;

	snprintf(cp, 19, "%04d%02d%02d%02d%02d%02d.%03d", year, month, day, hour, minute, second, millis);
}

void db_get_timestamp(char *cp, int *jdn, int *msecs)
{
	int year, month, day, hour, minute, second, millis;

	sscanf(cp, "%04d%02d%02d%02d%02d%02d.%03d", &year, &month, &day, &hour, &minute, &second, &millis);

	*jdn = db_gregorian_to_sdn(year, month, day);
	*msecs = (hour * 60 * 60 * 1000) + (minute * 60 * 1000) + (second * 1000) + millis;
}

#include <time.h>

char *db_cur_date(char *cp)
{
	struct tm *ctm, tmbuf;
	time_t	  c_time;

	c_time = time((time_t *)NULL);
	ctm = php_localtime_r(&c_time, &tmbuf);
	if (cp == NULL)
		cp = (char *)emalloc(9);

	if (ctm == NULL)
		return NULL;

	db_set_date(cp, ctm->tm_year + 1900, ctm->tm_mon + 1, ctm->tm_mday);

	return cp;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
