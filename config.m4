dnl
dnl $Id: config.m4,v 1.5 2000/05/02 04:32:33 sas Exp $
dnl

AC_ARG_WITH(dbase,[],[enable_dbase=$withval])

PHP_ARG_ENABLE(dbase,whether to enable dbase support,
[  --enable-dbase          Enable the bundled dbase library])

if test "$PHP_DBASE" = "yes"; then
  AC_DEFINE(DBASE,1,[ ])
  PHP_EXTENSION(dbase, $ext_shared)
fi
