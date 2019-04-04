--TEST--
Bug #73411 (dbase_pack() returns TRUE on failure)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'bug73411.dbf';

$db = dbase_create($filename, [['NAME', 'C', 50]]);
dbase_add_record($db, ['foo']);
dbase_add_record($db, ['bar']);
dbase_delete_record($db, 1);
dbase_close($db);

$db = dbase_open($filename, 0); // read-only
var_dump(dbase_pack($db));
dbase_close($db);
?>
===DONE===
--EXPECTF--
Warning: dbase_pack(): unable to write to the file in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'bug73411.dbf');
?>
