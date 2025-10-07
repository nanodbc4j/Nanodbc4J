#include "struct/nanodbc_c.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace utils;

Driver::Attribute::Attribute(const Driver::Attribute& other) {
	LOG_TRACE("Copying Driver::Attribute from {}", (void*)&other);
	keyword = duplicate_string(other.keyword);
	value = duplicate_string(other.value);
}


Driver::Attribute::Attribute(const nanodbc::driver::attribute& other) {
	LOG_TRACE("Constructing Driver::Attribute from nanodbc::driver::attribute");
	keyword = duplicate_string(other.keyword.c_str(), other.keyword.length());
	value = duplicate_string(other.value.c_str(), other.value.length());
}

Driver::Attribute::~Attribute() {
	if (keyword) {
		free((void*) keyword);		
	}

	if (value){
		free((void*) value);		
	}
}

Driver::Driver(const Driver& other) {
	LOG_TRACE("Copying Driver from {}", (void*)&other);
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
	name = duplicate_string(other.name.c_str(), other.name.length());
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
		free((void*) name);		
	}

	if (attributes) {
		for (int i = 0; i < attribute_count; ++i) {
			delete attributes[i];
		}
		delete[] attributes;
	}
}

Datasource::Datasource(const Datasource& other) {
	LOG_TRACE("Copying Datasource from {}", (void*)&other);
	name = duplicate_string(other.name);
	driver = duplicate_string(other.driver);
}

Datasource::Datasource(const nanodbc::datasource& other) {
	LOG_TRACE("Constructing Datasource from nanodbc::datasource");
	name = duplicate_string(other.name.c_str(), other.name.length());
	driver = duplicate_string(other.driver.c_str(), other.driver.length());
}

Datasource::~Datasource() {
	if (name) {
		free((void*) name);
	}
	if (driver) {
		free((void*) driver);
	}
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