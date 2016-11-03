<?php

// script to create the example database file

$filename = __DIR__ . DIRECTORY_SEPARATOR . 'tests' . DIRECTORY_SEPARATOR . 'example.dbf'; 

$definition = array(
    array('ID', 'N', 5, 0),
    array('NAME', 'C', 25),
    array('RELEASED', 'D'),
    array('RELEASED_X', 'T'),
    array('SUPORTED', 'L'),
    array('PRICE', 'N', 10, 2),
    array('MARKETSHAR', 'F', 6, 2),
);
$db = dbase_create($filename, $definition);

$records = array(
    array(1, 'dBase III', '19840501', '19840501000000.000', 'T', 123.45, 34.56),
    array(2, 'Clipper', '19850525', '1985052512000000.000', 'F', 56.78, 23.45),
    array(3, 'Visual FoxPro 7.0', '20010627', '20010627235959.999', 'F', 0.90, 12.34)
);
foreach ($records as $record) {
    dbase_add_record($db, $record);
}

dbase_close($db);
