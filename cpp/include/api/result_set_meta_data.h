#pragma once
#include "struct/result_set_meta_data_c.h"
#include "struct/error_info.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// \brief Retrieves metadata information for the result set.
	/// \param results Pointer to the result set object.
	/// \param error Error information structure to populate on failure.
	/// \return Pointer to result set metadata object, nullptr on failure.
	ODBC_API CResultSetMetaData* get_meta_data(nanodbc::result* results, NativeError* error) noexcept;

	/// \brief Releases result set metadata resources.
	/// \param meta_data Pointer to CResultSetMetaData object to delete.
	ODBC_API void delete_meta_data(CResultSetMetaData* meta_data) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif
