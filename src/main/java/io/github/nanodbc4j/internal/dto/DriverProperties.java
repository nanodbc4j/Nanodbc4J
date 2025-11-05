package io.github.nanodbc4j.internal.dto;

import java.util.List;

public record DriverProperties(String name, List<AttributeProperties> attributes) {
    public record AttributeProperties(String keyword, String value) {
    }
}
