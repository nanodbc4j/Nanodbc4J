#include "utils/struct_converter.hpp"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace std;
using namespace utils;

const Datasource** converter::convert(const std::list<nanodbc::datasource>& datasources) {
    LOG_DEBUG("Converting {} nanodbc::datasource entries", datasources.size());
    if (datasources.empty()) {
        LOG_DEBUG("Input datasources list is empty, returning nullptr array");
    }

    const Datasource** result = new const Datasource* [datasources.size()];

    size_t i = 0;
    for (const auto& source : datasources) {
        result[i++] = new Datasource(source);
    }

    LOG_DEBUG("Conversion of datasources completed, returned array of {} pointers", datasources.size());
    return result;
}

const Driver** converter::convert(const list<nanodbc::driver>& drivers) {
    LOG_DEBUG("Converting {} nanodbc::driver entries", drivers.size());
    if (drivers.empty()) {
        LOG_DEBUG("Input drivers list is empty, returning nullptr array");
    }

    const Driver** result = new const Driver* [drivers.size()];

    size_t i = 0;
    for (const auto& drv : drivers) {
        result[i++] = new Driver(drv);
    }

    LOG_DEBUG("Conversion of drivers completed, returned array of {} pointers", drivers.size());
    return result;
}