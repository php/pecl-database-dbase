--TEST--
dbase_pack(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '>')) die('skip for PHP 7 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
var_dump(dbase_pack());
var_dump(dbase_pack(fopen('data://text/plain,foo', 'r')));
?>
===DONE===
--EXPECTF--
Warning: dbase_pack() expects exactly 1 parameter, 0 given in %s on line %d
NULL

Warning: dbase_pack(): supplied resource is not a valid dbase resource in %s on line %d
bool(false)
===DONE===
