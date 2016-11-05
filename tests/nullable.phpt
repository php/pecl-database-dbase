--TEST--
Nullable fields
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'nullable.dbf';
$spec = array(
    ['A', 'C', 15],
    ['B', 'C', 15, true],
    ['C', 'C', 15, true],
    ['D', 'N', 15, 0],
    ['E', 'N', 15, 0, true],
    ['F', 'N', 15, 0, true],
    ['G', 'L'],
    ['H', 'L', true],
    ['I', 'L', true],
    ['J', 'D'],
    ['K', 'D', true],
    ['L', 'D', true],
    ['M', 'F', 15, 2],
    ['N', 'F', 15, 2, true],
    ['O', 'F', 15, 2, true],
);

$db = dbase_create($filename, $spec, DBASE_TYPE_FOXPRO);
dbase_add_record($db, [
    null, null, 'foo',
    null, null, 42,
    null, null, 'T',
    null, null, '20161105',
    null, null, 3.14
]);
var_dump(
    dbase_numfields($db),
    count(dbase_get_header_info($db))
);
var_dump(dbase_get_record($db, 1));
dbase_close($db);

$db = dbase_open($filename, DBASE_RDONLY);
var_dump(dbase_get_record_with_names($db, 1));
?>
===DONE===
--EXPECT--
int(15)
int(15)
array(16) {
  [0]=>
  string(15) "               "
  [1]=>
  NULL
  [2]=>
  string(15) "foo            "
  [3]=>
  int(0)
  [4]=>
  NULL
  [5]=>
  int(42)
  [6]=>
  bool(false)
  [7]=>
  NULL
  [8]=>
  bool(true)
  [9]=>
  string(8) "        "
  [10]=>
  NULL
  [11]=>
  string(8) "20161105"
  [12]=>
  float(0)
  [13]=>
  NULL
  [14]=>
  float(3.14)
  ["deleted"]=>
  int(0)
}
array(16) {
  ["A"]=>
  string(15) "               "
  ["B"]=>
  NULL
  ["C"]=>
  string(15) "foo            "
  ["D"]=>
  int(0)
  ["E"]=>
  NULL
  ["F"]=>
  int(42)
  ["G"]=>
  bool(false)
  ["H"]=>
  NULL
  ["I"]=>
  bool(true)
  ["J"]=>
  string(8) "        "
  ["K"]=>
  NULL
  ["L"]=>
  string(8) "20161105"
  ["M"]=>
  float(0)
  ["N"]=>
  NULL
  ["O"]=>
  float(3.14)
  ["deleted"]=>
  int(0)
}
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'nullable.dbf');
?>
