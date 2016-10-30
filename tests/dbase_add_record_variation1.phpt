--TEST--
dbase_add_record(): duplicate an existing record from numeric array
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_variation1.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, DBASE_RDWR);
var_dump($db);

$record = dbase_get_record($db, 1);
var_dump($record);

unset($record['deleted']);
$record[4] = $record[4] ? 'T' : 'F'; // we have to cater to bool fields ourselves
var_dump(dbase_add_record($db, $record));

var_dump(dbase_get_record($db, 4));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
resource(%d) of type (dbase)
array(7) {
  [0]=>
  int(1)
  [1]=>
  string(25) "dBase III                "
  [2]=>
  string(8) "19840501"
  [3]=>
  string(18) "19840501000000.000"
  [4]=>
  bool(true)
  [5]=>
  float(123.45)
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
  string(18) "19840501000000.000"
  [4]=>
  bool(true)
  [5]=>
  float(123.45)
  ["deleted"]=>
  int(0)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_variation1.dbf';
unlink($filename);
?>
