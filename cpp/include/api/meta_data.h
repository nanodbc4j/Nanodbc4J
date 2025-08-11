#pragma once
#include "struct/meta_data_c.h"
#include "struct/error_info.h"
#include "api/export.h"

#ifdef __cplusplus
extern "C" {
#endif

	ODBC_API MetaData* get_meta_data(nanodbc::result* results, NativeError* error);

	ODBC_API void delete_meta_data(MetaData* meta_data);

#ifdef __cplusplus
} // extern "C"
#endif
