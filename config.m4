dnl $Id: config.m4,v 1.3 1999/12/30 04:07:21 sas Exp $

AC_ARG_WITH(dbase,[],[enable_dbase=$withval])

PHP_ARG_ENABLE(dbase,whether to enable the bundled dbase library,
[  --enable-dbase          Enable the bundled dbase library])

if test "$PHP_DBASE" = "yes"; then
  AC_DEFINE(DBASE,1,[ ])
  PHP_EXTENSION(dbase)
else
  AC_DEFINE(DBASE,0,[ ])
fi
