#pragma once
#include <nanodbc/nanodbc.h>
#include "nanodbc_c.h"

namespace converter {
	datasource* convert(const nanodbc::datasource* data);

	attribute* convert(const nanodbc::driver::attribute* attr);
}