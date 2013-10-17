#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ed25519.h"
#include "squrl.h"
#include "php_sqrl.h"

#include <stdint.h>
#include <stdlib.h>



static function_entry sqrl_functions[] = {
	PHP_FE(sqrl_sign, NULL)
	PHP_FE(sqrl_verify, NULL)
	PHP_FE(sqrl_gen_sk, NULL)
	PHP_FE(sqrl_gen_pk, NULL)
	PHP_FE(sqrl_encode, NULL)
	PHP_FE(sqrl_decode, NULL)
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
	pOutput = squrl_encode( pInput, inputLength, &outputLength );

	//pRet = malloc( outputLength+1 );
	unsigned char pRet[outputLength+1];
	memcpy( pRet, pOutput, outputLength );
	pRet[outputLength] = 0;
	free( pOutput );
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
	pOutput = squrl_decode( pInput, inputLength, &outputLength );

	unsigned char pRet[outputLength+1];
	memcpy( pRet, pOutput, outputLength );
	pRet[outputLength] = 0;
	free( pOutput );
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
	
	pEncSk = squrl_encode( sk, 32, &i );
	
	// PHP needs null terminated string...
	unsigned char pRet[i+1];
	memcpy( pRet, pEncSk, i );
	pRet[i] = 0;
	free( pEncSk );
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
	
	sk = squrl_decode( skurl, skurl_len, &sk_len );
	ed25519_publickey(sk, pk);
	pkurl = squrl_encode( pk, 32, &pkurl_len );

	unsigned char pRet[pkurl_len+1];
	memcpy( pRet, pkurl, pkurl_len );
	pRet[pkurl_len] = 0;
	free( pkurl );

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
	pk = squrl_decode( pkurl, pkurl_length, &iPk );
	sig = squrl_decode( sigurl, sigurl_length, &iSig );
	
	nResult = ed25519_sign_open( msg, msg_length, pk, sig );

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
	const unsigned char *pSk;
	unsigned char *sigurl;
	unsigned char pk[32];
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
	pSk = squrl_decode( pSkUrl, iSkUrl, &iSk );
	ed25519_publickey( pSk, pk );
	ed25519_sign( pMessage, iMessage, pSk, &pk, sig );
	sigurl = squrl_encode( sig, 64, &iSigUrl );

	unsigned char pRet[iSigUrl+1];
	memcpy( pRet, sigurl, iSigUrl );
	pRet[iSigUrl] = 0;
	free( sigurl );
	free( pSk );
	RETURN_STRING( pRet, 1 );
}

