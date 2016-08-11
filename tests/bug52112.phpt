--TEST--
Bug #52112 (dbase_get_record() returns integer instead of decimal value)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
$locales = array('de_DE.UTF-8', 'de-DE');
if (array_search(setlocale(LC_NUMERIC, $locales), $locales) === false) {
    die('skip German locale not available');
}
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_basic.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);
setlocale(LC_NUMERIC, 'de_DE.UTF-8', 'de-DE');

$db = dbase_open($filename, 0);
var_dump($db);

var_dump(dbase_get_record($db, 1));
var_dump(dbase_get_record_with_names($db, 1));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
int(%d)
array(6) {
  [0]=>
  int(1)
  [1]=>
  string(25) "dBase III                "
  [2]=>
  string(8) "19840501"
  [3]=>
  int(1)
  [4]=>
  float(123,45)
  ["deleted"]=>
  int(0)
}
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
  float(123,45)
  ["deleted"]=>
  int(0)
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_basic.dbf';
unlink($filename);
?>
