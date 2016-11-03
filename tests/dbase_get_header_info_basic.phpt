--TEST--
dbase_get_header_info(): basic functionality
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_header_info_basic.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);

$db = dbase_open($filename, 0);
var_dump($db);

var_dump(dbase_get_header_info($db));

var_dump(dbase_close($db));
?>
===DONE===
--EXPECTF--
int(%d)
array(6) {
  [0]=>
  array(6) {
    ["name"]=>
    string(2) "ID"
    ["type"]=>
    string(6) "number"
    ["length"]=>
    int(5)
    ["precision"]=>
    int(0)
    ["format"]=>
    string(3) "%5s"
    ["offset"]=>
    int(1)
  }
  [1]=>
  array(6) {
    ["name"]=>
    string(4) "NAME"
    ["type"]=>
    string(9) "character"
    ["length"]=>
    int(25)
    ["precision"]=>
    int(0)
    ["format"]=>
    string(5) "%-25s"
    ["offset"]=>
    int(6)
  }
  [2]=>
  array(6) {
    ["name"]=>
    string(8) "RELEASED"
    ["type"]=>
    string(4) "date"
    ["length"]=>
    int(8)
    ["precision"]=>
    int(0)
    ["format"]=>
    string(3) "%8s"
    ["offset"]=>
    int(31)
  }
  [3]=>
  array(6) {
    ["name"]=>
    string(8) "SUPORTED"
    ["type"]=>
    string(7) "boolean"
    ["length"]=>
    int(1)
    ["precision"]=>
    int(0)
    ["format"]=>
    string(3) "%1s"
    ["offset"]=>
    int(39)
  }
  [4]=>
  array(6) {
    ["name"]=>
    string(5) "PRICE"
    ["type"]=>
    string(6) "number"
    ["length"]=>
    int(10)
    ["precision"]=>
    int(2)
    ["format"]=>
    string(4) "%10s"
    ["offset"]=>
    int(40)
  }
  [5]=>
  array(6) {
    ["name"]=>
    string(10) "MARKETSHAR"
    ["type"]=>
    string(5) "float"
    ["length"]=>
    int(6)
    ["precision"]=>
    int(0)
    ["format"]=>
    string(3) "%6s"
    ["offset"]=>
    int(50)
  }
}
bool(true)
===DONE===
--CLEAN--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_header_info_basic.dbf';
unlink($filename);
?>
