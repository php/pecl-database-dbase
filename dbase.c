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
#ifdef HAVE_SYS_FILE_H
# include <sys/file.h>
#endif
#include "ext/standard/flock_compat.h"
#include "Zend/zend_interfaces.h"

#include <stdlib.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if DBASE
#include "php_dbase.h"
#include "dbf.h"

#include <fcntl.h>
#include <errno.h>

/* compatitibility with PHP 8 */
#ifdef RETURN_THROWS
# define RETURN_THROWS_FALSE() RETURN_THROWS()
# define RETURN_THROWS_NULL()  RETURN_THROWS()
#else
# define RETURN_THROWS_FALSE() RETURN_FALSE
# define RETURN_THROWS_NULL()  return
#endif

#if PHP_VERSION_ID >= 80000
# define BAD_REC_NUMBER(i,n) zend_argument_value_error(i, "record number has to be in range 1..2147483647, but is " ZEND_LONG_FMT, n)
#else
# define BAD_REC_NUMBER(i,n) php_error_docref(NULL, E_WARNING, "record number has to be in range 1..2147483647, but is " ZEND_LONG_FMT, n)
#endif

#if PHP_VERSION_ID >= 80000
# include "dbase_arginfo.h"
#else
# include "dbase_7_arginfo.h"
#endif

typedef struct {
	dbhead_t *dbhead;
	zend_object std;
} dbase_dbase_handle;

static zend_class_entry *dbase_dbase_handle_ce;
static zend_object_handlers dbase_dbase_handle_object_handlers;

static zend_object *dbase_dbase_handle_create_object(zend_class_entry *class_type);
static void dbase_dbase_handle_free_object(zend_object *object);
static zend_function *dbase_dbase_handle_get_constructor(zend_object *object);

static const zend_function_entry dbase_dbase_handle_methods[] = {
	PHP_FE_END
};

PHP_MINIT_FUNCTION(dbase)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, "Dbase", "DbaseHandle", dbase_dbase_handle_methods);
	ce.ce_flags |= ZEND_ACC_FINAL;
	ce.create_object = dbase_dbase_handle_create_object;
	dbase_dbase_handle_ce = zend_register_internal_class(&ce);

	memcpy(&dbase_dbase_handle_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	dbase_dbase_handle_object_handlers.clone_obj = NULL;
	dbase_dbase_handle_object_handlers.free_obj = dbase_dbase_handle_free_object;
	dbase_dbase_handle_object_handlers.get_constructor = dbase_dbase_handle_get_constructor;
	dbase_dbase_handle_object_handlers.offset = XtOffsetOf(dbase_dbase_handle, std);

	REGISTER_STRING_CONSTANT("DBASE_VERSION", PHP_DBASE_VERSION, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("DBASE_TYPE_DBASE",  DBH_TYPE_NORMAL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DBASE_TYPE_FOXPRO", DBH_TYPE_FOXPRO, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("DBASE_RDONLY", O_RDONLY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("DBASE_RDWR",   O_RDWR,   CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}

static inline dbase_dbase_handle *dbase_dbase_handle_from_object(zend_object* obj)
{
	return (dbase_dbase_handle *)((char *)(obj) - XtOffsetOf(dbase_dbase_handle, std));
}

#define Z_DBASE_DBASEHANDLE_P(zv) dbase_dbase_handle_from_object(Z_OBJ_P(zv))

static zend_object *dbase_dbase_handle_create_object(zend_class_entry *class_type)
{
	dbase_dbase_handle *intern = emalloc(sizeof(dbase_dbase_handle) + zend_object_properties_size(class_type));
	memset(intern, 0, sizeof(dbase_dbase_handle) - sizeof(zend_object));

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);
	intern->std.handlers = &dbase_dbase_handle_object_handlers;

	return &intern->std;
};

static void dbase_dbase_handle_free_object(zend_object *object)
{
	dbase_dbase_handle *dbase_handle = dbase_dbase_handle_from_object(object);

	if (dbase_handle->dbhead) {
		php_flock(dbase_handle->dbhead->db_fd, LOCK_UN);
		close(dbase_handle->dbhead->db_fd);
		free_dbf_head(dbase_handle->dbhead);
		dbase_handle->dbhead = NULL;
	}

	zend_object_std_dtor(&dbase_handle->std);
};

static zend_function *dbase_dbase_handle_get_constructor(zend_object *object)
{
	zend_throw_error(NULL, "Cannot directly construct Dbase\\DbaseHandle, use dbase_create() or dbase_open() instead");
	return NULL;
}

/* {{{ proto Dbase\DbaseHandle dbase_open(string name, int mode)
   Opens a dBase-format database file */
PHP_FUNCTION(dbase_open)
{
	zend_string *dbf_name;
	zend_long mode;
	dbhead_t *dbh;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Pl", &dbf_name, &mode) == FAILURE) {	
		RETURN_THROWS_NULL();
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

	object_init_ex(return_value, dbase_dbase_handle_ce);
	Z_DBASE_DBASEHANDLE_P(return_value)->dbhead = dbh;
}
/* }}} */

/* {{{ proto bool dbase_close(Dbase\DbaseHandle identifier)
   Closes an open dBase-format database file */
PHP_FUNCTION(dbase_close)
{
	zval *dbh_id;
	dbase_dbase_handle *dbase_handle;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &dbh_id, dbase_dbase_handle_ce) == FAILURE) {
		RETURN_THROWS_NULL();
	}

	dbase_handle = Z_DBASE_DBASEHANDLE_P(dbh_id);
	if (dbase_handle->dbhead) {
		php_flock(dbase_handle->dbhead->db_fd, LOCK_UN);
		close(dbase_handle->dbhead->db_fd);
		free_dbf_head(dbase_handle->dbhead);
		dbase_handle->dbhead = NULL;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ proto int dbase_numrecords(Dbase\DbaseHandle identifier)
   Returns the number of records in the database */
PHP_FUNCTION(dbase_numrecords)
{
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &dbh_id, dbase_dbase_handle_ce) == FAILURE) {
		RETURN_THROWS_NULL();
	}

	dbht = Z_DBASE_DBASEHANDLE_P(dbh_id)->dbhead;

	RETURN_LONG(dbht->db_records);

}
/* }}} */

/* {{{ proto int dbase_numfields(Dbase\DbaseHandle identifier)
   Returns the number of fields (columns) in the database */
PHP_FUNCTION(dbase_numfields)
{
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &dbh_id, dbase_dbase_handle_ce) == FAILURE) {
		RETURN_THROWS_NULL();
	}

	dbht = Z_DBASE_DBASEHANDLE_P(dbh_id)->dbhead;

	RETURN_LONG(dbht->db_nfields - (dbht->db_nnullable ? 1 : 0));
}
/* }}} */

/* {{{ proto bool dbase_pack(Dbase\DbaseHandle identifier)
   Packs the database (deletes records marked for deletion) */
PHP_FUNCTION(dbase_pack)
{
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &dbh_id, dbase_dbase_handle_ce) == FAILURE) {
		RETURN_THROWS_NULL();
	}

	dbht = Z_DBASE_DBASEHANDLE_P(dbh_id)->dbhead;

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
	char nullable_flags[DBH_MAX_FIELDS / 8];

	if (replace) {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "Ohl", &dbh_id, dbase_dbase_handle_ce, &fields, &recnum) == FAILURE) {
			RETURN_THROWS_NULL();
		}
		if (recnum < 1 || recnum > 0x7FFFFFFF) {
			BAD_REC_NUMBER(3, recnum);
			RETURN_THROWS_FALSE();
		}
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "Oh", &dbh_id, dbase_dbase_handle_ce, &fields) == FAILURE) {	
			RETURN_THROWS_NULL();
		}
	}

	dbht = Z_DBASE_DBASEHANDLE_P(dbh_id)->dbhead;

	memset(nullable_flags, 0, sizeof(nullable_flags));

	num_fields = zend_hash_num_elements(fields);

	if (num_fields != dbht->db_nfields - (dbht->db_nnullable ? 1 : 0)) {
		php_error_docref(NULL, E_WARNING, "expected %d fields, but got %d", dbht->db_nfields, num_fields);
		RETURN_FALSE;
	}

	cp = t_cp = (char *)emalloc(dbht->db_rlen + 1);
	*t_cp++ = VALID_RECORD;

	dbf = dbht->db_fields;
	for (i = 0, cur_f = dbf; cur_f < &dbf[num_fields]; i++, cur_f++) {
		zend_string *field_val;

		if ((field = zend_hash_index_find(fields, i)) == NULL) {
			php_error_docref(NULL, E_WARNING, "expected plain indexed array");
			efree(cp);
			RETURN_FALSE;
		}

		if (Z_TYPE_P(field) == IS_NULL && cur_f->db_fnullable >= 0) {
			nullable_flags[cur_f->db_fnullable / 8] |= (1 << (cur_f->db_fnullable % 8));
		}

		if (Z_TYPE_P(field) == IS_DOUBLE && (cur_f->db_type == 'N' || cur_f->db_type == 'F')) {
			/* convert to string as if in C locale */
			field_val = _php_math_number_format_ex(Z_DVAL_P(field), cur_f->db_fdc, ".", 1, "", 0);
		} else {
			field_val = zval_get_string(field);
			if (EG(exception)) {
				zend_string_release(field_val);
				efree(cp);
				RETURN_FALSE;
			}
		}

		switch (cur_f->db_type) {
			case 'T':
				{
					int jdn, msecs;

					db_get_timestamp(ZSTR_VAL(field_val), &jdn, &msecs);
					put_long(t_cp, jdn);
					put_long(t_cp + 4, msecs);
				}
				break;
			default:
				snprintf(t_cp, cur_f->db_flen+1, cur_f->db_format, ZSTR_VAL(field_val));
		}

		zend_string_release(field_val);
		t_cp += cur_f->db_flen;
	}

	if (dbht->db_nnullable > 0) {
		memcpy(t_cp, nullable_flags, (dbht->db_nnullable - 1) / 8 + 1);
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

/* {{{ proto bool dbase_add_record(Dbase\DbaseHandle identifier, array data)
   Adds a record to the database */
PHP_FUNCTION(dbase_add_record)
{
	php_dbase_put_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ proto bool dbase_replace_record(Dbase\DbaseHandle identifier, array data, int recnum)
   Replaces a record to the database */
PHP_FUNCTION(dbase_replace_record)
{
	php_dbase_put_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto bool dbase_delete_record(Dbase\DbaseHandle identifier, int record)
   Marks a record to be deleted */
PHP_FUNCTION(dbase_delete_record)
{
	zend_long record;
	zval *dbh_id;
	dbhead_t *dbht;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Ol", &dbh_id, dbase_dbase_handle_ce, &record) == FAILURE) {
		RETURN_THROWS_NULL();
	}

	dbht = Z_DBASE_DBASEHANDLE_P(dbh_id)->dbhead;

	if (record < 1 || record > 0x7FFFFFFF) {
		BAD_REC_NUMBER(2, record);
		RETURN_THROWS_FALSE();
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
	char nullable_flags[DBH_MAX_FIELDS / 8];

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Ol", &dbh_id, dbase_dbase_handle_ce, &record) == FAILURE) {
		RETURN_THROWS_NULL();
	}

	dbht = Z_DBASE_DBASEHANDLE_P(dbh_id)->dbhead;

	if (record < 1 || record > 0x7FFFFFFF) {
		BAD_REC_NUMBER(2, record);
		RETURN_THROWS_FALSE();
	}

	if ((data = get_dbf_record(dbht, (long) record)) == NULL) {
		php_error_docref(NULL, E_WARNING, "Tried to read bad record %ld", record);
		RETURN_FALSE;
	}

	dbf = dbht->db_fields;

	if (dbht->db_nnullable > 0) {
		memset(nullable_flags, 0, sizeof(nullable_flags));
		cur_f = &dbf[dbht->db_nfields - 1];
		get_binary_field_val(data, cur_f, nullable_flags);
	}

	array_init(return_value);

	fnp = NULL;
	for (cur_f = dbf; cur_f < &dbf[dbht->db_nfields - (dbht->db_nnullable ? 1 : 0)]; cur_f++) {
		if (cur_f->db_fnullable >= 0 && (nullable_flags[cur_f->db_fnullable / 8] & (1 << (cur_f->db_fnullable % 8)))) {
			if (!assoc) {
				add_next_index_null(return_value);
			} else {
				add_assoc_null(return_value, cur_f->db_fname);
			}
			continue;
		}

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
 
/* {{{ proto array dbase_get_record(Dbase\DbaseHandle identifier, int record)
   Returns an array representing a record from the database */
PHP_FUNCTION(dbase_get_record)
{
	php_dbase_get_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* From Martin Kuba <makub@aida.inet.cz> */
/* {{{ proto array dbase_get_record_with_names(Dbase\DbaseHandle identifier, int record)
   Returns an associative array representing a record from the database */
PHP_FUNCTION(dbase_get_record_with_names)
{
	php_dbase_get_record(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */


static dbhead_t *create_head_from_spec(HashTable *fields, int fd, unsigned char type)
{
	int num_fields;
	dbfield_t *dbf, *cur_f;
	int i, rlen;
	zval *field, *value;
	dbhead_t *dbh;
	int nullable_bit = 0;

	num_fields = zend_hash_num_elements(fields);

	if (num_fields <= 0) {
		php_error_docref(NULL, E_WARNING, "Unable to create database without fields");
		return NULL;
	}

	dbh = (dbhead_t *)emalloc(sizeof(dbhead_t));
	dbf = (dbfield_t *)emalloc(sizeof(dbfield_t) * (num_fields + 1));
	
	/* initialize the header structure */
	dbh->db_fields = dbf;
	dbh->db_fd = fd;
	dbh->db_dbt = type;
	strcpy(dbh->db_date, "19930818");
	dbh->db_records = 0;
	dbh->db_nfields = num_fields;
	dbh->db_hlen = sizeof(struct dbf_dhead) + 1 + num_fields * sizeof(struct dbf_dfield);
	if (type == DBH_TYPE_FOXPRO) {
		dbh->db_hlen += 263;
	}

	rlen = 1;
	/**
	 * Patch by greg@darkphoton.com
	 **/
	/* make sure that the db_format entries for all fields are set to NULL to ensure we
       don't seg fault if there's an error and we need to call free_dbf_head() before all
       fields have been defined. */
	for (i = 0, cur_f = dbf; i < num_fields + 1; i++, cur_f++) {
		cur_f->db_format = NULL;
	}
	/**
	 * end patch
	 */


	for (i = 0, cur_f = dbf; i < num_fields; i++, cur_f++) {
		int element = 0;
		zval tmp_value;

		/* look up the field */
		if ((field = zend_hash_index_find(fields, i)) == NULL) {
			php_error_docref(NULL, E_WARNING, "expected plain indexed array");
			goto fail;
		}

		/* field name */
		
		if (Z_TYPE_P(field) != IS_ARRAY || (value = zend_hash_index_find(Z_ARRVAL_P(field), element)) == NULL) {
			php_error_docref(NULL, E_WARNING, "expected field name as element %d of list in field %d", element, i);
			goto fail;
		}
		ZVAL_COPY_VALUE(&tmp_value, value);
		zval_copy_ctor(&tmp_value);
		convert_to_string(&tmp_value);
		if (EG(exception)) {
			zval_dtor(&tmp_value);
			goto fail;
		}
		if (Z_STRLEN(tmp_value) > 10 || Z_STRLEN(tmp_value) == 0) {
			php_error_docref(NULL, E_WARNING, "invalid field name '%s' (must be non-empty and less than or equal to 10 characters)", Z_STRVAL(tmp_value));
			zval_dtor(&tmp_value);
			goto fail;
		}
		copy_crimp(cur_f->db_fname, Z_STRVAL(tmp_value), (int) Z_STRLEN(tmp_value));
		zval_dtor(&tmp_value);

		/* field type */
		if ((value = zend_hash_index_find(Z_ARRVAL_P(field), ++element)) == NULL) {
			php_error_docref(NULL, E_WARNING, "expected field type as element %d of list in field %d", element, i);
			goto fail;
		}
		ZVAL_COPY_VALUE(&tmp_value, value);
		zval_copy_ctor(&tmp_value);
		convert_to_string(&tmp_value);
		if (EG(exception)) {
			zval_dtor(&tmp_value);
			goto fail;
		}
		cur_f->db_type = toupper(*Z_STRVAL(tmp_value));
		zval_dtor(&tmp_value);

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
			cur_f->db_flen = 8;
			break;
		case 'T':
			if (type != DBH_TYPE_FOXPRO) {
				php_error_docref(NULL, E_WARNING, "datetime fields are not supported by dBASE");
				goto fail;
			}
			cur_f->db_flen = 8;
			break;
		case 'F':
		case 'N':
		case 'C':
			/* field length */
			if ((value = zend_hash_index_find(Z_ARRVAL_P(field), ++element)) == NULL) {
				php_error_docref(NULL, E_WARNING, "expected field length as element %d of list in field %d", element, i);
				goto fail;
			}
			ZVAL_COPY_VALUE(&tmp_value, value);
			zval_copy_ctor(&tmp_value);
			convert_to_long(&tmp_value);
			if (Z_LVAL(tmp_value) < 0 || Z_LVAL(tmp_value) > 254) {
				php_error_docref(NULL, E_WARNING, "expected length of field %d to be in range 0..254, but got " ZEND_LONG_FMT, i, Z_LVAL(tmp_value));
				zval_dtor(&tmp_value);
				goto fail;
			}
			cur_f->db_flen = (unsigned char) Z_LVAL(tmp_value);
			zval_dtor(&tmp_value);

			if (cur_f->db_type == 'N' || cur_f->db_type == 'F') {
				if ((value = zend_hash_index_find(Z_ARRVAL_P(field), ++element)) == NULL) {
					php_error_docref(NULL, E_WARNING, "expected field precision as element %d of list in field %d", element, i);
					goto fail;
				}
				ZVAL_COPY_VALUE(&tmp_value, value);
				zval_copy_ctor(&tmp_value);
				convert_to_long(&tmp_value);
				if (Z_LVAL(tmp_value) < 0 || Z_LVAL(tmp_value) > 254) {
					php_error_docref(NULL, E_WARNING, "expected precision of field %d to be in range 0..254, but got " ZEND_LONG_FMT, i, Z_LVAL(tmp_value));
					zval_dtor(&tmp_value);
					goto fail;
				}
				cur_f->db_fdc = (unsigned char) Z_LVAL(tmp_value);
				zval_dtor(&tmp_value);
			}
			break;
		default:
			php_error_docref(NULL, E_WARNING, "unknown field type '%c'", cur_f->db_type);
			goto fail;
		}
		cur_f->db_foffset = rlen;
		rlen += cur_f->db_flen;
	
		cur_f->db_format = get_dbf_f_fmt(cur_f);

		cur_f->db_fnullable = -1;
		if (type == DBH_TYPE_FOXPRO && (value = zend_hash_index_find(Z_ARRVAL_P(field), ++element)) != NULL) {
			ZVAL_COPY_VALUE(&tmp_value, value);
			zval_copy_ctor(&tmp_value);
			convert_to_boolean(&tmp_value);
			if (Z_TYPE(tmp_value) == IS_TRUE) {
				cur_f->db_fnullable = nullable_bit++;
			}
			zval_dtor(&tmp_value);
		}
	}

	if (nullable_bit) {
		dbh->db_nfields++;
		dbh->db_hlen += sizeof(struct dbf_dfield);
		copy_crimp(cur_f->db_fname, "_NullFlags", sizeof("_NullFlags") - 1);
		cur_f->db_type = '0';
		cur_f->db_flen = ((nullable_bit - 1)) / 8 + 1;
		cur_f->db_fdc = 0;
		cur_f->db_format = estrdup("");
		cur_f->db_foffset = rlen;
		cur_f->db_fnullable = -1;
		rlen += cur_f->db_flen;
	}

	dbh->db_nnullable = nullable_bit;
	dbh->db_rlen = rlen;

	return dbh;

fail:
	free_dbf_head(dbh);
	return NULL;
}


/* {{{ proto Dbase\DbaseHandle dbase_create(string filename, array fields [, int type])
   Creates a new dBase-format database file */
PHP_FUNCTION(dbase_create)
{
	zend_string *filename;
	HashTable *fields;
	zend_long type = DBH_TYPE_NORMAL;
	int fd;
	dbhead_t *dbh;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Ph|l", &filename, &fields, &type) == FAILURE) {
		RETURN_THROWS_NULL();
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
		goto fail;
	}

	if (type != DBH_TYPE_NORMAL && type != DBH_TYPE_FOXPRO) {
		php_error_docref(NULL, E_WARNING, "unknown database type " ZEND_LONG_FMT, type);
		goto fail;
	}

	if ((dbh = create_head_from_spec(fields, fd, (unsigned char) type)) == NULL) {
		goto fail;
	}

	if (put_dbf_info(dbh) != 1 || put_dbf_eof_marker(dbh)) {
		free_dbf_head(dbh);
		goto fail;
	}

	object_init_ex(return_value, dbase_dbase_handle_ce);
	Z_DBASE_DBASEHANDLE_P(return_value)->dbhead = dbh;
	return;

fail:
	close(fd);
	RETURN_FALSE;
}
/* }}} */


/* Added by Zak Greant <zak@php.net> */
/* {{{ proto array dbase_get_header_info(Dbase\DbaseHandle database_handle)
 */
PHP_FUNCTION(dbase_get_header_info)
{
	zval		row;
	dbhead_t 	*dbht;
	zval		*dbh_id;
	dbfield_t	*dbf, *cur_f;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &dbh_id, dbase_dbase_handle_ce) == FAILURE) {
		RETURN_THROWS_NULL();
	}

	dbht = Z_DBASE_DBASEHANDLE_P(dbh_id)->dbhead;

	array_init(return_value);

	dbf = dbht->db_fields;
	for (cur_f = dbf; cur_f < &dbht->db_fields[dbht->db_nfields]; ++cur_f) {
		if (cur_f->db_type == '0') {
			continue;
		}
		
		array_init(&row);
		
		add_next_index_zval(return_value, &row);
		
		/* field name */
		add_assoc_string(&row, "name", cur_f->db_fname);
		
		/* field type */
		switch (cur_f->db_type) {
			case 'C': add_assoc_string(&row, "type", "character"); break;
			case 'D': add_assoc_string(&row, "type", "date");      break;
			case 'T': add_assoc_string(&row, "type", "datetime");  break;
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
			case 'F':
			case 'N':
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
	ext_functions, 
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
