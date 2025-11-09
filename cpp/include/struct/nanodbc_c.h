#pragma once
#include <nanodbc/nanodbc.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct Driver {
        /// \brief Driver attributes.
        struct Attribute {
            const wchar_t* keyword = nullptr; ///< Driver keyword attribute.
            const wchar_t* value = nullptr;   ///< Driver attribute value.

            Attribute() = default;
            Attribute(const Attribute& other);
            explicit Attribute(const nanodbc::driver::attribute& other);
            ~Attribute();
        };

        const wchar_t* name = nullptr;            ///< Driver name.
        const Attribute** attributes = nullptr; ///< Driver attributes.
        int attribute_count = 0;

        Driver() = default;
        Driver(const Driver& other);
        explicit Driver(const nanodbc::driver& other);
        ~Driver();

        static Driver** convert(const std::list<nanodbc::driver>& drivers);
    };

    struct Datasource {
        const wchar_t* name = nullptr;   ///< DSN name.
        const wchar_t* driver = nullptr; ///< Driver description.

        Datasource() = default;
        Datasource(const Datasource& other);
        explicit Datasource(const nanodbc::datasource& other);
        ~Datasource();

        static Datasource** convert(const std::list<nanodbc::datasource>& datasources);
    };

    struct CDate {
        int16_t year = 0;  ///< Year [0-inf).
        int16_t month = 1; ///< Month of the year [1-12].
        int16_t day = 1;   ///< Day of the month [1-31].

        CDate() = default;
        CDate(int16_t y, int16_t m, int16_t d);
        CDate(const CDate& other);
        explicit CDate(const nanodbc::date& other);
    };

    struct CTime {
        int16_t hour = 0;    ///< Hours since midnight [0-23].
        int16_t minute = 0;  ///< Minutes after the hour [0-59].
        int16_t second = 0;  ///< Seconds after the minute.

        CTime() = default;
        CTime(int16_t h, int16_t m, int16_t s);
        CTime(const CTime& other);
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
        CTimestamp(int16_t y, int16_t m, int16_t d, int16_t h = 0, int16_t min = 0, int16_t s = 0, int32_t f = 0);
        CTimestamp(const CTimestamp& other);
        explicit CTimestamp(const nanodbc::timestamp& other);
    };    

#ifdef __cplusplus
} // extern "C"
#endif