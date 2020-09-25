--TEST--
dbase_create() - error conditions
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '8', '<')) die('skip for PHP 8 only');
?>
--FILE--
<?php
define('FILENAME', __DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_error.dbf');

/* too few arguments */
try {
	var_dump(dbase_create(FILENAME));
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
if (file_exists(FILENAME)) unlink(FILENAME);

/* too many arguments */
try {
	var_dump(dbase_create(FILENAME, array(), DBASE_TYPE_DBASE, 'additional argument'));
} catch (ArgumentCountError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
if (file_exists(FILENAME)) unlink(FILENAME);

/* second argument is no array */
try {
    dbase_create(FILENAME, 'no array');
} catch (TypeError $ex) {
    echo $ex->getMessage(), PHP_EOL;
}
if (file_exists(FILENAME)) unlink(FILENAME);

/* no fields */
var_dump(dbase_create(FILENAME, array()));
if (file_exists(FILENAME)) unlink(FILENAME);

/* associative array */
var_dump(dbase_create(FILENAME, array('foo' => 'bar')));
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

/* field length too small */
var_dump(dbase_create(FILENAME, array(array('foo', 'C', -1))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field length too large */
var_dump(dbase_create(FILENAME, array(array('foo', 'C', 255))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field precision missing */
var_dump(dbase_create(FILENAME, array(array('foo', 'N', 10))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field precision too small */
var_dump(dbase_create(FILENAME, array(array('foo', 'N', 10, -1))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* field precision too large */
var_dump(dbase_create(FILENAME, array(array('foo', 'N', 10, 255))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* unknown field type */
var_dump(dbase_create(FILENAME, array(array('foo', '~'))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* unsupported field type*/
var_dump(dbase_create(FILENAME, array(array('foo', 'T'))));
if (file_exists(FILENAME)) unlink(FILENAME);

/* unknown database type */
var_dump(dbase_create(FILENAME, array(array('foo', 'C', 15)), 17));
if (file_exists(FILENAME)) unlink(FILENAME);
?>
===DONE===
--EXPECTF--
dbase_create() expects at least 2 arguments, 1 given
dbase_create() expects at most 3 arguments, 4 given
dbase_create(): Argument #2 ($fields) must be of type array, string given

Warning: dbase_create(): Unable to create database without fields in %s on line %d
bool(false)

Warning: dbase_create(): expected plain indexed array in %s on line %d
bool(false)

Warning: dbase_create(): expected field name as element 0 of list in field 0 in %s on line %d
bool(false)

Warning: dbase_create(): invalid field name '' (must be non-empty and less than or equal to 10 characters) in %s on line %d
bool(false)

Warning: dbase_create(): invalid field name 'abcdefghijk' (must be non-empty and less than or equal to 10 characters) in %s on line %d
bool(false)

Warning: dbase_create(): expected field type as element 1 of list in field 0 in %s on line %d
bool(false)

Warning: dbase_create(): expected field length as element 2 of list in field 0 in %s on line %d
bool(false)

Warning: dbase_create(): expected length of field 0 to be in range 0..254, but got -1 in %s on line %d
bool(false)

Warning: dbase_create(): expected length of field 0 to be in range 0..254, but got 255 in %s on line %d
bool(false)

Warning: dbase_create(): expected field precision as element 3 of list in field 0 in %s on line %d
bool(false)

Warning: dbase_create(): expected precision of field 0 to be in range 0..254, but got -1 in %s on line %d
bool(false)

Warning: dbase_create(): expected precision of field 0 to be in range 0..254, but got 255 in %s on line %d
bool(false)

Warning: dbase_create(): unknown field type '~' in %s on line %d
bool(false)

Warning: dbase_create(): datetime fields are not supported by dBASE in %s on line %d
bool(false)

Warning: dbase_create(): unknown database type 17 in %s on line %d
bool(false)
===DONE===
--CLEAN--
<?php
define('FILENAME', __DIR__ . DIRECTORY_SEPARATOR . 'dbase_create_error.dbf');

unlink(FILENAME);
?>
