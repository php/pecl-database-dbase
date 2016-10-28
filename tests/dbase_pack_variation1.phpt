--TEST--
dbase_pack(): date of last update is properly updated
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--INI--
date.timezone=UTC
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_pack_variation1.dbf';
copy(__DIR__ . DIRECTORY_SEPARATOR . 'example.dbf', $filename);
$db = dbase_open($filename, 2);
dbase_pack($db);
dbase_close($db);
$date = getdate();
$result = unpack('C4byte', file_get_contents($filename));
var_dump(
    $result['byte2'] + 1900 == $date['year'],
    $result['byte3'] == $date['mon'],
    $result['byte4'] == $date['mday']
);
?>
===DONE===
--EXPECT--
bool(true)
bool(true)
bool(true)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_pack_variation1.dbf');
?>
