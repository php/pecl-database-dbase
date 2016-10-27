--TEST--
dbase_add_record(): basic functionality
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_basic.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, 2);
var_dump($db);

var_dump(dbase_add_record($db, array(4, 'JPLDIS', '19730101', 'F', 1234567.89)));

var_dump(dbase_numrecords($db));
var_dump(dbase_get_record($db, 4));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
resource(%d) of type (dbase)
bool(true)
int(4)
array(6) {
  [0]=>
  int(4)
  [1]=>
  string(25) "JPLDIS                   "
  [2]=>
  string(8) "19730101"
  [3]=>
  int(0)
  [4]=>
  float(1234567.89)
  ["deleted"]=>
  int(0)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_add_record_basic.dbf';
unlink($filename);
?>
