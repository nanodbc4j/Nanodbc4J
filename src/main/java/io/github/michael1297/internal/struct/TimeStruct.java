package io.github.michael1297.internal.struct;

import com.sun.jna.Structure;

@Structure.FieldOrder({"hour", "minute", "second"})
public class TimeStruct extends Structure {
    public short hour;    ///< Hours since midnight [0-23].
    public short minute;  ///< Minutes after the hour [0-59].
    public short second;  ///< Seconds after the minute.
}
