#include "utils/struct_converter.h"
#include "utils/string_utils.h"

using namespace std;
using namespace utils;

const Datasource** converter::convert(const std::list<nanodbc::datasource>& datasources) {
    const Datasource** result = new const Datasource* [datasources.size()];

    size_t i = 0;
    for (const auto& source : datasources) {
        result[i++] = new Datasource(source);
    }

    return result;
}

const Driver** converter::convert(const list<nanodbc::driver>& drivers) {
    const Driver** result = new const Driver* [drivers.size()];

    size_t i = 0;
    for (const auto& drv : drivers) {
        result[i++] = new Driver(drv);
    }

    return result;
}