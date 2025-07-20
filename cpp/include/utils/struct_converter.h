#pragma once
#include <vector>
#include <nanodbc/nanodbc.h>
#include "nanodbc_c.h"

namespace converter {
	datasource* convert(const nanodbc::datasource* data);

	const datasource** convert(const std::vector<nanodbc::datasource>& datasources);

	driver::attribute* convert(const nanodbc::driver::attribute* attr);

	driver* convert(const nanodbc::driver* _driver);
}