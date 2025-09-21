package io.github.nanodbc4j.internal.dto;

import java.util.List;

public record DriverDto(String name, List<Attribute> attributes) {
    public record Attribute(String keyword, String value) {
    }
}
