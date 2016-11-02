--TEST--
Bug #73442 (Float fields always have precision 0)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'bug73442.dbf';

$db = dbase_create($filename, [['FLOAT', 'F', 15, 5]]);
dbase_close($db);

$stream = fopen($filename, 'r');
fseek($stream, 48);
$bytes = fread($stream, 2);
var_dump(unpack('Clength/Cprecision', $bytes));
?>
===DONE===
--EXPECT--
array(2) {
  ["length"]=>
  int(15)
  ["precision"]=>
  int(5)
}
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'bug73442.dbf');
?>
