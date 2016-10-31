--TEST--
Reading and writing of numeric integer values
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (PHP_INT_SIZE != 4) die('skip for 32bit platforms only');
?>
--FILE--
<?php
$db = dbase_create(
    __DIR__ . DIRECTORY_SEPARATOR . 'type_numeric_int_32bit.dbf',
    array(
        array('integer', 'N', 20, 0)
    )
);

$values = array(
    '-9223372036854775809',
    '-9223372036854775808',
    '-2147483649',
    '-2147483648',
    '0',
    '2147483647',
    '2147483648',
    '9223372036854775807',
    '9223372036854775808',
);

foreach ($values as $value) {
    dbase_add_record($db, [$value]);
}

for ($i = 1; $i <= dbase_numrecords($db); $i++) {
    var_dump(
        dbase_get_record($db, $i)[0],
        dbase_get_record_with_names($db, $i)['integer']
    );
}
?>
===DONE===
--EXPECT--
string(20) "-9223372036854775809"
string(20) "-9223372036854775809"
string(20) "-9223372036854775808"
string(20) "-9223372036854775808"
string(20) "         -2147483649"
string(20) "         -2147483649"
int(-2147483648)
int(-2147483648)
int(0)
int(0)
int(2147483647)
int(2147483647)
string(20) "          2147483648"
string(20) "          2147483648"
string(20) " 9223372036854775807"
string(20) " 9223372036854775807"
string(20) " 9223372036854775808"
string(20) " 9223372036854775808"
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'type_numeric_int_32bit.dbf');
?>
