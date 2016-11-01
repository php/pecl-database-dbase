--TEST--
dbase_open(): open_basedir restriction
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--INI--
open_basedir=.
--FILE--
<?php
var_dump(dbase_open('../bad', DBASE_RDONLY));
?>
===DONE===
--EXPECTF--
Warning: dbase_open(): open_basedir restriction in effect. File(../bad) is not within the allowed path(s): (.) in %s on line %d
bool(false)
===DONE===
