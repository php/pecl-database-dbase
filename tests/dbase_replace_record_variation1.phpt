--TEST--
dbase_replace_record(): update existing record from numeric array
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_variation1.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, 2);
var_dump($db);

$record = dbase_get_record($db, 1);
var_dump($record);

unset($record['deleted']);
$record[3] = $record[3] ? 'T' : 'F'; // we have to cater to bool fields ourselves
$record[4] = 12.34;
var_dump(dbase_replace_record($db, $record, 1));

var_dump(dbase_get_record($db, 1));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
int(%d)
array(7) {
  [0]=>
  int(1)
  [1]=>
  string(25) "dBase III                "
  [2]=>
  string(8) "19840501"
  [3]=>
  int(1)
  [4]=>
  float(123.45)
  [5]=>
  float(34.56)
  ["deleted"]=>
  int(0)
}
bool(true)
array(7) {
  [0]=>
  int(1)
  [1]=>
  string(25) "dBase III                "
  [2]=>
  string(8) "19840501"
  [3]=>
  int(1)
  [4]=>
  float(12.34)
  [5]=>
  float(34.56)
  ["deleted"]=>
  int(0)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_replace_record_variation1.dbf';
unlink($filename);
?>
