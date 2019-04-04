--TEST--
dbase_get_header_info(): error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--INI--
allow_url_fopen=1
--FILE--
<?php
var_dump(dbase_get_header_info('no resource'));
var_dump(dbase_get_header_info(fopen('data://text/plain,foo', 'r')));
?>
===DONE===
--EXPECTF--
Warning: dbase_get_header_info() expects parameter 1 to be resource, string given in %s on line %d
NULL

Warning: dbase_get_header_info(): supplied resource is not a valid dbase resource in %s on line %d
bool(false)
===DONE===
