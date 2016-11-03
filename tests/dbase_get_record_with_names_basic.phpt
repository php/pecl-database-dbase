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

$db = dbase_open($filename, DBASE_RDWR);
var_dump($db);

var_dump(dbase_get_record_with_names($db, 1));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
resource(%d) of type (dbase)
array(8) {
  ["ID"]=>
  int(1)
  ["NAME"]=>
  string(25) "dBase III                "
  ["RELEASED"]=>
  string(8) "19840501"
  ["RELEASED_X"]=>
  string(18) "19840501000000.000"
  ["SUPORTED"]=>
  bool(true)
  ["PRICE"]=>
  float(123.45)
  ["MARKETSHAR"]=>
  float(34.56)
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
