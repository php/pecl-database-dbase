--TEST--
Bug #73395 (A failing dbase_create() may leak memory)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
dbase_create(__DIR__ . DIRECTORY_SEPARATOR . 'bug73395.dbf', array(array('foo')));
?>
===DONE===
--EXPECTF--
Warning: dbase_create(): expected field type as second element of list in field 0 in %s on line %d
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'bug73395.dbf');
?>
