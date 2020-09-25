--TEST--
dbase_delete_record(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (PHP_INT_SIZE != 8) die('skip for 64bit platforms only');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_delete_record_error.dbf';
$db = dbase_create($filename, [['foo', 'C', 15]]);
try {
	var_dump(dbase_delete_record($db));
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
try {
	var_dump(dbase_delete_record(fopen('data://text/plain,foo', 'r'), 1));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
try {
	var_dump(dbase_delete_record($db, -1));
} catch (ValueError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
var_dump(dbase_delete_record($db, 1));
try {
	var_dump(dbase_delete_record($db, 2147483648));
} catch (ValueError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
?>
===DONE===
--EXPECTF--
dbase_delete_record() expects exactly 2 arguments, 1 given
dbase_delete_record(): supplied resource is not a valid dbase resource
dbase_delete_record(): Argument #2 ($number) record number has to be in range 1..2147483647, but is -1

Warning: dbase_delete_record(): record 1 out of bounds in %s on line %d
bool(false)
dbase_delete_record(): Argument #2 ($number) record number has to be in range 1..2147483647, but is 2147483648
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_delete_record_error.dbf');
?>
