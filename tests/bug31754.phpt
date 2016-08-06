--TEST--
Bug #31754 (dbase_open() fails for mode = 1)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) {
	die('skip dbase extension not available');
}
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'bug31754.dbf';

// database "definition"
$def = array(
	array("foo", "L")
);

// creation
$dbh = dbase_create($filename, array(array('foo', 'L')));
dbase_close($dbh);

$dbh = dbase_open($filename, 1);
?>
--EXPECTF--
Warning: dbase_open(): Cannot open %s%ebug31754.dbf in write-only mode in %s%ebug31754.php on line %d
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'bug31754.dbf';
unlink($filename);
?>
