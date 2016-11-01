/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Jim Winstead <jimw@php.net>                                  |
   +----------------------------------------------------------------------+
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "fopen_wrappers.h"
#include "php_globals.h"
#include "ext/standard/php_math.h"
#include "ext/standard/flock_compat.h"

#include <stdlib.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if DBASE
#include "php_dbase.h"
#include "dbf.h"

static int le_dbhead;

#include <fcntl.h>
#include <errno.h>


static void _close_dbase(zend_resource *rsrc)
{
	dbhead_t *dbhead = (dbhead_t *)rsrc->ptr;

	php_flock(dbhead->db_fd, LOCK_UN);
	close(dbhead->db_fd);
	free_dbf_head(dbhead);
}


PHP_MINIT_FUNCTION(dbase)
{
	le_dbhead = zend_register_list_destructors_ex(_close_dbase, NULL, "dbase", module_number);

	REGISTER_STRING_CONSTANT("DBASE_VERSION", PHP_DBASE_VERSION, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("DBASE_RDONLY", O_RDONLY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DBASE_RDWR",   O_RDWR,   CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}

/* {{{ proto resource dbase_open(string name, int mode)
   Opens a dBase-format database file */
PHP_FUNCTION(dbase_open)
{
	zend_string *dbf_name;
	zend_long mode;
	dbhead_t *dbh;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Pl", &dbf_name, &mode) == FAILURE) {	
		return;
	}

	if (!ZSTR_LEN(dbf_name)) {
		php_error_docref(NULL, E_WARNING, "The filename cannot be empty.");
		RETURN_FALSE;
	}

	if (mode == O_WRONLY) {
		php_error_docref(NULL, E_WARNING, "Cannot open %s in write-only mode", ZSTR_VAL(dbf_name));
		RETURN_FALSE;
	} else if (mode != O_RDONLY && mode != O_RDWR) {
		php_error_docref(NULL, E_WARNING, "Invalid access mode %ld", mode);
		RETURN_FALSE;
	}

	if (php_check_open_basedir(ZSTR_VAL(dbf_name))) {
		RETURN_FALSE;
	}

	dbh = dbf_open(ZSTR_VAL(dbf_name), (int) mode);
	if (dbh == NULL) {
		php_error_docref(NULL, E_WARNING, "unable to open database %s", ZSTR_VAL(dbf_name));
		RETURN_FALSE;
	}

	RETURN_RES(zend_register_resource(dbh, le_dbhead));
}
/* }}} */

/* {{{ proto bool dbase_close(resource identifier)
   Closes an open dBase-format database file */
PHP_FUNCTION(dbase_close)
{
	zval *dbh_id;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &dbh_id) == FAILURE) {
		return;
	}

	if (zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead) == NULL) {
		RETURN_FALSE;
	}
	
	zend_list_close(Z_RES_P(dbh_id));
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int dbase_numrecords(resource identifier)
   Returns the number of records in the database */
PHP_FUNCTION(dbase_numrecords)
{
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &dbh_id) == FAILURE) {
		return;
	}
	
	if ((dbht = (dbhead_t *) zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead)) == NULL) {
		RETURN_FALSE;
	}

	RETURN_LONG(dbht->db_records);

}
/* }}} */

/* {{{ proto int dbase_numfields(resource identifier)
   Returns the number of fields (columns) in the database */
PHP_FUNCTION(dbase_numfields)
{
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &dbh_id) == FAILURE) {
		return;
	}
	
	if ((dbht = (dbhead_t *) zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead)) == NULL) {
		RETURN_FALSE;
	}

	RETURN_LONG(dbht->db_nfields);
}
/* }}} */

/* {{{ proto bool dbase_pack(resource identifier)
   Packs the database (deletes records marked for deletion) */
PHP_FUNCTION(dbase_pack)
{
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &dbh_id) == FAILURE) {
		return;
	}
	
	if ((dbht = (dbhead_t *) zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead)) == NULL) {
		RETURN_FALSE;
	}

	if (!pack_dbf(dbht)) {
        if (put_dbf_info(dbht) != 1) {
			RETURN_FALSE;
		}
		if (put_dbf_eof_marker(dbht)) {
			RETURN_FALSE;
		}
		RETURN_TRUE;
	}
	RETURN_FALSE;

}
/* }}} */

/* {{{ php_dbase_put_record
 */  
static void php_dbase_put_record(INTERNAL_FUNCTION_PARAMETERS, int replace)
{
	HashTable *fields;
	zval *field;
	zend_long recnum;
	zval *dbh_id;
	dbhead_t *dbht;
	int num_fields;
	dbfield_t *dbf, *cur_f;
	char *cp, *t_cp;
	int i;

	if (replace) {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "rhl", &dbh_id, &fields, &recnum) == FAILURE) {
			return;
		}
		if (recnum < 1 || recnum > 0x7FFFFFFF) {
			zend_string *str = zend_long_to_str(recnum);

			php_error_docref(NULL, E_WARNING, "record number has to be in range 1..2147483647, but is %s", ZSTR_VAL(str));
			zend_string_free(str);
			RETURN_FALSE;
		}
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "rh", &dbh_id, &fields) == FAILURE) {	
			return;
		}
	}

	if ((dbht = (dbhead_t *) zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead)) == NULL) {
		RETURN_FALSE;
	}

	num_fields = zend_hash_num_elements(fields);

	if (num_fields != dbht->db_nfields) {
		php_error_docref(NULL, E_WARNING, "expected %d fields, but got %d", dbht->db_nfields, num_fields);
		RETURN_FALSE;
	}

	cp = t_cp = (char *)emalloc(dbht->db_rlen + 1);
	*t_cp++ = VALID_RECORD;

	dbf = dbht->db_fields;
	for (i = 0, cur_f = dbf; cur_f < &dbf[num_fields]; i++, cur_f++) {
		if ((field = zend_hash_index_find(fields, i)) == NULL) {
			php_error_docref(NULL, E_WARNING, "unexpected error");
			efree(cp);
			RETURN_FALSE;
		}

		/* force cast to string as if in C locale */
		if (Z_TYPE_P(field) == IS_DOUBLE) {
			zend_string *formatted;

			formatted = _php_math_number_format_ex(Z_DVAL_P(field), cur_f->db_fdc, ".", 1, "", 0);
			ZVAL_STRING(field, ZSTR_VAL(formatted));
			zend_string_free(formatted);
		}

		convert_to_string(field);

		switch (cur_f->db_type) {
			case 'T':
				{
					int jdn, msecs;

					db_get_timestamp(Z_STRVAL_P(field), &jdn, &msecs);
					put_long(t_cp, jdn);
					put_long(t_cp + 4, msecs);
				}
				break;
			default:
				snprintf(t_cp, cur_f->db_flen+1, cur_f->db_format, Z_STRVAL_P(field));
		}

		t_cp += cur_f->db_flen;
	}

	if (!replace) {
			dbht->db_records++;
	}

	if (put_dbf_record(dbht, (replace ? (long) recnum : dbht->db_records), cp) < 0) {
		php_error_docref(NULL, E_WARNING, "unable to put record at %ld", dbht->db_records);
		efree(cp);
		RETURN_FALSE;
	}

	efree(cp);
	if (put_dbf_info(dbht) != 1) {
		RETURN_FALSE;
	}

	if (put_dbf_eof_marker(dbht)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool dbase_add_record(resource identifier, array data)
   Adds a record to the database */
PHP_FUNCTION(dbase_add_record)
{
	php_dbase_put_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ proto bool dbase_replace_record(resource identifier, array data, int recnum)
   Replaces a record to the database */
PHP_FUNCTION(dbase_replace_record)
{
	php_dbase_put_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto bool dbase_delete_record(resource identifier, int record)
   Marks a record to be deleted */
PHP_FUNCTION(dbase_delete_record)
{
	zend_long record;
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &dbh_id, &record) == FAILURE) {
		return;
	}

	if ((dbht = (dbhead_t *) zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead)) == NULL) {
		RETURN_FALSE;
	}

	if (record < 1 || record > 0x7FFFFFFF) {
		zend_string *str = zend_long_to_str(record);

		php_error_docref(NULL, E_WARNING, "record number has to be in range 1..2147483647, but is %s", ZSTR_VAL(str));
		zend_string_free(str);
		RETURN_FALSE;
	}

	if (del_dbf_record(dbht, (long) record) < 0) {
		if (record > dbht->db_records) {
			php_error_docref(NULL, E_WARNING, "record %ld out of bounds", record);
		} else {
			php_error_docref(NULL, E_WARNING, "unable to delete record %ld", record);
		}
		RETURN_FALSE;
	}

	if (put_dbf_info(dbht) != 1) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ php_dbase_get_record
 */  
static void php_dbase_get_record(INTERNAL_FUNCTION_PARAMETERS, int assoc)
{
	zend_long record;
	dbhead_t *dbht;
	zval *dbh_id;
	dbfield_t *dbf, *cur_f;
	char *data, *fnp, *str_value;
	size_t cursize = 0;
	zend_long overflow_test;
	int errno_save;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &dbh_id, &record) == FAILURE) {
		return;
	}

	if ((dbht = (dbhead_t *) zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead)) == NULL) {
		RETURN_FALSE;
	}

	if (record < 1 || record > 0x7FFFFFFF) {
		zend_string *str = zend_long_to_str(record);

		php_error_docref(NULL, E_WARNING, "record number has to be in range 1..2147483647, but is %s", ZSTR_VAL(str));
		zend_string_free(str);
		RETURN_FALSE;
	}

	if ((data = get_dbf_record(dbht, (long) record)) == NULL) {
		php_error_docref(NULL, E_WARNING, "Tried to read bad record %ld", record);
		RETURN_FALSE;
	}

	dbf = dbht->db_fields;

	array_init(return_value);

	fnp = NULL;
	for (cur_f = dbf; cur_f < &dbf[dbht->db_nfields]; cur_f++) {
		/* get the value */
		str_value = (char *)emalloc(cur_f->db_flen + 1);

		if(cursize <= (unsigned)cur_f->db_flen) {
			cursize = cur_f->db_flen + 1;
			fnp = erealloc(fnp, cursize);
		}
		if (*cur_f->db_format) {
			snprintf(str_value, cursize, cur_f->db_format, get_field_val(data, cur_f, fnp));
		} else {
			memcpy(str_value, get_binary_field_val(data, cur_f, fnp), cur_f->db_flen);
		}

		/* now convert it to the right php internal type */
		switch (cur_f->db_type) {
			case 'C':
			case 'D':
				if (!assoc) {
					add_next_index_string(return_value, str_value);
				} else {
					add_assoc_string(return_value, cur_f->db_fname, str_value);
				}
				break;
			case 'I':	/* FALLS THROUGH */
			case 'N':
				if (cur_f->db_fdc == 0) {
					/* Large integers in dbase can be larger than long */
					errno_save = errno;
					overflow_test = ZEND_STRTOL(str_value, NULL, 10);
					if (errno == ERANGE) {
					    /* If the integer is too large, keep it as string */
						if (!assoc) {
						    add_next_index_string(return_value, str_value);
						} else {
						    add_assoc_string(return_value, cur_f->db_fname, str_value);
						}
					} else {
						if (!assoc) {
						    add_next_index_long(return_value, overflow_test);
						} else {
						    add_assoc_long(return_value, cur_f->db_fname, overflow_test);
						}
					}
					errno = errno_save;
				} else {
					if (!assoc) {
						add_next_index_double(return_value, zend_strtod(str_value, NULL));
					} else {
						add_assoc_double(return_value, cur_f->db_fname, zend_strtod(str_value, NULL));
					}
				}
				break;
			case 'F':
				if (!assoc) {
					add_next_index_double(return_value, zend_strtod(str_value, NULL));
				} else {
					add_assoc_double(return_value, cur_f->db_fname, zend_strtod(str_value, NULL));
				}
				break;
			case 'L':	/* we used to FALL THROUGH, but now we check for T/Y and F/N
						   and insert 1 or 0, respectively.  db_fdc is the number of
						   decimals, which we don't care about.      3/14/2001 LEW */
				if ((*str_value == 'T') || (*str_value == 'Y')) {
					if (!assoc) {
						add_next_index_bool(return_value, 1);
					} else {
						add_assoc_bool(return_value, cur_f->db_fname, 1);
					}
				} else if (*str_value == '?') {
					if (!assoc) {
						add_next_index_null(return_value);
					} else {
						add_assoc_null(return_value, cur_f->db_fname);
					}
				} else {
					if (!assoc) {
						add_next_index_bool(return_value, 0);
					} else {
						add_assoc_bool(return_value, cur_f->db_fname, 0);
					}
				}
				break;
			case 'M':
				/* this is a memo field. don't know how to deal with this yet */
				break;
			case 'T':
				{
					char buf[19];

					db_set_timestamp(buf, get_long(str_value), get_long(str_value + 4));
					if (!assoc) {
						add_next_index_string(return_value, buf);
					} else {
						add_assoc_string(return_value, cur_f->db_fname, buf);
					}
				}
				break;
			default:
				/* should deal with this in some way */
				break;
		}
		efree(str_value);
	}

	efree(fnp);

	/* mark whether this record was deleted */
	if (data[0] == '*') {
		add_assoc_long(return_value, "deleted", 1);
	} else {
		add_assoc_long(return_value, "deleted", 0);
	}

	efree(data);
}
/* }}} */
 
/* {{{ proto array dbase_get_record(resource identifier, int record)
   Returns an array representing a record from the database */
PHP_FUNCTION(dbase_get_record)
{
	php_dbase_get_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* From Martin Kuba <makub@aida.inet.cz> */
/* {{{ proto array dbase_get_record_with_names(resource identifier, int record)
   Returns an associative array representing a record from the database */
PHP_FUNCTION(dbase_get_record_with_names)
{
	php_dbase_get_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto resource dbase_create(string filename, array fields)
   Creates a new dBase-format database file */
PHP_FUNCTION(dbase_create)
{
	zend_string *filename;
	HashTable *fields;
	zval *field, *value;
	int fd;
	dbhead_t *dbh;

	int num_fields;
	dbfield_t *dbf, *cur_f;
	int i, rlen;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Ph", &filename, &fields) == FAILURE) {
		return;
	}

	if (php_check_open_basedir(ZSTR_VAL(filename))) {
		RETURN_FALSE;
	}

	if ((fd = VCWD_OPEN_MODE(ZSTR_VAL(filename), O_BINARY|O_RDWR|O_CREAT, 0644)) < 0) {
		php_error_docref(NULL, E_WARNING, "Unable to create database (%d): %s", errno, strerror(errno));
		RETURN_FALSE;
	}

	if (php_flock(fd, LOCK_EX)) {
		php_error_docref(NULL, E_WARNING, "unable to lock database");
		close(fd);
		RETURN_FALSE;
	}

	num_fields = zend_hash_num_elements(fields);

	if (num_fields <= 0) {
		php_error_docref(NULL, E_WARNING, "Unable to create database without fields");
		close(fd);
		RETURN_FALSE;
	}

	dbh = (dbhead_t *)emalloc(sizeof(dbhead_t));
	dbf = (dbfield_t *)emalloc(sizeof(dbfield_t) * num_fields);
	
	/* initialize the header structure */
	dbh->db_fields = dbf;
	dbh->db_fd = fd;
	dbh->db_dbt = DBH_TYPE_NORMAL;
	strcpy(dbh->db_date, "19930818");
	dbh->db_records = 0;
	dbh->db_nfields = num_fields;
	dbh->db_hlen = sizeof(struct dbf_dhead) + 1 + num_fields * sizeof(struct dbf_dfield);

	rlen = 1;
	/**
	 * Patch by greg@darkphoton.com
	 **/
	/* make sure that the db_format entries for all fields are set to NULL to ensure we
       don't seg fault if there's an error and we need to call free_dbf_head() before all
       fields have been defined. */
	for (i = 0, cur_f = dbf; i < num_fields; i++, cur_f++) {
		cur_f->db_format = NULL;
	}
	/**
	 * end patch
	 */


	for (i = 0, cur_f = dbf; i < num_fields; i++, cur_f++) {
		/* look up the first field */
		if ((field = zend_hash_index_find(fields, i)) == NULL) {
			php_error_docref(NULL, E_WARNING, "unable to find field %d", i);
			free_dbf_head(dbh);
			close(fd);
			RETURN_FALSE;
		}

		/* field name */
		
		if (Z_TYPE_P(field) != IS_ARRAY || (value = zend_hash_index_find(Z_ARRVAL_P(field), 0)) == NULL) {
			php_error_docref(NULL, E_WARNING, "expected field name as first element of list in field %d", i);
			free_dbf_head(dbh);
			close(fd);
			RETURN_FALSE;
		}
		convert_to_string_ex(value);
		if (Z_STRLEN_P(value) > 10 || Z_STRLEN_P(value) == 0) {
			php_error_docref(NULL, E_WARNING, "invalid field name '%s' (must be non-empty and less than or equal to 10 characters)", Z_STRVAL_P(value));
			free_dbf_head(dbh);
			close(fd);
			RETURN_FALSE;
		}
		copy_crimp(cur_f->db_fname, Z_STRVAL_P(value), (int) Z_STRLEN_P(value));

		/* field type */
		if ((value = zend_hash_index_find(Z_ARRVAL_P(field), 1)) == NULL) {
			php_error_docref(NULL, E_WARNING, "expected field type as second element of list in field %d", i);
			free_dbf_head(dbh);
			close(fd);
			RETURN_FALSE;
		}
		convert_to_string_ex(value);
		cur_f->db_type = toupper(*Z_STRVAL_P(value));

		cur_f->db_fdc = 0;

		/* verify the field length */
		switch (cur_f->db_type) {
		case 'L':
			cur_f->db_flen = 1;
			break;
		case 'M':
			cur_f->db_flen = 10;
			dbh->db_dbt = DBH_TYPE_MEMO;
			/* should create the memo file here, probably */
			break;
		case 'D':
		case 'T':
			cur_f->db_flen = 8;
			break;
		case 'F':
			cur_f->db_flen = 20;
			break;
		case 'N':
		case 'C':
			/* field length */
			if ((value = zend_hash_index_find(Z_ARRVAL_P(field), 2)) == NULL) {
				php_error_docref(NULL, E_WARNING, "expected field length as third element of list in field %d", i);
				free_dbf_head(dbh);
				close(fd);
				RETURN_FALSE;
			}
			convert_to_long_ex(value);
			if (Z_LVAL_P(value) < 0 || Z_LVAL_P(value) > 254) {
				php_error_docref(NULL, E_WARNING, "expected length of field %d to be in range 0..254, but got %d", i, Z_LVAL_P(value));
				free_dbf_head(dbh);
				close(fd);
				RETURN_FALSE;
			}
			cur_f->db_flen = (unsigned char) Z_LVAL_P(value);

			if (cur_f->db_type == 'N') {
				if ((value = zend_hash_index_find(Z_ARRVAL_P(field), 3)) == NULL) {
					php_error_docref(NULL, E_WARNING, "expected field precision as fourth element of list in field %d", i);
					free_dbf_head(dbh);
					close(fd);
					RETURN_FALSE;
				}
				convert_to_long_ex(value);
				if (Z_LVAL_P(value) < 0 || Z_LVAL_P(value) > 254) {
					php_error_docref(NULL, E_WARNING, "expected precision of field %d to be in range 0..254, but got %d", i, Z_LVAL_P(value));
					free_dbf_head(dbh);
					close(fd);
					RETURN_FALSE;
				}
				cur_f->db_fdc = (unsigned char) Z_LVAL_P(value);
			}
			break;
		default:
			php_error_docref(NULL, E_WARNING, "unknown field type '%c'", cur_f->db_type);
			free_dbf_head(dbh);
			close(fd);
			RETURN_FALSE;
		}
		cur_f->db_foffset = rlen;
		rlen += cur_f->db_flen;
	
		cur_f->db_format = get_dbf_f_fmt(cur_f);
	}

	dbh->db_rlen = rlen;

	if (put_dbf_info(dbh) != 1 || put_dbf_eof_marker(dbh)) {
		free_dbf_head(dbh);
		close(fd);
		RETURN_FALSE;
	}

	RETURN_RES(zend_register_resource(dbh, le_dbhead));
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_dbase_open, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_close, 0)
	ZEND_ARG_INFO(0, identifier)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_numrecords, 0)
	ZEND_ARG_INFO(0, identifier)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_numfields, 0)
	ZEND_ARG_INFO(0, identifier)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_pack, 0)
	ZEND_ARG_INFO(0, identifier)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_add_record, 0)
	ZEND_ARG_INFO(0, identifier)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_replace_record, 0)
	ZEND_ARG_INFO(0, identifier)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
	ZEND_ARG_INFO(0, recnum)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_delete_record, 0)
	ZEND_ARG_INFO(0, identifier)
	ZEND_ARG_INFO(0, record)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_get_record, 0)
	ZEND_ARG_INFO(0, identifier)
	ZEND_ARG_INFO(0, record)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_get_record_with_names, 0)
	ZEND_ARG_INFO(0, identifier)
	ZEND_ARG_INFO(0, record)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_create, 0)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_ARRAY_INFO(0, fields, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_get_header_info, 0)
	ZEND_ARG_INFO(0, database_handle)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ dbase_functions[]
 */
const zend_function_entry dbase_functions[] = {
	PHP_FE(dbase_open,								arginfo_dbase_open)
	PHP_FE(dbase_create,							arginfo_dbase_create)
	PHP_FE(dbase_close,								arginfo_dbase_close)
	PHP_FE(dbase_numrecords,						arginfo_dbase_numrecords)
	PHP_FE(dbase_numfields,							arginfo_dbase_numfields)
	PHP_FE(dbase_add_record,						arginfo_dbase_add_record)
	PHP_FE(dbase_replace_record,					arginfo_dbase_replace_record)
	PHP_FE(dbase_get_record,						arginfo_dbase_get_record)
	PHP_FE(dbase_get_record_with_names,				arginfo_dbase_get_record_with_names)
	PHP_FE(dbase_delete_record,						arginfo_dbase_delete_record)
	PHP_FE(dbase_pack,								arginfo_dbase_pack)
	PHP_FE(dbase_get_header_info,					arginfo_dbase_get_header_info)
	{NULL, NULL, NULL}
};
/* }}} */

/* Added by Zak Greant <zak@php.net> */
/* {{{ proto array dbase_get_header_info(resource database_handle)
 */
PHP_FUNCTION(dbase_get_header_info)
{
	zval		row;
	dbhead_t 	*dbht;
	zval		*dbh_id;
	dbfield_t	*dbf, *cur_f;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &dbh_id) == FAILURE) {
		return;
	}

	if ((dbht = (dbhead_t *) zend_fetch_resource(Z_RES_P(dbh_id), "dbase", le_dbhead)) == NULL) {
		RETURN_FALSE;
	}

	array_init(return_value);

	dbf = dbht->db_fields;
	for (cur_f = dbf; cur_f < &dbht->db_fields[dbht->db_nfields]; ++cur_f) {
		
		array_init(&row);
		
		add_next_index_zval(return_value, &row);
		
		/* field name */
		add_assoc_string(&row, "name", cur_f->db_fname);
		
		/* field type */
		switch (cur_f->db_type) {
			case 'C': add_assoc_string(&row, "type", "character"); break;
			case 'D': add_assoc_string(&row, "type", "date");      break;
			case 'T': add_assoc_string(&row, "type", "datetime");  break;
			case 'I': add_assoc_string(&row, "type", "integer");   break;
			case 'N': add_assoc_string(&row, "type", "number");    break;
			case 'L': add_assoc_string(&row, "type", "boolean");   break;
			case 'M': add_assoc_string(&row, "type", "memo");      break;
			case 'F': add_assoc_string(&row, "type", "float");     break;
			default:  add_assoc_string(&row, "type", "unknown");   break;
		}
		
		/* length of field */
		add_assoc_long(&row, "length", cur_f->db_flen);
		
		/* number of decimals in field */
		switch (cur_f->db_type) {
			case 'N':
			case 'I':
				add_assoc_long(&row, "precision", cur_f->db_fdc);
				break;
			default:
				add_assoc_long(&row, "precision", 0);
		}

		/* format for printing %s etc */
		add_assoc_string(&row, "format", cur_f->db_format);
		
		/* offset within record */
		add_assoc_long(&row, "offset", cur_f->db_foffset);
	}
}
/* }}} */

zend_module_entry dbase_module_entry = {
	STANDARD_MODULE_HEADER,
	"dbase", 
	dbase_functions, 
	PHP_MINIT(dbase), 
	NULL, 
	NULL, NULL, NULL, 
	PHP_DBASE_VERSION, 
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_DBASE
ZEND_GET_MODULE(dbase)

#if defined(PHP_WIN32) && defined(THREAD_SAFE)

/*NOTE: You should have an odbc.def file where you
export DllMain*/
BOOL WINAPI DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
    return 1;
}
#endif
#endif

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
