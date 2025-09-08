package io.github.michael1297.jdbc;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

@Getter
@RequiredArgsConstructor
public enum SpdLogLevel {
    TRACE(0),
    DEBUG(1),
    INFO(2),
    WARN(3),
    ERROR(4),
    CRITICAL(5),
    OFF(6);

    private final int value;

    public static SpdLogLevel fromInt(int value) {
        for (SpdLogLevel level : values()) {
            if (level.value == value) return level;
        }
        return INFO;    //fallback
    }
}
