--TEST--
Reading and writing of logical values
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$db = dbase_create(
    __DIR__ . DIRECTORY_SEPARATOR . 'type_logical.dbf',
    array(
        array('flag', 'L')
    )
);

foreach (array('T', 'Y', 'F', 'N', ' ', '1', '0') as $flag) {
    dbase_add_record($db, array($flag));
}

for ($i = 1; $i <= dbase_numrecords($db); $i++) {
    var_dump(
        dbase_get_record($db, $i)[0],
        dbase_get_record_with_names($db, $i)['flag']
    );
}
?>
===DONE===
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'type_logical.dbf');
?>
