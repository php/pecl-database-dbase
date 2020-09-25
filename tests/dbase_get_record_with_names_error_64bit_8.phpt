--TEST--
dbase_get_record_with_names(): error conditions
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
try {
	var_dump(dbase_get_record_with_names());
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
try {
	var_dump(dbase_get_record_with_names(fopen('data://text/plain,foo', 'r'), 1));
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}

$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_error.dbf';
$db = dbase_create($filename, [['foo', 'C', 15]]);
try {
	var_dump(dbase_get_record_with_names($db, -1));
} catch (valueError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
var_dump(dbase_get_record_with_names($db, 1));
try {
	var_dump(dbase_get_record_with_names($db, 2147483648));
} catch (valueError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
?>
===DONE===
--EXPECTF--
dbase_get_record_with_names() expects exactly 2 arguments, 0 given
dbase_get_record_with_names(): supplied resource is not a valid dbase resource
dbase_get_record_with_names(): Argument #2 record number has to be in range 1..2147483647, but is -1

Warning: dbase_get_record_with_names(): Tried to read bad record 1 in %s on line %d
bool(false)
dbase_get_record_with_names(): Argument #2 record number has to be in range 1..2147483647, but is 2147483648
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_error.dbf');
?>
