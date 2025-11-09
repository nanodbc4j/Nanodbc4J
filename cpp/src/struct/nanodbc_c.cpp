#include "struct/nanodbc_c.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace utils;

static const wchar_t* convert_string(const nanodbc::string& str) {
	LOG_TRACE("Converting string to char array: '{}'", to_string(str));
	const auto wstr = to_wstring(str);
	const wchar_t* result = duplicate_string(wstr.c_str(), wstr.length());
	LOG_TRACE("Converted string duplicated at {}", reinterpret_cast<uintptr_t>(result));
	return result;
}

Driver::Attribute::Attribute(const Attribute& other) {
	LOG_TRACE("Copying Driver::Attribute from {}", reinterpret_cast<uintptr_t>(&other));
	keyword = duplicate_string(other.keyword);
	value = duplicate_string(other.value);
}


Driver::Attribute::Attribute(const nanodbc::driver::attribute& other) {
	LOG_TRACE("Constructing Driver::Attribute from nanodbc::driver::attribute");
	keyword = convert_string(other.keyword);
	value = convert_string(other.value);
}

Driver::Attribute::~Attribute() {
	if (keyword) {
		free(const_cast<void*>(static_cast<const void*>(keyword)));
	}

	if (value){
		free(const_cast<void*>(static_cast<const void*>(value)));
	}
}

Driver::Driver(const Driver& other) {
	LOG_TRACE("Copying Driver from {}", reinterpret_cast<uintptr_t>(&other));
	attribute_count = static_cast<int>(other.attribute_count);
	name = duplicate_string(other.name);
	attributes = nullptr;
	
	if (attribute_count > 0) {
		attributes = new const Attribute* [attribute_count];
		for (int i = 0; i < attribute_count; ++i) {
			attributes[i] = new Attribute(*other.attributes[i]);
		}
	}
}

Driver::Driver(const nanodbc::driver& other) {
	LOG_TRACE("Constructing Driver from nanodbc::driver");
	attribute_count = static_cast<int>(other.attributes.size());
	name = convert_string(other.name);
	attributes = nullptr;

	if (attribute_count > 0) {		
		attributes = new const Attribute* [other.attributes.size()];
		size_t i = 0;
		for (auto& attr : other.attributes) {
			// The check is now before the access
			if (i >= other.attributes.size()) {
				break; // Protection against out-of-bounds (should never happen)
			}
			attributes[i++] = new Attribute(attr);
		}
	}
}

Driver::~Driver() {
	if (name) {
		free(const_cast<void*>(static_cast<const void*>(name)));
	}

	if (attributes) {
		for (int i = 0; i < attribute_count; ++i) {
			delete attributes[i];
		}
		delete[] attributes;
	}
}

Driver** Driver::convert(const std::list<nanodbc::driver>& drivers) {
	LOG_DEBUG("Converting {} nanodbc::driver entries", drivers.size());
	if (drivers.empty()) {
		LOG_DEBUG("Input drivers list is empty, returning nullptr array");
	}

	auto** result = new Driver* [drivers.size()];

	size_t i = 0;
	for (const auto& drv : drivers) {
		result[i++] = new Driver(drv);
	}

	LOG_DEBUG("Conversion of drivers completed, returned array of {} pointers", drivers.size());
	return result;
}

Datasource::Datasource(const Datasource& other) {
	LOG_TRACE("Copying Datasource from {}", reinterpret_cast<uintptr_t>(&other));
	name = duplicate_string(other.name);
	driver = duplicate_string(other.driver);
}

Datasource::Datasource(const nanodbc::datasource& other) {
	LOG_TRACE("Constructing Datasource from nanodbc::datasource");
	name = convert_string(other.name);
	driver = convert_string(other.driver);
}

Datasource::~Datasource() {
	if (name) {
		free(const_cast<void*>(static_cast<const void*>(name)));
	}
	if (driver) {
		free(const_cast<void*>(static_cast<const void*>(driver)));
	}
}

Datasource** Datasource::convert(const std::list<nanodbc::datasource>& datasources) {
	LOG_DEBUG("Converting {} nanodbc::datasource entries", datasources.size());
	if (datasources.empty()) {
		LOG_DEBUG("Input datasources list is empty, returning nullptr array");
	}

	auto** result = new Datasource* [datasources.size()];

	size_t i = 0;
	for (const auto& source : datasources) {
		result[i++] = new Datasource(source);
	}

	LOG_DEBUG("Conversion of datasources completed, returned array of {} pointers", datasources.size());
	return result;
}

CDate::CDate(int16_t y, int16_t m, int16_t d)
	: year(y), month(m), day(d) {
}

CDate::CDate(const CDate& other) {
	LOG_TRACE("Copying CDate: {}/{}/{}", other.day, other.month, other.year);
	day = other.day;
	month = other.month;
	year = other.year;
}

CDate::CDate(const nanodbc::date& other) {
	LOG_TRACE("Constructing CDate from nanodbc::date: {}/{}/{}", other.day, other.month, other.year);
	day = other.day;
	month = other.month;
	year = other.year;
}

CTime::CTime(int16_t h, int16_t m, int16_t s)
	: hour(h), minute(m), second(s) {
}

CTime::CTime(const CTime& other) {
	LOG_TRACE("Copying CTime: {}:{}:{}", other.hour, other.minute, other.second);
	hour = other.hour;
	minute = other.minute;
	second = other.second;
}

CTime::CTime(const nanodbc::time& other) {
	LOG_TRACE("Constructing CTime from nanodbc::time: {}:{}:{}", other.hour, other.min, other.sec);
	hour = other.hour;
	minute = other.min;
	second = other.sec;
}

CTimestamp::CTimestamp(int16_t y, int16_t m, int16_t d, int16_t h, int16_t min, int16_t s, int32_t f)
	: year(y), month(m), day(d), hour(h), minute(min), second(s), fract(f) {
}

CTimestamp::CTimestamp(const CTimestamp& other) {
	LOG_TRACE("Copying CTimestamp: {}-{}-{} {}:{}:{}.{:09}",
		other.year, other.month, other.day,
		other.hour, other.minute, other.second, other.fract);

	year = other.year;
	month = other.month;
	day = other.day;
	hour = other.hour;
	minute = other.minute;
	second = other.second;
	fract = other.fract;
}

CTimestamp::CTimestamp(const nanodbc::timestamp& other) {
	LOG_TRACE("Constructing CTimestamp from nanodbc::timestamp: {}-{}-{} {}:{}:{}.{:09}",
		other.year, other.month, other.day,
		other.hour, other.min, other.sec, other.fract);

	year = other.year;
	month = other.month;
	day = other.day;
	hour = other.hour;
	minute = other.min;
	second = other.sec;
	fract = other.fract;
}