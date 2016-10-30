--TEST--
Reading and writing of datetime values
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$db = dbase_create(
    __DIR__ . DIRECTORY_SEPARATOR . 'type_datetime.dbf',
    array(
        array('datetime', 'T')
    )
);

$values = array(
    // good values
    '00010101000000.000', // minimum
    '15821015120000.000', // gregorian calendar
    '19690324101000.000', // author's birthday
    '19950608090000.000', // PHP's birthday
    '19991231235959.999', // millenium - 1msec
    '20000101000000.000', // millenium
    '20630405111500.000', // Bozeman, Montana
    '45670123123456.789', // all digits
    '99991231235959.999',  // maximum
    
    // bad values - will be silently accepted, but produce garbage
    '00000000000000.000',
    '99999999999999.999',
);

foreach ($values as $value) {
    dbase_add_record($db, array($value));
}

for ($i = 1; $i <= dbase_numrecords($db); $i++) {
    var_dump(
        dbase_get_record($db, $i)[0],
        dbase_get_record_with_names($db, $i)['datetime']
    );
}
?>
===DONE===
--EXPECT--
string(18) "00010101000000.000"
string(18) "00010101000000.000"
string(18) "15821015120000.000"
string(18) "15821015120000.000"
string(18) "19690324101000.000"
string(18) "19690324101000.000"
string(18) "19950608090000.000"
string(18) "19950608090000.000"
string(18) "19991231235959.999"
string(18) "19991231235959.999"
string(18) "20000101000000.000"
string(18) "20000101000000.000"
string(18) "20630405111500.000"
string(18) "20630405111500.000"
string(18) "45670123123456.789"
string(18) "45670123123456.789"
string(18) "99991231235959.999"
string(18) "99991231235959.999"
string(18) "00000000000000.000"
string(18) "00000000000000.000"
string(18) "000000001004039.99"
string(18) "000000001004039.99"
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'type_datetime.dbf');
?>
