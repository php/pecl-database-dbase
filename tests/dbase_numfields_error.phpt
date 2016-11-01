--TEST--
dbase_numfields(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
var_dump(dbase_numfields());
var_dump(dbase_numfields(fopen('php://input', 'r')));
?>
===DONE===
--EXPECTF--
Warning: dbase_numfields() expects exactly 1 parameter, 0 given in %s on line %d
NULL

Warning: dbase_numfields(): supplied resource is not a valid dbase resource in %s on line %d
bool(false)
===DONE===
