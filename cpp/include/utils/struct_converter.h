#pragma once
#include <vector>
#include <nanodbc/nanodbc.h>
#include "struct/nanodbc_c.h"

namespace converter {
	Datasource* convert(const nanodbc::datasource* data);

	const Datasource** convert(const std::list<nanodbc::datasource>& datasources);

	Driver::Attribute* convert(const nanodbc::driver::attribute* attr);

	Driver* convert(const nanodbc::driver* _driver);

	const Driver** convert(const std::list<nanodbc::driver>& drivers);

}