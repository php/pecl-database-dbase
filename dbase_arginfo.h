/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: aa034911aa492488ef0fc614dba8fd40ba6da7a0 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_dbase_open, 0, 2, Dbase\\DbaseHandle, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dbase_close, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, database, Dbase\\DbaseHandle, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dbase_add_record, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, database, Dbase\\DbaseHandle, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dbase_delete_record, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, database, Dbase\\DbaseHandle, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dbase_replace_record, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, database, Dbase\\DbaseHandle, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dbase_numrecords, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, database, Dbase\\DbaseHandle, 0)
ZEND_END_ARG_INFO()

#define arginfo_dbase_numfields arginfo_dbase_numrecords

#define arginfo_dbase_pack arginfo_dbase_close

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_dbase_get_record, 0, 2, MAY_BE_ARRAY|MAY_BE_FALSE)
	ZEND_ARG_OBJ_INFO(0, database, Dbase\\DbaseHandle, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_dbase_get_record_with_names arginfo_dbase_get_record

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_dbase_create, 0, 2, Dbase\\DbaseHandle, MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, fields, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, type, IS_LONG, 0, "DBASE_TYPE_DBASE")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_dbase_get_header_info, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_OBJ_INFO(0, database, Dbase\\DbaseHandle, 0)
ZEND_END_ARG_INFO()


ZEND_FUNCTION(dbase_open);
ZEND_FUNCTION(dbase_close);
ZEND_FUNCTION(dbase_add_record);
ZEND_FUNCTION(dbase_delete_record);
ZEND_FUNCTION(dbase_replace_record);
ZEND_FUNCTION(dbase_numrecords);
ZEND_FUNCTION(dbase_numfields);
ZEND_FUNCTION(dbase_pack);
ZEND_FUNCTION(dbase_get_record);
ZEND_FUNCTION(dbase_get_record_with_names);
ZEND_FUNCTION(dbase_create);
ZEND_FUNCTION(dbase_get_header_info);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(dbase_open, arginfo_dbase_open)
	ZEND_FE(dbase_close, arginfo_dbase_close)
	ZEND_FE(dbase_add_record, arginfo_dbase_add_record)
	ZEND_FE(dbase_delete_record, arginfo_dbase_delete_record)
	ZEND_FE(dbase_replace_record, arginfo_dbase_replace_record)
	ZEND_FE(dbase_numrecords, arginfo_dbase_numrecords)
	ZEND_FE(dbase_numfields, arginfo_dbase_numfields)
	ZEND_FE(dbase_pack, arginfo_dbase_pack)
	ZEND_FE(dbase_get_record, arginfo_dbase_get_record)
	ZEND_FE(dbase_get_record_with_names, arginfo_dbase_get_record_with_names)
	ZEND_FE(dbase_create, arginfo_dbase_create)
	ZEND_FE(dbase_get_header_info, arginfo_dbase_get_header_info)
	ZEND_FE_END
};


static const zend_function_entry class_Dbase_DbaseHandle_methods[] = {
	ZEND_FE_END
};
