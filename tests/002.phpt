--TEST--
Test SQRL URL (base64url-ish) encode / decode
--FILE--
<?php
function randomString( $len ) {
	$characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$str = '';
	for( $j = 0; $j < $len; $j++ ) {
		$str .= $characters[rand(0, strlen($characters)-1)];
	}
	return $str;
}

function testEnDeCode( $nLength ) {
	$str = randomString( $nLength );
	$eStr = sqrl_encode( $str );
	$dStr = sqrl_decode( $eStr );
	if( $dStr == $str ) {
		return true;
	}
	return false;
}


	if( extension_loaded("sqrl")) {
		$error = false;
		for( $i = 1; $i < 101; $i++ ) {
			if( ! testEnDeCode( $i )) {
				$error = true;
			}
		}
		if( $error == false ) {
			print "PASS";
		}
	}
?>
--EXPECT--
PASS
