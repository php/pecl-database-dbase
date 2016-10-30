--TEST--
dbase_add_record(): test error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_error.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, DBASE_RDWR);

try {
    dbase_add_record($db, 'no array');
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

var_dump(dbase_add_record($db, []));
?>
===DONE===
--EXPECTF--
Argument 2 passed to dbase_add_record() must be of the type array, string given

Warning: dbase_add_record(): expected 5 fields, but got 0 in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_error.dbf');
?>
