--TEST--
Bug #73414 (Unsupported field types may cause dbase_open() to leak memory)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
dbase_open(__DIR__ . DIRECTORY_SEPARATOR . 'bug73414.dbf', 0);
?>
===DONE===
--EXPECTF--
Warning: dbase_open(): unable to open database %s in %s on line %d
===DONE===
