--TEST--
dbase_replace_record(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (PHP_INT_SIZE != 4) die('skip for 32bit platforms only');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
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

try {
    var_dump(dbase_replace_record($db, [], -1));
} catch (ValueError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
try {
    var_dump(dbase_replace_record($db, [], 2147483648));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

var_dump(dbase_replace_record($db, [], 1));
?>
===DONE===
--EXPECTF--
dbase_replace_record(): Argument #2 ($data) must be of type array, string given
dbase_replace_record(): Argument #3 ($number) record number has to be in range 1..2147483647, but is -1
dbase_replace_record(): Argument #3 ($number) must be of type int, float given

Warning: dbase_replace_record(): expected 7 fields, but got 0 in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_error_1.dbf');
?>
