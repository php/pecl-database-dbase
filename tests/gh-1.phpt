--TEST--
GH-1 (dbase_add_record() converts passed floats and leaks memory)
--SKIPIF--
<?php
if (!extension_loaded("dbase")) die("skip dbase extension not available");
?>
--FILE--
<?php
$filename = __DIR__ . "/gh-1.dbf";
$db = dbase_create($filename, array(array('num', 'n', 10, 1)));

$record = array(1234.5);
dbase_add_record($db, $record);
var_dump($record);
?>
--EXPECT--
array(1) {
  [0]=>
  float(1234.5)
}
--CLEAN--
<?php
unlink(__DIR__ . "/gh-1.dbf");
?>
