#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

    /// \brief Driver attributes.
    struct attribute {
        const char16_t* keyword; ///< Driver keyword attribute.
        const char16_t* value;   ///< Driver attribute value.
    };

    struct driver {
        const char16_t* name;            ///< Driver name.
        attribute* attributes; ///< Driver attributes.
        int attribute_count;
    };

    struct datasource {
        const char16_t* name;   ///< DSN name.
        const char16_t* driver; ///< Driver description.
    };

    struct c_date {
        int16_t year;  ///< Year [0-inf).
        int16_t month; ///< Month of the year [1-12].
        int16_t day;   ///< Day of the month [1-31].
    };

    struct c_time {
        int16_t hour;    ///< Hours since midnight [0-23].
        int16_t minute;  ///< Minutes after the hour [0-59].
        int16_t second;  ///< Seconds after the minute.
    };

    struct c_timestamp {
        int16_t year;     ///< Year [0-inf).
        int16_t month;    ///< Month of the year [1-12].
        int16_t day;      ///< Day of the month [1-31].
        int16_t hour;     ///< Hours since midnight [0-23].
        int16_t minute;   ///< Minutes after the hour [0-59].
        int16_t second;   ///< Seconds after the minute.
        int32_t fract;    ///< Fractional seconds.
    };

#ifdef __cplusplus
} // extern "C"
#endif