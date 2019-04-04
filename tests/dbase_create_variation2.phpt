--TEST--
dbase_create() writes proper EOF marker
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_variation2.dbf';

$db = dbase_create($filename, array(array('foo', 'C', 1)));
dbase_close($db);

$contents = file_get_contents($filename);
printf("%02X\n", ord(substr($contents, -1)));
?>
===DONE===
--EXPECT--
1A
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_variation2.dbf');
?>
