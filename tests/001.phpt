--TEST--
Tests for SQRL presence
--FILE--
<?php
	if( extension_loaded("sqrl")) {
		print "sqrl extension is available";
	}
?>
--EXPECT--
sqrl extension is available
