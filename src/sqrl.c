#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <png.h>
#include <stdint.h>
#include <stdlib.h>
#include <qrencode.h>

#include "php.h"
#include "ed25519.h"
#include "qr-png.h"
#include "b64u.h"
#include "php_sqrl.h"



static function_entry sqrl_functions[] = {
	PHP_FE(sqrl_sign, NULL)
	PHP_FE(sqrl_verify, NULL)
	PHP_FE(sqrl_gen_sk, NULL)
	PHP_FE(sqrl_gen_pk, NULL)
	PHP_FE(sqrl_encode, NULL)
	PHP_FE(sqrl_decode, NULL)
	PHP_FE(sqrl_code_png, NULL)
	{NULL,NULL,NULL}
};

zend_module_entry sqrl_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_SQRL_EXTNAME,
	sqrl_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SQRL_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SQRL
ZEND_GET_MODULE(sqrl)
#endif

PHP_FUNCTION(sqrl_encode)
{
	const unsigned char *pInput;
	unsigned char* pOutput;
	int inputLength, outputLength;

	if( zend_parse_parameters( 
		ZEND_NUM_ARGS() TSRMLS_CC, 
		"s",
		&pInput, &inputLength
	) == FAILURE ) {
		RETURN_NULL();
	}
	pOutput = b64u_encode( pInput, inputLength, &outputLength );

	//pRet = malloc( outputLength+1 );
	unsigned char pRet[outputLength+1];
	memcpy( pRet, pOutput, outputLength );
	pRet[outputLength] = 0;
	efree( pOutput );
	RETURN_STRING( pRet, 1 );
}

PHP_FUNCTION(sqrl_decode)
{
	const unsigned char *pInput;
	unsigned char* pOutput;
	int inputLength, outputLength;

	if( zend_parse_parameters( 
		ZEND_NUM_ARGS() TSRMLS_CC, 
		"s",
		&pInput, &inputLength
	) == FAILURE ) {
		RETURN_NULL();
	}
	pOutput = b64u_decode( pInput, inputLength, &outputLength );

	unsigned char pRet[outputLength+1];
	memcpy( pRet, pOutput, outputLength );
	pRet[outputLength] = 0;
	efree( pOutput );
	RETURN_STRING( pRet, 1 );
}

PHP_FUNCTION(sqrl_gen_sk)
{
	unsigned char sk[32];
	unsigned char *pEncSk;
	int i;
	
	for( i = 0; i < 32; i++ ) {
		sk[i] = (unsigned char)(rand() % 256);
	}
	
	pEncSk = b64u_encode( sk, 32, &i );
	
	// PHP needs null terminated string...
	unsigned char pRet[i+1];
	memcpy( pRet, pEncSk, i );
	pRet[i] = 0;
	efree( pEncSk );
	RETURN_STRING( pRet, 1 );
}

PHP_FUNCTION(sqrl_gen_pk)
{
	const unsigned char *skurl;
	unsigned char *pkurl;
	unsigned char *sk;
	unsigned char pk[32];
	int skurl_len, sk_len, pkurl_len;

	if( zend_parse_parameters( 
		ZEND_NUM_ARGS() TSRMLS_CC, 
		"s",
		&skurl, &skurl_len
	) == FAILURE ) {
		RETURN_NULL();
	}
	
	sk = b64u_decode( skurl, skurl_len, &sk_len );
	ed25519_publickey(sk, pk);
	pkurl = b64u_encode( pk, 32, &pkurl_len );

	unsigned char pRet[pkurl_len+1];
	memcpy( pRet, pkurl, pkurl_len );
	pRet[pkurl_len] = 0;
	
	efree(sk);
	efree( pkurl );

	RETURN_STRING( pRet, 1 );
}

PHP_FUNCTION(sqrl_verify)
{
	char *msg, *pkurl, *sigurl;
	unsigned char *pk, *sig;
	int msg_length, pkurl_length, sigurl_length;
	int iPk, iSig, nResult;
	
	if( zend_parse_parameters( 
		ZEND_NUM_ARGS() TSRMLS_CC, 
		"sss",
		&msg, &msg_length,
		&sigurl, &sigurl_length,
		&pkurl, &pkurl_length
	) == FAILURE ) {
		RETURN_NULL();
	}
	pk = b64u_decode( pkurl, pkurl_length, &iPk );
	sig = b64u_decode( sigurl, sigurl_length, &iSig );
	
	nResult = ed25519_sign_open( msg, msg_length, pk, sig );
	efree(pk);
	efree(sig);
	if( nResult == 0 ) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

PHP_FUNCTION(sqrl_sign)
{
	const unsigned char *pMessage;
	const unsigned char *pSkUrl;
	unsigned char *pSk;
	unsigned char *sigurl;
	unsigned char *pk;
	unsigned char sig[64];
	int iMessage, iSkUrl, iSk, iSigUrl;
	
	if( zend_parse_parameters( 
		ZEND_NUM_ARGS() TSRMLS_CC, 
		"ss",
		&pMessage, &iMessage,
		&pSkUrl, &iSkUrl
	) == FAILURE ) {
		RETURN_NULL();
	}
	pSk = b64u_decode( pSkUrl, iSkUrl, &iSk );
	pk = emalloc( 32 );
	ed25519_publickey( pSk, pk );
	ed25519_sign( pMessage, iMessage, pSk, pk, sig );
	sigurl = b64u_encode( sig, 64, &iSigUrl );

	unsigned char pRet[iSigUrl+1];
	memcpy( pRet, sigurl, iSigUrl );
	pRet[iSigUrl] = 0;
	efree( sigurl );
	efree( pSk );
	efree( pk );
	
	RETURN_STRING( pRet, 1 );
}

PHP_FUNCTION(sqrl_code_png)
{
	QRcode *qrcode;
	const unsigned char *pInput;
	unsigned char *string;
	int input_length;
	
	if( zend_parse_parameters( 
		ZEND_NUM_ARGS() TSRMLS_CC, 
		"s",
		&pInput, &input_length
	) == FAILURE ) {
		RETURN_NULL();
	}
	
	string = emalloc( input_length+1 );
	memcpy( string, pInput, input_length );
	string[input_length] = 0;
	
	qrcode = QRcode_encodeString(string, 0, QR_ECLEVEL_L, QR_MODE_8, 1);
	if(qrcode == NULL) {
		RETURN_FALSE;
	}
	qr_png_print( qrcode );
	efree( string );
	QRcode_free(qrcode);
	
	RETURN_TRUE;
}
