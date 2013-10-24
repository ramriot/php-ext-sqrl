PHP_ARG_ENABLE(sqrl, whether to enable SQRL support,
[ --enable-sqrl	     Enable SQRL support])

PNGNAME=png
PNGSYM=png_write_row
QRENCODE=qrencode
QRENCODESYM=QRcode_encodeString

if test "$PHP_SQRL" = "yes"; then
	AC_DEFINE(HAVE_SQRL, 1, [Whether you have SQRL])
	PHP_CHECK_LIBRARY( $PNGNAME, $PNGSYM,
	[
		PHP_ADD_LIBRARY($PNGNAME, 1, SQRL_SHARED_LIBADD)
		AC_DEFINE(HAVE_PNG,1,[ ])
	],[
		AC_MSG_ERROR([wrong $PNGNAME lib version or lib not found])
	],[
		-L$PNGNAME -ldl
	])
	PHP_CHECK_LIBRARY( $QRENCODE, $QRENCODESYM,
	[
		PHP_ADD_LIBRARY($QRENCODE, 1, SQRL_SHARED_LIBADD)
		AC_DEFINE(HAVE_QRENCODE,1,[ ])
	],[
		AC_MSG_ERROR([wrong $QRENCODE lib version or lib not found])
	],[
		-L$QRENCODE -ldl
	])
	PHP_NEW_EXTENSION(sqrl, src/qr-png.c src/b64u.c src/sqrl.c src/ed25519.c, $ext_shared)
	PHP_SUBST(SQRL_SHARED_LIBADD)
fi
