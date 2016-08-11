--TEST--
dbase_create() - error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
?>
--FILE--
<?php
define('FILENAME', __DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_error.dbf');

/* too few arguments */
var_dump(dbase_create(FILENAME));
if (file_exists(FILENAME)) unlink(FILENAME);

/* too many arguments */
var_dump(dbase_create(FILENAME, array(), 'additional argument'));
if (file_exists(FILENAME)) unlink(FILENAME);

/* no fields */
var_dump(dbase_create(FILENAME, array()));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field without name */
var_dump(dbase_create(FILENAME, array(array())));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field with empty name */
var_dump(dbase_create(FILENAME, array(array(''))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field name too long */
var_dump(dbase_create(FILENAME, array(array('abcdefghijk'))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field type missing */
var_dump(dbase_create(FILENAME, array(array('foo'))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field length missing */
var_dump(dbase_create(FILENAME, array(array('foo', 'C'))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field precision missing */
var_dump(dbase_create(FILENAME, array(array('foo', 'N', 10))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* unknown field type */
var_dump(dbase_create(FILENAME, array(array('foo', '~'))));
if (file_exists(FILENAME)) unlink(FILENAME);
?>
===DONE===
--EXPECTF--
Warning: dbase_create() expects exactly 2 parameters, 1 given in %s%edbase_create_error.php on line %d

Warning: Wrong parameter count for dbase_create() in %s%edbase_create_error.php on line %d
NULL

Warning: dbase_create() expects exactly 2 parameters, 3 given in %s%edbase_create_error.php on line %d

Warning: Wrong parameter count for dbase_create() in %s%edbase_create_error.php on line %d
NULL

Warning: dbase_create(): Unable to create database without fields in %s%edbase_create_error.php on line %d
bool(false)

Warning: dbase_create(): expected field name as first element of list in field 0 in %s%edbase_create_error.php on line %d
bool(false)

Warning: dbase_create(): invalid field name '' (must be non-empty and less than or equal to 10 characters) in %s%edbase_create_error.php on line %d
bool(false)

Warning: dbase_create(): invalid field name 'abcdefghijk' (must be non-empty and less than or equal to 10 characters) in %s%edbase_create_error.php on line %d
bool(false)

Warning: dbase_create(): expected field type as second element of list in field 0 in %s%edbase_create_error.php on line %d
bool(false)

Warning: dbase_create(): expected field length as third element of list in field 0 in %s%edbase_create_error.php on line %d
bool(false)

Warning: dbase_create(): expected field precision as fourth element of list in field 0 in %s%edbase_create_error.php on line %d
bool(false)

Warning: dbase_create(): unknown field type '~' in %s%edbase_create_error.php on line %d
bool(false)
===DONE===
--CLEAN--
<?php
define('FILENAME', __DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_error.dbf');

unlink(FILENAME);
?>
