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

const datasource** converter::convert(const vector<nanodbc::datasource>& datasources) {
    // Выделяем массив указателей на datasource
    const datasource** result = static_cast<const datasource**>(malloc(sizeof(datasource*) * datasources.size()));
    if (!result) return nullptr;

    for (size_t i = 0; i < datasources.size(); ++i) {
        datasource* item = convert(&datasources[i]);

        if (!item) {
            // Освобождаем уже выделенную память в случае ошибки
            for (int j = 0; j < i; ++j) {
                free((void*)result[j]->name);
                free((void*)result[j]->driver);
                free((void*)result[j]);
            }
            free(result);
            return nullptr;
        }

        result[i] = item;
    }

    return result;
}

driver::attribute* converter::convert(const nanodbc::driver::attribute* attr) {
    auto item = static_cast<driver::attribute*>(malloc(sizeof(driver::attribute)));
    if (!item) return nullptr;

    auto keyword = utils::to_u16string(attr->keyword);
    item->keyword = duplicate_string(keyword.c_str());

    auto value = utils::to_u16string(attr->value);
    item->value = duplicate_string(value.c_str());

    return item;
}

driver* converter::convert(const nanodbc::driver* _driver) {
    auto* item = static_cast<driver*>(malloc(sizeof(driver)));
    if (!item) return nullptr;

    auto name = utils::to_u16string(_driver->name);
    item->name = duplicate_string(name.c_str());

    vector<nanodbc::driver::attribute> attributes(_driver->attributes.begin(), _driver->attributes.end());

    // Выделяем массив указателей на driver::attribute
    item->attributes = static_cast<const driver::attribute**>(malloc(sizeof(driver::attribute*) * attributes.size()));
    if (!item->attributes) {
        free(item);
        return nullptr;
    }

    for (size_t i = 0; i < attributes.size(); ++i) {
        auto* attr_item = convert(&attributes[i]);
        if (!attr_item) {
            // Освобождаем уже выделенную память в случае ошибки
            for (int j = 0; j < i; ++j) {
                free((void*)item->attributes[j]->keyword);
                free((void*)item->attributes[j]->value);
                free((void*)item->attributes[j]);
            }
            free(item);
            return nullptr;
        }

        item->attributes[i] = attr_item;
        ++item->attribute_count;
    }

    return item;
}