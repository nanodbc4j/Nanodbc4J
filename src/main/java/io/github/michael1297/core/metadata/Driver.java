package io.github.michael1297.core.metadata;

import java.util.List;

public record Driver(String name, List<Attribute> attributes) {
    public record Attribute (String keyword, String value) {
    }
}
