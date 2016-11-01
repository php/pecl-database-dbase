--TEST--
dbase_get_record_with_names(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (PHP_INT_SIZE != 8) die('skip for 64bit platforms only');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_error.dbf';
$db = dbase_create($filename, [['foo', 'C', 15]]);
var_dump(dbase_get_record_with_names($db, -1));
var_dump(dbase_get_record_with_names($db, 1));
var_dump(dbase_get_record_with_names($db, 2147483648));
?>
===DONE===
--EXPECTF--
Warning: dbase_get_record_with_names(): record number has to be in range 1..2147483647, but is -1 in %s on line %d
bool(false)

Warning: dbase_get_record_with_names(): Tried to read bad record 1 in %s on line %d
bool(false)

Warning: dbase_get_record_with_names(): record number has to be in range 1..2147483647, but is 2147483648 in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_error.dbf');
?>
