--TEST--
Bug #39305 (Use of decimal point in different countries)
--SKIPIF--
<?php
$locales = array('de_DE.UTF-8', 'de-DE');

if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (array_search(setlocale(LC_NUMERIC, $locales), $locales) === false) {
    die('skip German locale not available');
}
?>
--FILE--
<?php
$locales = array('de_DE.UTF-8', 'de-DE');
define('FILENAME', __DIR__ . DIRECTORY_SEPARATOR . 'bug39305.dbf');

$db = dbase_create(FILENAME, array(array('num', 'n', 18, 2)));

setlocale(LC_NUMERIC, $locales);
dbase_add_record($db, array(1e14));
$record = dbase_get_record_with_names($db, 1);

setlocale(LC_NUMERIC, 'C');
var_dump($record['num']);

setlocale(LC_NUMERIC, $locales);
dbase_replace_record($db, array(1e14), 1);
$record = dbase_get_record_with_names($db, 1);

setlocale(LC_NUMERIC, 'C');
var_dump($record['num']);

dbase_close($db);
?>
===DONE===
--EXPECT--
float(1.0E+14)
float(1.0E+14)
===DONE===
--CLEAN--
<?php
define('FILENAME', __DIR__ . DIRECTORY_SEPARATOR . 'bug39305.dbf');

unlink(FILENAME);
?>
