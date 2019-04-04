--TEST--
dbase_open() raises warning for unsupported field types
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_open_error1.dbf';
dbase_open($filename, DBASE_RDONLY);
?>
===DONE===
--EXPECTF--
Warning: dbase_open(): unknown field type 'X' in %s on line %d

Warning: dbase_open(): unable to open database %s in %s on line %d
===DONE===
