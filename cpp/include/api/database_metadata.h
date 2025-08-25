#pragma once
#include "struct/database_metadata_c.h"
#include "struct/error_info.h"
#include "api/export.h"

#ifdef __cplusplus
extern "C" {
#endif

	ODBC_API CDatabaseMetaData* get_database_meta_data(nanodbc::connection* conn, NativeError* error);

	ODBC_API void delete_database_meta_data(CDatabaseMetaData* meta_data);

#ifdef __cplusplus
} // extern "C"
#endif
