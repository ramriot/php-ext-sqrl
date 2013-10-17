PHP_ARG_ENABLE(sqrl, whether to enable SQRL support,
[ --enable-sqrl	     Enable SQRL support])

if test "$PHP_SQRL" = "yes"; then
   AC_DEFINE(HAVE_SQRL, 1, [Whether you have SQRL])
   PHP_NEW_EXTENSION(sqrl, src/squrl.c src/sqrl.c src/ed25519.c, $ext_shared)
fi
