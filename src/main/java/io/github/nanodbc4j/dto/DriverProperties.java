package io.github.nanodbc4j.dto;

import java.util.List;

public record DriverProperties(String name, List<AttributeProperties> attributes) {
    public record AttributeProperties(String keyword, String value) {
    }
}
