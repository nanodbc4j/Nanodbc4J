package io.github.michael1297.internal.struct;

import com.sun.jna.Structure;

@Structure.FieldOrder({"year", "month", "day"})
public final class DateStruct extends Structure {
    public short year; ///< Year [0-inf).
    public short month; ///< Month of the year [1-12].
    public short day;   ///< Day of the month [1-31].
}
