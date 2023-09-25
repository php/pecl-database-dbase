--TEST--
dbase_delete_record(): basic functionality
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_delete_record_basic.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, DBASE_RDWR);
var_dump($db);

var_dump(dbase_delete_record($db, 2));

var_dump(dbase_numrecords($db));
var_dump(dbase_get_record($db, 2));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
object(Dbase\DbaseHandle)#%d (0) {
}
bool(true)
int(3)
array(8) {
  [0]=>
  int(2)
  [1]=>
  string(25) "Clipper                  "
  [2]=>
  string(8) "19850525"
  [3]=>
  string(18) "19850525120000.000"
  [4]=>
  bool(false)
  [5]=>
  float(56.78)
  [6]=>
  float(23.45)
  ["deleted"]=>
  int(1)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_delete_record_basic.dbf';
unlink($filename);
?>
