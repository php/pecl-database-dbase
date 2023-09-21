--TEST--
Bug #78668 (Out-of-bounds Read in dbase.c)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '>')) die('skip for PHP 7 only');
?>
--FILE--
<?php
$db_path = __DIR__ . "/bug78668.dbf";
$dbh = dbase_open($db_path, 0);
$column_info = dbase_get_header_info($dbh);
?>
===DONE===
--EXPECTF--
Warning: dbase_open(): unable to open database %s on line %d

Warning: dbase_get_header_info() expects parameter 1 to be Dbase\DbaseHandle, bool%S given in %s on line %d
===DONE===
