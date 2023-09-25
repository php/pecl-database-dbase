--TEST--
dbase_delete_record(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (PHP_INT_SIZE != 8) die('skip for 64bit platforms only');
if (version_compare(PHP_VERSION, '8', '>')) die('skip for PHP 7 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_delete_record_error.dbf';
$db = dbase_create($filename, [['foo', 'C', 15]]);
var_dump(dbase_delete_record($db));
var_dump(dbase_delete_record(fopen('data://text/plain,foo', 'r'), 1));
var_dump(dbase_delete_record($db, -1));
var_dump(dbase_delete_record($db, 1));
var_dump(dbase_delete_record($db, 2147483648));
?>
===DONE===
--EXPECTF--
Warning: dbase_delete_record() expects exactly 2 parameters, 1 given in %s on line %d
NULL

Warning: dbase_delete_record() expects parameter 1 to be Dbase\DbaseHandle, resource given in %s on line %d
NULL

Warning: dbase_delete_record(): record number has to be in range 1..2147483647, but is -1 in %s on line %d
bool(false)

Warning: dbase_delete_record(): record 1 out of bounds in %s on line %d
bool(false)

Warning: dbase_delete_record(): record number has to be in range 1..2147483647, but is 2147483648 in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_delete_record_error.dbf');
?>
