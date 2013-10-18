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
		$tests = array( 
			"" => "",
			"f" => "Zg",
			"fo" => "Zm8",
			"foo" => "Zm9v",
			"foob" => "Zm9vYg",
			"fooba" => "Zm9vYmE",
			"foobar" => "Zm9vYmFy"
		);

		foreach( $tests as $key => $value ) {
			if( sqrl_encode( $key ) != $value || sqrl_decode( $value ) != $key ) {
				$error = true;
			}
		}

		for( $i = 1; $i < 311; $i++ ) {
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
