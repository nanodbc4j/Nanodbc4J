package io.github.michael1297.internal.dto;

import java.util.List;

public record OdbcDriver(String name, List<Attribute> attributes) {
    public record Attribute(String keyword, String value) {
    }
}
