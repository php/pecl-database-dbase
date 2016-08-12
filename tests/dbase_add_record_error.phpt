--TEST--
dbase_add_record() - error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
const FILENAME = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_error.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', FILENAME);

set_error_handler(function ($errno, $errstr) {
    echo $errstr, PHP_EOL;
});

$db = dbase_open(FILENAME, 0);
dbase_add_record($db, 1);
dbase_close($db);
?>
===DONE===
--EXPECTF--
Argument 2 passed to dbase_add_record() must be of the type array, integer given
dbase_add_record() expects parameter 2 to be array, integer given
===DONE===
--CLEAN--
<?php
const FILENAME = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_error.dbf';

unlink(FILENAME);
?>
