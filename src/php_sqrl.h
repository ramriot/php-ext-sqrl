#ifndef PHP_SQRL_H
#define PHP_SQRL_H 1

#define PHP_SQRL_VERSION "0.1"
#define PHP_SQRL_EXTNAME "sqrl"

PHP_FUNCTION(sqrl_sign);
PHP_FUNCTION(sqrl_verify);
PHP_FUNCTION(sqrl_gen_sk);
PHP_FUNCTION(sqrl_gen_pk);
PHP_FUNCTION(sqrl_encode);
PHP_FUNCTION(sqrl_decode);
PHP_FUNCTION(sqrl_code_png);

extern zend_module_entry sqrl_module_entry;
#define phpext_sqrl_ptr &sqrl_module_entry

#endif
