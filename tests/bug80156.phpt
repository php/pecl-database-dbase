--TEST--
Bug #80156 (Incomplete records may be written)
--SKIPIF--
<?php
if (!extension_loaded('dbase')) die('skip dbase extension not available');
if (version_compare(PHP_VERSION, '7.4', '<')) die('skip for PHP 7.4+ only');
?>
--FILE--
<?php
class Foo {
    public function __toString() {
        throw new Exception("not allowed");
    }
}

$def = [["name1", "c", 10], ["name2", "c", 10]];
$dbase = dbase_create(__DIR__ . "/bug80156.dbf", $def);
try {
    dbase_add_record($dbase, ["cmb", new Foo]);
} catch (Exception $ex) {}
var_dump(dbase_numrecords($dbase));
?>
--CLEAN--
<?php
unlink(__DIR__ . "/bug80156.dbf");
?>
--EXPECT--
int(0)
