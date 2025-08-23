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
	auto _keyword = to_u16string(other.keyword);
	keyword = duplicate_string(_keyword.c_str());

	auto _value = to_u16string(other.value);
	value = duplicate_string(_value.c_str());
}

Driver::Attribute::~Attribute() {
	if (keyword) {
		free((void*) keyword);		
	}

	if (value){
		free((void*) value);		
	}

	keyword = nullptr;
	value = nullptr;
}

Driver::Driver(const Driver& other) {
	LOG_TRACE("Copying Driver from {}", (void*)&other);
	attribute_count = static_cast<int>(other.attribute_count);
	auto _name = to_u16string(other.name);
	name = duplicate_string(_name.c_str());
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
	auto _name = to_u16string(other.name);
	name = duplicate_string(_name.c_str());
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

	name = nullptr;
	attributes = nullptr;
	attribute_count = 0;
}

Datasource::Datasource(const Datasource& other) {
	LOG_TRACE("Copying Datasource from {}", (void*)&other);
	auto _name = to_u16string(other.name);
	name = duplicate_string(_name.c_str());	

	auto _driver = to_u16string(other.driver);
	driver = duplicate_string(_driver.c_str());
}

Datasource::Datasource(const nanodbc::datasource& other) {
	LOG_TRACE("Constructing Datasource from nanodbc::datasource");
	auto _name = to_u16string(other.name);
	name = duplicate_string(_name.c_str());

	auto _driver = to_u16string(other.driver);
	driver = duplicate_string(_driver.c_str());
}

Datasource::~Datasource() {
	if (name) {
		free((void*) name);
	}
	if (driver) {
		free((void*) driver);
	}

	name = nullptr;
	driver = nullptr;
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