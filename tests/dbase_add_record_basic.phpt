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

$db = dbase_open($filename, DBASE_RDWR);
var_dump($db);

var_dump(dbase_add_record($db, array(4, 'JPLDIS', '19730101', '19730101104923.123', 'F', 1234567.89, 1.23)));

var_dump(dbase_numrecords($db));
var_dump(dbase_get_record($db, 4));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
object(Dbase\DbaseHandle)#%d (0) {
}
bool(true)
int(4)
array(8) {
  [0]=>
  int(4)
  [1]=>
  string(25) "JPLDIS                   "
  [2]=>
  string(8) "19730101"
  [3]=>
  string(18) "19730101104923.123"
  [4]=>
  bool(false)
  [5]=>
  float(1234567.89)
  [6]=>
  float(1.23)
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
