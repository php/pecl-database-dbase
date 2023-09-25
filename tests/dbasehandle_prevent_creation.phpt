--TEST--
Checks that Dbase\DbaseHandle cannot be directly created
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
new Dbase\DbaseHandle();
?>
--EXPECTF--
Fatal error: Uncaught Error: Cannot directly construct Dbase\DbaseHandle, use dbase_create() or dbase_open() instead in %s:%d
Stack trace:
#0 {main}
  thrown in %s on line %d
