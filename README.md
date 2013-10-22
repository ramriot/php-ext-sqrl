PHP-EXT-SQRL
============
This is an implementation of parts of the (as yet incomplete) [Secure QR Login](https://www.grc.com/sqrl/sqrl.htm) Specification implemented in C as a PHP extension.  Currently only tested on 64bit linux systems.

This is *much* faster than it's pure PHP counterpart... Passes all 1024 ed25519.cr.yp.to tests in 0.35 seconds total on an old Core 2 machine.


Installation Requirements
-------------------------
* Linux build environment (autotools, etc)
* php5
* php5-dev
* libqrencode
* libpng

Installation
------------
* Download, configure, and make:

	
	git clone https://github.com/Novators/php-ext-sqrl.git
	
	cd php-ext-sqrl
	
	phpize && ./configure && make
	

* Run Tests, if desired (may take a few seconds)

	
	make test
	

* Install

	
	sudo make install
	

* Enable the sqrl extension in php.ini:

	
	extension=sqrl.so
	

Documentation
-------------
Coming soon...

Acknowledgments
---------------
* crypto code is from [ed25519-donna](https://github.com/floodyberry/ed25519-donna).

License
-------
This software is Public Domain.
