--TEST--
dbase_open(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '>')) die('skip for PHP 7 only');
?>
--FILE--
<?php
var_dump(dbase_open());
?>
===DONE===
--EXPECTF--
Warning: dbase_open() expects exactly 2 parameters, 0 given in %s on line %d
NULL
===DONE===
