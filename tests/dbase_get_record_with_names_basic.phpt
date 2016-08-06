--TEST--
dbase_get_record_with_names(): basic functionality
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_basic.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, 2);
var_dump($db);

var_dump(dbase_get_record_with_names($db, 1));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
int(%d)
array(6) {
  ["ID"]=>
  int(1)
  ["NAME"]=>
  string(25) "dBase III                "
  ["RELEASED"]=>
  string(8) "19840501"
  ["SUPORTED"]=>
  int(1)
  ["PRICE"]=>
  float(123.45)
  ["deleted"]=>
  int(0)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_basic.dbf';
unlink($filename);
?>
