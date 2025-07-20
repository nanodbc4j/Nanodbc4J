#pragma once
#include <cstdint>
#include <nanodbc/nanodbc.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct Driver {
        /// \brief Driver attributes.
        struct Attribute {
            const char16_t* keyword; ///< Driver keyword attribute.
            const char16_t* value;   ///< Driver attribute value.

            Attribute();
            Attribute(const Attribute& other);
            Attribute(const nanodbc::driver::attribute& other);
            ~Attribute();
        };

        const char16_t* name;            ///< Driver name.
        const Attribute** attributes; ///< Driver attributes.
        int attribute_count;

        Driver();
        Driver(const Driver& other);
        Driver(const nanodbc::driver& other);
        ~Driver();
    };

    struct Datasource {
        const char16_t* name;   ///< DSN name.
        const char16_t* driver; ///< Driver description.

        Datasource();
        Datasource(const Datasource& other);
        Datasource(const nanodbc::datasource& other);
        ~Datasource();
    };

    struct CDate {
        int16_t year;  ///< Year [0-inf).
        int16_t month; ///< Month of the year [1-12].
        int16_t day;   ///< Day of the month [1-31].
    };

    struct CTime {
        int16_t hour;    ///< Hours since midnight [0-23].
        int16_t minute;  ///< Minutes after the hour [0-59].
        int16_t second;  ///< Seconds after the minute.
    };

    struct CTimestamp {
        int16_t year;     ///< Year [0-inf).
        int16_t month;    ///< Month of the year [1-12].
        int16_t day;      ///< Day of the month [1-31].
        int16_t hour;     ///< Hours since midnight [0-23].
        int16_t minute;   ///< Minutes after the hour [0-59].
        int16_t second;   ///< Seconds after the minute.
        int32_t fract;    ///< Fractional seconds.
    };

    void delete_datasource (Datasource* datasource);

    void delete_datasource_array (Datasource** datasource_array, int size);

    void delete_driver (Driver* driver);

    void delete_driver_array (Driver** driver_array, int size);

#ifdef __cplusplus
} // extern "C"
#endif