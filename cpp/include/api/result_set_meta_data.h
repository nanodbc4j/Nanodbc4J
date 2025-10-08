#pragma once
#include "struct/result_set_meta_data_c.h"
#include "struct/error_info.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

	ODBC_API CResultSetMetaData* get_meta_data(nanodbc::result* results, NativeError* error);

	ODBC_API void delete_meta_data(CResultSetMetaData* meta_data);

#ifdef __cplusplus
} // extern "C"
#endif
