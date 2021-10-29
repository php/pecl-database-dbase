--TEST--
Bug #81563 (SIGSEGV reading dbase file with few data)
--SKIPIF--
<?php
if (!extension_loaded("dbase")) die("skip dbase extension not available");
?>
--FILE--
<?php
$filename = __DIR__ . "/bug81563.dbf";

$db = dbase_open($filename, 0);
$n = dbase_numrecords($db);
for ($index = 1; $index <= $n; $index++) {
    $record = dbase_get_record_with_names($db, $index);
    var_dump($record);
}
dbase_close($db);
?>
--EXPECT--
array(4) {
  ["SCHLAGNR"]=>
  int(12345)
  ["TEILSCHLAG"]=>
  string(2) "a "
  ["FLIK"]=>
  string(16) "DENWLI0546132777"
  ["deleted"]=>
  int(0)
}
