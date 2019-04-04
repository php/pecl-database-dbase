--TEST--
dbase_get_record_with_names(): 'deleted' always contains deletion marker
--DESCRIPTION--
Even if the database has a field named 'deleted', dbase_get_record_with_names()
will not return its value, but rather the deletion marker.
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$db = dbase_create(
    __DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_variation1.dbf',
    array(
        array('deleted', 'C', 15)
    )
);
dbase_add_record($db, array('foo'));
var_dump(dbase_get_record_with_names($db, 1));
?>
===DONE===
--EXPECT--
array(1) {
  ["deleted"]=>
  int(0)
}
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_get_record_with_names_variation1.dbf');
?>
