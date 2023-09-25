--TEST--
dbase_get_record(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (PHP_INT_SIZE != 4) die('skip for 32bit platforms only');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
try {
    var_dump(dbase_get_record());
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
try {
    var_dump(dbase_get_record(fopen('data://text/plain,foo', 'r'), 1));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_error.dbf';
$db = dbase_create($filename, [['foo', 'C', 15]]);
try {
    var_dump(dbase_get_record($db, -1));
} catch (ValueError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
var_dump(dbase_get_record($db, 1));
try {
    var_dump(dbase_get_record($db, 2147483648));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
?>
===DONE===
--EXPECTF--
dbase_get_record() expects exactly 2 arguments, 0 given
dbase_get_record(): Argument #1 ($database) must be of type Dbase\DbaseHandle, resource given
dbase_get_record(): Argument #2 ($number) record number has to be in range 1..2147483647, but is -1

Warning: dbase_get_record(): Tried to read bad record 1 in %s on line %d
bool(false)
dbase_get_record(): Argument #2 ($number) must be of type int, float given
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_error.dbf');
?>
