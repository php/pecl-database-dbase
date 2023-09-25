--TEST--
dbase_numfields(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '>')) die('skip for PHP 7 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
var_dump(dbase_numfields());
var_dump(dbase_numfields(fopen('data://text/plain,foo', 'r')));
?>
===DONE===
--EXPECTF--
Warning: dbase_numfields() expects exactly 1 parameter, 0 given in %s on line %d
NULL

Warning: dbase_numfields() expects parameter 1 to be Dbase\DbaseHandle, resource given in %s on line %d
NULL
===DONE===
