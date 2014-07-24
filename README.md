PHP-EXT-SQRL
============
This is an implementation of parts of the (as yet incomplete) [Secure QR Login](https://www.grc.com/sqrl/sqrl.htm) Specification implemented in C as a PHP extension.  Currently only tested on 64bit linux systems.

This is *much* faster than it's pure PHP counterpart... Passes all 1024 ed25519.cr.yp.to tests in 0.35 seconds total on an old Core 2 machine.

Features
--------
* Generate Secret Key (bad rng, not for production work)
* Derive Public Key from Secret Key
* Sign Message
* Verify Message Signature
* Encode/Decode base64url
* (TBR) Create QR code image (png)

Installation Requirements
-------------------------
* Linux build environment (autotools, etc)
* php5
* php5-dev
* (TBR)[libqrencode](http://fukuchi.org/works/qrencode/)
* (TBR)[libpng](http://www.libpng.org/pub/png/libpng.html)

Installation
------------
* Download, configure, and make:

```
git clone https://github.com/ramriot/php-ext-sqrl.git
cd php-ext-sqrl
phpize && ./configure && make
```

* Run Tests: `make test`
* Install: `sudo make install`
* Enable the sqrl extension in php.ini: `extension=sqrl.so`

Documentation
-------------
* Encode / Decode base64url
```php
$encoded = sqrl_encode( $plain );
$decoded = sqrl_decode( $encoded );
$decoded == plain // true
```
* Verify Signature
```php
$result = sqrl_verify( $message, $signature, $pk );
// $message = plain text of signed message
// $signature = base64url-encoded signature
// $pk = base64url-encoded Public Key
```

*(TBR) Create QR code image
```php
header( 'content-type: image/png' );
sqrl_code_png( $content );
// Encodes $content into a QR code image and output that image.
```

Acknowledgments
---------------
* crypto code is from [ed25519-donna](https://github.com/floodyberry/ed25519-donna).
* github user Novartors for providing the root of this fork

License
-------
This software is Public Domain.
