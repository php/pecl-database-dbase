--TEST--
dbase_create() creates a FoxPro DBF
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
$filename = __DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_variation3.dbf';

$db = dbase_create($filename, [['DATETIME', 'T']], DBASE_TYPE_FOXPRO);
dbase_close($db);

$contents = file_get_contents($filename);
var_dump(
    strlen($contents),
    ord($contents[0]),  // version
    ord($contents[64]), // header record terminator
    ord($contents[65]), // first byte of DBC
    ord($contents[328]) // end of file marker
);
?>
===DONE===
--EXPECT--
int(329)
int(48)
int(13)
int(0)
int(26)
===DONE===
--CLEAN--
<?php
unlink(__DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_variation3.dbf');
?>
