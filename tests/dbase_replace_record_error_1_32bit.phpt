--TEST--
dbase_replace_record(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (PHP_INT_SIZE != 4) die('skip for 32bit platforms only');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_error_1.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, DBASE_RDWR);

try {
    dbase_replace_record($db, 'no array', 1);
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

var_dump(dbase_replace_record($db, [], -1));
var_dump(dbase_replace_record($db, [], 2147483648));

var_dump(dbase_replace_record($db, [], 1));
?>
===DONE===
--EXPECTF--
Argument 2 passed to dbase_replace_record() must be of the type array, string given

Warning: dbase_replace_record(): record number has to be in range 1..2147483647, but is -1 in %s on line %d
bool(false)

Warning: dbase_replace_record() expects parameter 3 to be in%s, float given in %s on line %d
NULL

Warning: dbase_replace_record(): expected 7 fields, but got 0 in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_error.dbf');
?>
