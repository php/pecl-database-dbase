--TEST--
Bug #78070 (dbase functions may modify passed array)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php

class CharacterType
{
    public function __toString()
    {
        return 'C';
    }
}

$def = array(
    [17, new CharacterType, 10],
    ['foo', 'N', '5', '0', 1]
);
$dbh = dbase_create(__DIR__ . '/bug78070.dbf', $def, DBASE_TYPE_FOXPRO);
var_dump($def);
$record = [4, 42];
dbase_add_record($dbh, $record);
var_dump($record);
dbase_close($dbh);
?>
===DONE===
--EXPECTF--
array(2) {
  [0]=>
  array(3) {
    [0]=>
    int(17)
    [1]=>
    object(CharacterType)#%d (0) {
    }
    [2]=>
    int(10)
  }
  [1]=>
  array(5) {
    [0]=>
    string(3) "foo"
    [1]=>
    string(1) "N"
    [2]=>
    string(1) "5"
    [3]=>
    string(1) "0"
    [4]=>
    int(1)
  }
}
array(2) {
  [0]=>
  int(4)
  [1]=>
  int(42)
}
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . '/bug78070.dbf');
?>
