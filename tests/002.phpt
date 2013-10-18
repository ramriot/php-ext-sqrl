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
		for( $i = 0; $i < 100; $i++ ) {
			for( $j = 1; $j < 101; $j++ ) {
				if( ! testEnDeCode( $j )) {
					$error = true;
				}
			}
		}
		if( $error == false ) {
			print "PASS";
		}
	}
?>
--EXPECT--
PASS
