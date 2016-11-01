--TEST--
dbase_get_header_info(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
var_dump(dbase_get_header_info('no resource'));
?>
===DONE===
--EXPECTF--
Warning: dbase_get_header_info() expects parameter 1 to be resource, string given in %s on line %d
NULL
===DONE===
