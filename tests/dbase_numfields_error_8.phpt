--TEST--
dbase_numfields(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
try {
	var_dump(dbase_numfields());
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
try {
	var_dump(dbase_numfields(fopen('data://text/plain,foo', 'r')));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
?>
===DONE===
--EXPECTF--
dbase_numfields() expects exactly 1 argument, 0 given
dbase_numfields(): Argument #1 ($database) must be of type Dbase\DbaseHandle, resource given
===DONE===
