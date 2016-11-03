--TEST--
dbase_replace_record(): update existing record from associative array
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_variation2.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, 2);
var_dump($db);

$record = dbase_get_record_with_names($db, 1);
var_dump($record);

unset($record['deleted']);
$record['SUPORTED'] = $record['SUPORTED'] ? 'T' : 'F'; // we have to cater to bool fields ourselves
$record['PRICE'] = 12.34;
$record = array_values($record); // we can't replace with an associative array
var_dump(dbase_replace_record($db, $record, 1));

var_dump(dbase_get_record_with_names($db, 1));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
int(%d)
array(7) {
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
  ["MARKETSHAR"]=>
  float(34.56)
  ["deleted"]=>
  int(0)
}
bool(true)
array(7) {
  ["ID"]=>
  int(1)
  ["NAME"]=>
  string(25) "dBase III                "
  ["RELEASED"]=>
  string(8) "19840501"
  ["SUPORTED"]=>
  int(1)
  ["PRICE"]=>
  float(12.34)
  ["MARKETSHAR"]=>
  float(34.56)
  ["deleted"]=>
  int(0)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_variation2.dbf';
unlink($filename);
?>
