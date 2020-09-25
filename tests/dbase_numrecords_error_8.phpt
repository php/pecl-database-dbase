--TEST--
dbase_numrecords(): error conditions
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
	var_dump(dbase_numrecords());
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
try {
	var_dump(dbase_numrecords(fopen('data://text/plain,foo', 'r')));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
?>
===DONE===
--EXPECTF--
dbase_numrecords() expects exactly 1 argument, 0 given
dbase_numrecords(): supplied resource is not a valid dbase resource
===DONE===
