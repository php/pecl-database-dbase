--TEST--
dbase_open() fails if '0' field is not last field
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_open_error2.dbf';
var_dump(dbase_open($filename, DBASE_RDONLY));
?>
===DONE===
--EXPECTF--
Warning: dbase_open(): unexpected field type '0' in %s on line %d

Warning: dbase_open(): unable to open database %s in %s on line %d
bool(false)
===DONE===
