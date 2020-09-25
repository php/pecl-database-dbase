--TEST--
dbase_open(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
?>
--FILE--
<?php
try {
	var_dump(dbase_open());
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
?>
===DONE===
--EXPECTF--
dbase_open() expects exactly 2 arguments, 0 given
===DONE===
