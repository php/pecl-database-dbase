--TEST--
Bug #73391 (Writing of floats can cause OOB reads)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extesion not available');
?>
--FILE--
<?php
$db = dbase_create(
    __DIR__ . DIRECTORY_SEPARATOR . 'bug73391.dbf',
    array(
        array('num', 'N', 10, 2)
    )
);

dbase_add_record($db, array(1.23));
dbase_replace_record($db, array(1.23), 1);

dbase_close($db);
?>
===DONE===
--EXPECT--
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'bug73391.dbf');
?>
