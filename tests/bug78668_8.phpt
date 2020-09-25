--TEST--
Bug #78668 (Out-of-bounds Read in dbase.c)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
?>
--FILE--
<?php
$db_path = __DIR__ . "/bug78668.dbf";
$dbh = dbase_open($db_path, 0);
try {
	$column_info = dbase_get_header_info($dbh);
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
?>
===DONE===
--EXPECTF--
Warning: dbase_open(): unable to open database %s on line %d
dbase_get_header_info(): Argument #1 ($database) must be of type resource, bool given
===DONE===
