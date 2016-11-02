--TEST--
Bug #73447 (Floats written to character fields are truncated to integer)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'bug73447.dbf';
$db = dbase_create($filename, [['FLOAT', 'C', 15]]);

dbase_add_record($db, [123.45]);
var_dump(dbase_get_record_with_names($db, 1));
dbase_replace_record($db, [987.65], 1);
var_dump(dbase_get_record_with_names($db, 1));
?>
===DONE===
--EXPECT--
array(2) {
  ["FLOAT"]=>
  string(15) "123.45         "
  ["deleted"]=>
  int(0)
}
array(2) {
  ["FLOAT"]=>
  string(15) "987.65         "
  ["deleted"]=>
  int(0)
}
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'bug73447.dbf');
?>
