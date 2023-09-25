--TEST--
Checks that Dbase\DbaseHandle instances cannot be cloned
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbasehandle_prevent_cloning.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, DBASE_RDWR);
var_dump($db);
$dbClone = clone $db;
?>
--EXPECTF--
object(Dbase\DbaseHandle)#%d (0) {
}

Fatal error: Uncaught Error: Trying to clone an uncloneable object of class Dbase\DbaseHandle in %s:%d
Stack trace:
#0 {main}
  thrown in %s on line %d
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbasehandle_prevent_cloning.dbf');
?>
