--TEST--
dbase_pack() writes proper EOF marker
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_pack_variation2.dbf';

$db = dbase_create($filename, array(array('NAME', 'C', 15)));
dbase_add_record($db, array('foo'));
dbase_delete_record($db, 1);
dbase_pack($db);
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
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_pack_variation2.dbf');
?>
