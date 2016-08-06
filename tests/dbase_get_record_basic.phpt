--TEST--
dbase_get_record(): basic functionality
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_basic.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, 2);
var_dump($db);

var_dump(dbase_get_record($db, 3));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
int(%d)
array(6) {
  [0]=>
  int(3)
  [1]=>
  string(25) "Visual FoxPro 7.0        "
  [2]=>
  string(8) "20010627"
  [3]=>
  int(0)
  [4]=>
  float(0.9)
  ["deleted"]=>
  int(0)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_basic.dbf';
unlink($filename);
?>
