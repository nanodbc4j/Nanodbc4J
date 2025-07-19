#include "utils/struct_converter.h"
#include "utils/string_utils.h"

using namespace std;
using namespace utils;

datasource* converter::convert(const nanodbc::datasource* data) {
    datasource* item = static_cast<datasource*>(malloc(sizeof(datasource)));
    if (!item) return nullptr;

    // Копируем name
    auto name = utils::to_u16string(data->name);
    item->name = duplicate_string(name.c_str());

    // Копируем driver
    auto driver = utils::to_u16string(data->driver);
    item->driver = duplicate_string(driver.c_str());

    return item;
}

attribute* converter::convert(const nanodbc::driver::attribute* attr) {
    attribute* item = static_cast<attribute*>(malloc(sizeof(attribute)));
    if (!item) return nullptr;

    auto keyword = utils::to_u16string(attr->keyword);
    item->keyword = duplicate_string(keyword.c_str());

    auto value = utils::to_u16string(attr->value);
    item->value = duplicate_string(value.c_str());

    return item;
}