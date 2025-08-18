package io.github.michael1297.internal.struct;

import com.sun.jna.Structure;

@Structure.FieldOrder({"year", "month", "day", "hour", "minute", "second", "fract"})
public final class TimestampStruct extends Structure {
    public short year;     ///< Year [0-inf).
    public short month;    ///< Month of the year [1-12].
    public short day;      ///< Day of the month [1-31].
    public short hour;     ///< Hours since midnight [0-23].
    public short minute;   ///< Minutes after the hour [0-59].
    public short second;   ///< Seconds after the minute.
    public int fract;      ///< Fractional seconds.
}
