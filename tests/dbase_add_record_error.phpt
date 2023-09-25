--TEST--
dbase_add_record(): test error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '>')) die('skip for PHP 7 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_error.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, DBASE_RDWR);

var_dump(dbase_add_record($db));

var_dump(dbase_add_record(fopen('data://text/plain,foo', 'r'), []));

try {
    dbase_add_record($db, 'no array');
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

var_dump(dbase_add_record($db, []));

var_dump(dbase_add_record($db, [0, 1, 2, 3, 4, 'foo' => 5, 6]));
?>
===DONE===
--EXPECTF--
Warning: dbase_add_record() expects exactly 2 parameters, 1 given in %s on line %d
NULL

Warning: dbase_add_record() expects parameter 1 to be Dbase\DbaseHandle, resource given in %s on line %d
NULL
Argument 2 passed to dbase_add_record() must be of the type array, string given

Warning: dbase_add_record(): expected 7 fields, but got 0 in %s on line %d
bool(false)

Warning: dbase_add_record(): expected plain indexed array in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_error.dbf');
?>
