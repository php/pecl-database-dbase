--TEST--
dbase_numrecords(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
var_dump(dbase_numrecords());
var_dump(dbase_numrecords(fopen('php://input', 'r')));
?>
===DONE===
--EXPECTF--
Warning: dbase_numrecords() expects exactly 1 parameter, 0 given in %s on line %d
NULL

Warning: dbase_numrecords(): supplied resource is not a valid dbase resource in %s on line %d
bool(false)
===DONE===
