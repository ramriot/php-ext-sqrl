PHP_ARG_ENABLE(sqrl, whether to enable SQRL support,
[ --enable-sqrl	     Enable SQRL support])

PNGNAME=png
PNGSYM=png_write_row
QRENCODE=qrencode
QRENCODESYM=QRcode_encodeString

if test "$PHP_SQRL" = "yes"; then
	AC_DEFINE(HAVE_SQRL, 1, [Whether you have SQRL])
	PHP_NEW_EXTENSION(sqrl, src/b64u.c src/sqrl.c src/ed25519.c, $ext_shared)
	PHP_SUBST(SQRL_SHARED_LIBADD)
fi
