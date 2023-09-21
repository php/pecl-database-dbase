--TEST--
dbase_replace_record(): test error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_error.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, DBASE_RDWR);

try {
	var_dump(dbase_replace_record($db));
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

try {
	var_dump(dbase_replace_record(fopen('data://text/plain,foo', 'r'), [], 1));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

try {
    dbase_replace_record($db, 'no array', 1);
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

var_dump(dbase_replace_record($db, [], 1));

var_dump(dbase_replace_record($db, [0, 1, 2, 3, 4, 'foo' => 5, 6], 1));
?>
===DONE===
--EXPECTF--
dbase_replace_record() expects exactly 3 arguments, 1 given
dbase_replace_record(): Argument #1 ($database) must be of type Dbase\DbaseHandle, resource given
dbase_replace_record(): Argument #2 ($data) must be of type array, string given

Warning: dbase_replace_record(): expected 7 fields, but got 0 in %s on line %d
bool(false)

Warning: dbase_replace_record(): expected plain indexed array in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_error.dbf');
?>
