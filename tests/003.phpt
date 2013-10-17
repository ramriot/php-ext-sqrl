--TEST--
Test Signature Validation
--FILE--
<?php
function unHex( $str ) {
	if( (strlen( $str ) % 2) == 1 ) {
		$str .= '0';
	}
	$chars = '';
	for( $i = 0; $i < strlen( $str ); $i += 2 ) {
		$chars .= chr( hexdec( substr( $str, $i, 2 )));
	}
	return $chars;
}

function randomString( $len ) {
	$characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$str = '';
	for( $j = 0; $j < $len; $j++ ) {
		$str .= $characters[rand(0, strlen($characters)-1)];
	}
	return $str;
}

function runTestString( $str ) {
	$parts = explode( ":", $str );
	if( count( $parts ) < 4 ) {
		print "Bad input data...\n";
		return false;
	}
	$sk = sqrl_encode( unHex( substr( $parts[0], 0, 64 )));
	$pk = sqrl_encode( unHex( substr( $parts[0], 64 )));
	$sig = sqrl_encode( unHex( $parts[3] ));
	$msg = unHex( $parts[2] );
	$v = sqrl_verify( $msg, $sig, $pk );
	if( strlen( $msg == 0 )) 
		$alteredMsg = randomString( 1 );
	else
		$alteredMsg = randomString( strlen( $msg ));
	$av = sqrl_verify( $alteredMsg, $sig, $pk );
	if( $v && !$av ) {
		return true;
	}
	return false;
}


	if( extension_loaded("sqrl")) {
		$handle = @fopen( "tests/sign.input", "r" );
		$error = false;
		$line = 1;
		if( $handle ) {
			while(( $buffer = fgets( $handle, 8192 )) !== false ) {
				if( !runTestString( $buffer )) {
					$error = true;
				}
				$line++;
			}
			fclose( $handle );
		} else {
			print "Unable to open file...\n";
		}
		if( $line > 1000 && $error == false ) {
			print "TRUE";
		}
	}
?>
--EXPECT--
TRUE
