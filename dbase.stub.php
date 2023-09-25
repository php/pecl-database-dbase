<?php

/** @generate-function-entries */

namespace {
    function dbase_open(string $path, int $mode): Dbase\DbaseHandle|false {}

    function dbase_close(Dbase\DbaseHandle $database): bool {}

    function dbase_add_record(Dbase\DbaseHandle $database, array $data): bool {}

    function dbase_delete_record(Dbase\DbaseHandle $database, int $number): bool {}

    function dbase_replace_record(Dbase\DbaseHandle $database, array $data, int $number): bool {}

    function dbase_numrecords(Dbase\DbaseHandle $database): int {}

    function dbase_numfields(Dbase\DbaseHandle $database): int {}

    function dbase_pack(Dbase\DbaseHandle $database): bool {}

    function dbase_get_record(Dbase\DbaseHandle $database, int $number): array|false {}

    function dbase_get_record_with_names(Dbase\DbaseHandle $database, int $number): array|false {}

    function dbase_create(string $path, array $fields, int $type = DBASE_TYPE_DBASE): Dbase\DbaseHandle|false {}

    function dbase_get_header_info(Dbase\DbaseHandle $database): array {}
}

namespace Dbase {
    /** @strict-properties */
    final class DbaseHandle
    {
    }
}
