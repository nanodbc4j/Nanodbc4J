#pragma once
#include <cstdint>
#include <nanodbc/nanodbc.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct Driver {
        /// \brief Driver attributes.
        struct Attribute {
            const char16_t* keyword = nullptr; ///< Driver keyword attribute.
            const char16_t* value = nullptr;   ///< Driver attribute value.

            Attribute() = default;
            explicit Attribute(const Attribute& other);
            explicit Attribute(const nanodbc::driver::attribute& other);
            ~Attribute();
        };

        const char16_t* name = nullptr;            ///< Driver name.
        const Attribute** attributes = nullptr; ///< Driver attributes.
        int attribute_count = 0;

        Driver() = default;
        explicit Driver(const Driver& other);
        explicit Driver(const nanodbc::driver& other);
        ~Driver();
    };

    struct Datasource {
        const char16_t* name = nullptr;   ///< DSN name.
        const char16_t* driver = nullptr; ///< Driver description.

        Datasource() = default;
        explicit Datasource(const Datasource& other);
        explicit Datasource(const nanodbc::datasource& other);
        ~Datasource();
    };

    struct CDate {
        int16_t year = 0;  ///< Year [0-inf).
        int16_t month = 1; ///< Month of the year [1-12].
        int16_t day = 1;   ///< Day of the month [1-31].

        CDate() = default;
        explicit CDate(const CDate& other);
        explicit CDate(const nanodbc::date& other);
    };

    struct CTime {
        int16_t hour = 0;    ///< Hours since midnight [0-23].
        int16_t minute = 0;  ///< Minutes after the hour [0-59].
        int16_t second = 0;  ///< Seconds after the minute.

        CTime() = default;
        explicit CTime(const CTime& other);
        explicit CTime(const nanodbc::time& other);
    };

    struct CTimestamp {
        int16_t year = 0;     ///< Year [0-inf).
        int16_t month = 1;    ///< Month of the year [1-12].
        int16_t day = 1;      ///< Day of the month [1-31].
        int16_t hour = 0;     ///< Hours since midnight [0-23].
        int16_t minute = 0;   ///< Minutes after the hour [0-59].
        int16_t second = 0;   ///< Seconds after the minute.
        int32_t fract = 0;    ///< Fractional seconds.

        CTimestamp() = default;
        explicit CTimestamp(const CTimestamp& other);
        explicit CTimestamp(const nanodbc::timestamp& other);
    };    

#ifdef __cplusplus
} // extern "C"
#endif