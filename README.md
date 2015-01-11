PHP-EXT-SQRL
============
This is an implementation of parts of the (as yet incomplete) [Secure QR Login](https://www.grc.com/sqrl/sqrl.htm) Specification implemented in C as a PHP extension. Specifically the ed25519 signature verification function.

This is *much* faster than it's pure PHP counterpart... Passes all 1024 ed25519.cr.yp.to tests in 0.35 seconds total on an old Core 2 machine.

Features
--------
* Generate Secret Key (bad rng, not for production work)
* Derive Public Key from Secret Key
* Sign Message
* Verify Message Signature
* Encode/Decode base64url

Installation Requirements
-------------------------
* Linux build environment (autotools, etc)
* php5
* php5-dev

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

Acknowledgments
---------------
* crypto code is from [ed25519-donna](https://github.com/floodyberry/ed25519-donna).
* github user Novartors for providing the root of this fork

License
-------
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

