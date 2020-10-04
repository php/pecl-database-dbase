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

ZEND_BEGIN_ARG_INFO_EX(arginfo_dbase_create, 0, 0, 2)
	ZEND_ARG_INFO(0, filename)
	ZEND_ARG_ARRAY_INFO(0, fields, 0)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dbase_get_header_info, 0)
	ZEND_ARG_INFO(0, database_handle)
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


const zend_function_entry ext_functions[] = {
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
	PHP_FE_END
};
