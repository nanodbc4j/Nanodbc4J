package io.github.michael1297.core.metadata;

public record Datasource(String name, String driver) {
    @Override
    public String toString() {
        return String.format("ODBC Data Source: %s (Driver: %s)", name, driver);
    }
}
