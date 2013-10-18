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
	$parts = explode( "+", trim($str) );
	if( count( $parts ) < 3 ) {
		print "Bad input data...\n";
		return false;
	}
	$pk = $parts[0];
	$sig = $parts[1];
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

$handle = @fopen( "tests/003.input", "r" );
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
?>
--EXPECT--
TRUE
